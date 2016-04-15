#ifndef EDITCONNECTIONDIALOG_H
#define EDITCONNECTIONDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/QSerialPortInfo>

#include "com/serialportconfig.h"

namespace Ui {
class EditConnectionDialog;
}

class EditConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditConnectionDialog(QWidget *parent = 0);
    ~EditConnectionDialog();



    void  setDefaultConfig( );

    static SerialPortConfig serialConfig;

    static bool hasSerialPort;

private slots:
    int on_buttonBox_clicked( QAbstractButton* button );



private:
    Ui::EditConnectionDialog *ui;

    QHash< QString, QSerialPort::DataBits > dataBitsList;
    QHash< QString, QSerialPort::StopBits > stopBitsList;
    QHash< QString, QSerialPort::Parity > parityList;

};

#endif // EDITCONNECTIONDIALOG_H
