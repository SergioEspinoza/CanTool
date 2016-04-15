#include "com/serialcomdelegate.h"


QSerialPort* SerialComDelegate::serPort(nullptr);
QSemaphore SerialComDelegate::serSema(1);


SerialComDelegate::SerialComDelegate( )
{

}

SerialComDelegate::~SerialComDelegate( )
{
    if( isOpen )
    {
        delete serPort;
    }
}

void SerialComDelegate::setSerialConfig( SerialPortConfig& config )
{
   serialConfig = config;
}

bool SerialComDelegate::open()
{
    bool ret = false;

    if( serPort == nullptr )
    {
       serPort = new QSerialPort();

       serPort->setPortName( serialConfig.portName );
       serPort->setBaudRate( serialConfig.baudRate );
       serPort->setParity( serialConfig.parity );
       serPort->setFlowControl( QSerialPort::NoFlowControl );
       serPort->setDataBits( serialConfig.databits );
       serPort->setStopBits( serialConfig.stopbits );

       if( serPort->open( QSerialPort::ReadWrite )  )
       {
           isOpen = true;
           ret = true;
       }
       else
       {
           qDebug( "error while opening serial port" );
       }
    }

    return ret;
}

void SerialComDelegate::close( )
{
   if( isOpen )
   {
       serPort->close();
   }
}

bool SerialComDelegate::send( QByteArray& message )
{
    bool ret = false;

    if( !isOpen )
    {
        serSema.acquire();

        if( serPort->open( QSerialPort::ReadWrite ) )
        {
            qDebug( "serial port open on : %s : ",
                    serialConfig.portName.toStdString().c_str() );
        }

        serSema.release();

    }

    serSema.acquire();

    if( serPort->write( message ) )
    {
        ret = true;
    }
    else
    {
        qDebug( "Failed to send serial message" );
    }

    serSema.release();
    return ret;
}

