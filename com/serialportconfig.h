#ifndef SERIALPORTCONFIG_H
#define SERIALPORTCONFIG_H


#include <QtSerialPort/QtSerialPort>

class SerialPortConfig
{
public:
    SerialPortConfig();

    QSerialPort* serPort;
    QString portName;
    QSerialPort::BaudRate baudRate;
    QSerialPort::FlowControl flowControl;
    QSerialPort::DataBits databits;
    QSerialPort::StopBits stopbits;
    QSerialPort::Parity parity;


    bool configured;

    bool valid( void );
    bool configure( void );

};

#endif // SERIALPORTCONFIG_H
