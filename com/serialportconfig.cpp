
#include <QtSerialPort/QSerialPortInfo>
#include "com/serialportconfig.h"


SerialPortConfig::SerialPortConfig() : portName(""), flowControl( QSerialPort::NoFlowControl )
{
}


bool SerialPortConfig::valid( void )
{
    QList< int > availBrLst = QSerialPortInfo::standardBaudRates();
    QList<QSerialPortInfo> serialInfoLst = QSerialPortInfo::availablePorts();
    QList<QString> availPortNames;
    bool ret = false;


    if( portName.size() != 0 )
    {
       for( QSerialPortInfo systemPort : serialInfoLst )
       {
           availPortNames.append( systemPort.portName() );
       }

       if( availPortNames.contains( portName ) &&
            availBrLst.contains( baudRate ) )
       {
            ret = true;
       }

    }
    return ret;
}


bool SerialPortConfig::configure( void )
{
    bool ret = 0;

    if( valid() )
    {

        if( serPort == nullptr )
        {
            serPort = new QSerialPort( portName );
            ret = serPort->open( QIODevice::ReadWrite );
        }
        else if( serPort->isOpen() &&
                 serPort->portName() != portName )
        {
            serPort->close( );
            serPort->setPortName( portName );
            ret = serPort->open( QIODevice::ReadWrite );
        }

        if ( ret )
        {
            serPort->setBaudRate( baudRate );
            serPort->setParity( parity );
            serPort->setStopBits( stopbits );
            serPort->setDataBits( databits );
            serPort->setFlowControl( flowControl );
        }
    }

    return ret;
}
