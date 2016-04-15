#include <QtSerialPort/qserialport.h>
#include <QFileDialog>
#include <QMessageBox>

#include "maingeneratorwindow.h"
#include "ui_maingeneratorwindow.h"


#include "scriptEditGUI/groupedittablemodel.h"

#include "libcandbc/dbcReader.h"
#include "libcandbc/dbcModel.h"
#include "com/serialportconfig.h"
#include "com/editconnectiondialog.h"

//#define DBC_FILE_NAME   "CCAN.dbc"

QList< DbcData* > MainGeneratorWindow::dbcDataList;

ComInterface* MainGeneratorWindow::activeComInterface = nullptr;

MainGeneratorWindow::MainGeneratorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainGeneratorWindow),
    msgModel( nullptr ) ,
    msgTableView( nullptr )
{
    QList<QSerialPortInfo> serialInfoLst = QSerialPortInfo::availablePorts();


   ui->setupUi( this );

   if( serialInfoLst.size() > 0 )
   {
       serialComDelegate.setSerialConfig( EditConnectionDialog::serialConfig );

       if( !serialComDelegate.open() )
       {

           QMessageBox::warning( this, "ERROR", "Unable to open defualt serial port" );
       }

   }
   else
   {
      QMessageBox::warning( this, "ERROR", "Unable to detect any working serial port" );
   }


   activeComInterface = &serialComDelegate;
   scriptGUIController.init( ui );
   scriptGUIController.loadDefaultSelectionList();

   // TEST CODE!!!
   //groupEditModel= new GroupEditTableModel();
   //msgTableView = ui->MsgTableView;
   //msgTableView->setModel( groupEditModel );
   //msgTableView->show();

}


void MainGeneratorWindow::on_actionLoadDbcFile_triggered( void )
{
    DBCFileBrowserDialog dbcBrowser;

    dbcBrowser.setDbcList( dbcDataList );
    dbcBrowser.add_newDbcFiles();
    dbcDataList = dbcBrowser.getDbcList();
}

void MainGeneratorWindow::on_actionLoadScript_triggered( void )
{
    QString filePath = "";
    QUrl url;

    filePath = QFileDialog::getOpenFileName(this,
                                            tr("Open json script file"), "./", tr("JSON Files (*.json)"));

    if( filePath != "" )
    {
        url.setUrl( filePath );
        scriptGUIController.addToSelectionList( url );
    }
}

void MainGeneratorWindow::on_actionEditConnection_triggered( void )
{
    editConDialog.show();

    serialComDelegate.setSerialConfig( EditConnectionDialog::serialConfig );
}

void  MainGeneratorWindow::on_actionViewDBCBrowser_triggered( void )
{
    DBCFileBrowserDialog dbcBrowser;

    dbcBrowser.init( dbcDataList );
    dbcBrowser.exec();

    //update databse list
    dbcDataList = dbcBrowser.getDbcList();
}

void MainGeneratorWindow::on_actionEditScript_triggered( void )
{
    EditScriptDialog editScriptDialog;
    QMap< QString, JSONScript > editScriptMap;
    ScriptController* scriptController;

    scriptController = scriptGUIController.getScriptController();

    if( scriptController->isRunning() )
    {
        scriptController->stop();
    }

    scriptController->getLoadedScripts( editScriptMap );

    if( !editScriptMap.values().isEmpty() )
    {
        editScriptDialog.init( editScriptMap, MainGeneratorWindow::dbcDataList );
        editScriptDialog.exec();
        editScriptDialog.getEditedScripts( editScriptMap );

        scriptController->loadScripts( editScriptMap );
    }

    scriptController->start();
}

MainGeneratorWindow::~MainGeneratorWindow()
{
    delete ui;
}

