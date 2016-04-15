#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QtSerialPort/qserialport.h>
#include <QMessageBox>

#include "igGUI/vintage/custommsgtablemodel.h"
#include "libcandbc/dbcTypes.h"
#include "libcandbc/dbcModel.h"
#include "dbcModels/qdbcmodel.h"
#include "com/serialportconfig.h"
#include "dbcModels/dbcdata.h"
#include "scriptMainGUI/scriptguicontroller.h"
#include "com/editconnectiondialog.h"
#include "com/serialcomdelegate.h"
#include "scriptEditGUI/groupedittablemodel.h"
#include "dbcModels/dbcfilebrowserdialog.h"
#include "scriptEditGUI/editscriptdialog.h"

namespace Ui {
class MainGeneratorWindow;
}


class MainGeneratorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainGeneratorWindow(QWidget *parent = 0);
    ~MainGeneratorWindow();

    //Test code
    CustomMsgTableModel* msgModel;
    GroupEditTableModel* groupEditModel;


    QTableView* msgTableView;

    static QList< DbcData* > dbcDataList;
    static ComInterface* activeComInterface;

private:
    Ui::MainGeneratorWindow *ui;

    EditConnectionDialog editConDialog;
    SerialComDelegate serialComDelegate;
    ScriptGUIController scriptGUIController;


private slots:

     void on_actionLoadDbcFile_triggered( void );
     void on_actionLoadScript_triggered( void );
     void on_actionEditConnection_triggered( void );
     void on_actionViewDBCBrowser_triggered( void );
     void on_actionEditScript_triggered( void );

};

#endif // MAINWINDOW_H
