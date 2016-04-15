#include "com/editconnectiondialog.h"
#include "ui_editconnectiondialog.h"
#include "maingeneratorwindow.h"


SerialPortConfig EditConnectionDialog::serialConfig;
bool EditConnectionDialog::hasSerialPort = false;

EditConnectionDialog::EditConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditConnectionDialog)
{
    QList< int > brList = QSerialPortInfo::standardBaudRates();;
    QList<QSerialPortInfo> serialInfoLst = QSerialPortInfo::availablePorts();

    ui->setupUi( this );

    if( serialInfoLst.size() > 0 )
    {
        hasSerialPort = true;

        for( QSerialPortInfo curInfo : serialInfoLst )
        {
            ui->portNameComboBox->addItem( curInfo.portName() );
        }

        for( int i : brList )
        {
            ui->baudRateComboBox->addItem( QString::number( i ) );
        }

        dataBitsList.insert( "5 bits", QSerialPort::Data5 );
        dataBitsList.insert( "6 bits", QSerialPort::Data6 );
        dataBitsList.insert( "7 bits", QSerialPort::Data7 );
        dataBitsList.insert( "8 bits", QSerialPort::Data8 );


        stopBitsList.insert( "one bit", QSerialPort::OneStop );
        stopBitsList.insert( "one and a half", QSerialPort::OneAndHalfStop );
        stopBitsList.insert( "two bits", QSerialPort::TwoStop );

        parityList.insert( "No parity", QSerialPort::NoParity );
        parityList.insert( "Even parity", QSerialPort::EvenParity );
        parityList.insert( "Odd parity", QSerialPort::OddParity );

        ui->dataBitsComboBox->addItems( dataBitsList.keys() );
        ui->stopBitsComboBox->addItems( stopBitsList.keys() );
        ui->parityComboBox->addItems( parityList.keys() );

        // setup widget default values

        ui->portNameComboBox->setCurrentText( serialInfoLst.at(0).portName() );
        ui->parityComboBox->setCurrentText( "No parity" );
        ui->dataBitsComboBox->setCurrentText( "8 bits" );
        ui->stopBitsComboBox->setCurrentText( "one bit" );
        ui->baudRateComboBox->setCurrentText( QString::number( 115200 ) );

        setDefaultConfig();
    }
}


void  EditConnectionDialog::setDefaultConfig( )
{
        // setup global settings
        //serialConfig.portName = serialInfoLst.at(0).portName();
        serialConfig.baudRate = QSerialPort::Baud115200;
        serialConfig.databits = QSerialPort::Data8;
        serialConfig.flowControl = QSerialPort::NoFlowControl;
        serialConfig.parity = QSerialPort::NoParity;
        serialConfig.stopbits = QSerialPort::OneStop;
}

 int EditConnectionDialog::on_buttonBox_clicked( QAbstractButton* button )
 {
     QDialogButtonBox::StandardButton stdBtn = ui->buttonBox->standardButton( button );
     int ret = 0;

     switch( stdBtn )
     {
        case QDialogButtonBox::Ok:

         serialConfig.baudRate =  static_cast< QSerialPort::BaudRate >( ui->baudRateComboBox->currentText().toInt() );

         serialConfig.databits =  dataBitsList.value( ui->dataBitsComboBox->currentText() );
         serialConfig.parity =  parityList.value( ui->parityComboBox->currentText() );
         serialConfig.stopbits = stopBitsList.value( ui->stopBitsComboBox->currentText() );
         serialConfig.portName = ui->portNameComboBox->currentText();


         break;

     case QDialogButtonBox::Cancel:
     default:

         break;
     }

     return ret;
 }




EditConnectionDialog::~EditConnectionDialog()
{
    delete ui;
}
