#ifndef SERIALCOMDELEGATE_H
#define SERIALCOMDELEGATE_H

#include <QSerialPort>
#include <QByteArray>
#include <QSemaphore>

#include "com/serialportconfig.h"
#include "com/cominterface.h"


class SerialComDelegate : public ComInterface
{
public:
    explicit SerialComDelegate();
    ~SerialComDelegate( );

    void setSerialConfig( SerialPortConfig& config );


    bool open( ) Q_DECL_OVERRIDE;
    void close( ) Q_DECL_OVERRIDE;
    bool send( QByteArray& message ) Q_DECL_OVERRIDE;


 private:
    SerialPortConfig serialConfig;

    static QSemaphore serSema;
    static QSerialPort* serPort;

};

#endif // SERIALCOMDELEGATE_H
