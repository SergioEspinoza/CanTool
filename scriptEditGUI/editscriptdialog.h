#ifndef EDITSCRIPTDIALOG_H
#define EDITSCRIPTDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QTableView>
#include <QLineEdit>
#include <QSpinBox>

#include "scriptMainGUI/jsonscript.h"
#include "scriptEditGUI/groupedittablemodel.h"
#include "scriptEditGUI/msgedittablemodel.h"
#include "dbcModels/dbcdata.h"

namespace Ui {
class EditScriptDialog;
}

class EditScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditScriptDialog(QWidget *parent = 0);
    ~EditScriptDialog();


    void init(QMap<QString, JSONScript>& scriptListParam, QList<DbcData *> dbcDataParam);
    void updateViews( );

    void getEditedScripts(  QMap< QString, JSONScript >& scriptListParam );


private:

    bool setCurrentScript( QString scriptName );
    bool setCurrentGroup( QString  groupName );


    Ui::EditScriptDialog *ui;

    QPushButton* addGroupBtn;
    QPushButton*  addMsgBtn;
    QPushButton* deleteGroupBtn;
    QPushButton* deleteMsgBtn;
    QListWidget*  groupListWidget;
    QTableView* groupTableView;
    QPushButton* loadScriptBtn;
    QPushButton* newScriptBtn;
    QPushButton* saveScriptBtn;
    QListWidget* scriptListWidget;
    QTableView* msgTableView;
    QLineEdit*  groupDescLineEdit;
    QSpinBox*   groupDelaySpinBox;

    QMap< QString, JSONScript > scriptList;
    QList< DbcData* > dbcList;

    JSONScript currentScript;
    CanMsgGroup currentGroup;
    CanMsg      currentMsg;
    GroupEditTableModel groupTableModel;
    MsgEditTableModel msgTableModel;

private slots:

    void addGroupBtn_clicked();
    void addMsgBtn_clicked();
    void deleteGrpBtn_clicked();
    void deletemsgBtn_clicked();
    void loadScriptBtn_clicked();
    void newScriptBnt_clicked();
    void saveScriptBtn_clicked();
    void groupDescEdit_finished();
    void groupDelaySpinBox_finished();
    void scriptListSelection_changed();


};

#endif // EDITSCRIPTDIALOG_H
