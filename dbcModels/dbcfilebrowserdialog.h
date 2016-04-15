#ifndef DBCFILEBROWSERDIALOG_H
#define DBCFILEBROWSERDIALOG_H

#include <QDialog>
#include <QListWidget>

#include "dbcModels/dbcdata.h"

namespace Ui {
class dbcFileBrowserDialog;
}

class DBCFileBrowserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBCFileBrowserDialog(QWidget *parent = 0);
    ~DBCFileBrowserDialog();

    void init( QList<DbcData *> &dbcListParam);

    void setDbcList( QList< DbcData* >& dbcListParam );
    QList< DbcData* >  getDbcList( );

    static QDbcModel* getDbcModelFromList( QString name, QList< DbcData* > dataList );
    void  add_newDbcFiles(  );

    QList<QDbcItem *> getSelectedItems( );

public slots:

    void addDbcFileBtn_clicked();

protected:
    void showEvent(QShowEvent * event) override;


private:
    Ui::dbcFileBrowserDialog *ui;

     //TODO: assign to local elements
    QPushButton*  addDbcFileBtn;
    QPushButton* deleteDbcFileBtn;
    QPushButton*  selectMsgBtn;
    QListWidget*  dbcFileList;
    QTreeView* dbcTreeView;

    QList< DbcData*> dbcList;
    int curDbcIdx;

    QList < QDbcItem* > selectedMsgLst;

private slots:

    void deleteDbcFileBtn_clicked();
    void dbcFileList_currentChanged( QListWidgetItem *wdgt );
    void selectMsgBtn_clicked();
    void treeIndexActivated( QModelIndex idx );
    void treeItemClicked(  QModelIndex idx );  
    void treeItemDoubleClicked( QModelIndex idx );

};

#endif // DBCFILEBROWSERDIALOG_H
