#include <QMessageBox>
#include <QFileDialog>

#include "dbcfilebrowserdialog.h"
#include "ui_dbcfilebrowserdialog.h"
#include "libcandbc/dbcModel.h"

DBCFileBrowserDialog::DBCFileBrowserDialog(QWidget *parent) :
    QDialog(parent), curDbcIdx( 0 ),
    ui(new Ui::dbcFileBrowserDialog)
{
    ui->setupUi(this);
        // assign to local elements
    addDbcFileBtn = ui->addDbcFileBtn;
    deleteDbcFileBtn = ui->deleteDbcFileBtn;
    dbcFileList = ui->dbcFileListWidget;
    dbcTreeView = ui->dbcTreeView;
    selectMsgBtn = ui->selectMsgBtn;

}


void  DBCFileBrowserDialog::init( QList<DbcData *> &dbcListParam )
{
    connect( addDbcFileBtn, SIGNAL(clicked()), this, SLOT(addDbcFileBtn_clicked()) );
    connect( deleteDbcFileBtn, SIGNAL(clicked()), this, SLOT(deleteDbcFileBtn_clicked()) );
    connect( dbcFileList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
             this, SLOT(dbcFileList_currentChanged(QListWidgetItem*)) );
    connect( dbcTreeView, SIGNAL(activated(QModelIndex)), this, SLOT(treeIndexActivated(QModelIndex)) );
    connect( dbcTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(treeItemClicked(QModelIndex)) );
    connect( dbcTreeView, SIGNAL(doubleClicked( QModelIndex )), this, SLOT(treeItemDoubleClicked(QModelIndex)) );

    connect( selectMsgBtn, SIGNAL(clicked()), this, SLOT(selectMsgBtn_clicked()) );

    setDbcList( dbcListParam );
}

void DBCFileBrowserDialog::showEvent(QShowEvent * event)
{
    QDbcModel* curModel;
    qDebug() << "dbc browser dialog shown";

    if( dbcTreeView != nullptr )
    {
        // show first item in dbc file list
       if( ( dbcList.size() ) > 0 )
       {
            curModel = getDbcModelFromList( dbcList.at(0)->name, dbcList );

            dbcTreeView->setModel( curModel );
            dbcTreeView->show();
       }
    }
    else
    {
        qDebug() << " dbc file browser empty view / item list!!";
    }


    dbcTreeView->setSelectionMode( QAbstractItemView::ExtendedSelection );

}


 void DBCFileBrowserDialog::setDbcList( QList<DbcData *> &dbcListParam )
{
   dbcList = dbcListParam;

   dbcFileList->clear();

   for( DbcData* curDbc : dbcList )
   {
       dbcFileList->addItem( curDbc->name );
   }

}

QList< DbcData* > DBCFileBrowserDialog::getDbcList( )
{
    return dbcList;
}

QDbcModel* DBCFileBrowserDialog::getDbcModelFromList( QString name, QList< DbcData* > dataList )
{
   QDbcModel* retModel = nullptr;

   for( DbcData* curData : dataList )
   {
       if( curData->name == name )
       {
           retModel = curData->getDbcModel();
           break;
       }
   }
   return retModel;
}

void  DBCFileBrowserDialog::add_newDbcFiles( )
{
    QStringList fileNames;
    dbc_t* dbcStruct = nullptr;
    DbcData* dbcData = nullptr;

    fileNames = QFileDialog::getOpenFileNames(this,
                                            tr("Open DBC file"), "./", tr("DBC Files (*.dbc)"));

    if( fileNames.size() > 0 )
    {
        for( QString curFileName : fileNames )
        {
            QUrl url( curFileName );
            // uncomment when using actual dll
            QByteArray *array = new QByteArray(curFileName.toLocal8Bit());
            char* buffer = array->data();
            dbcStruct = dbc_read_file( buffer );

            if( dbcStruct != nullptr )
            {
                dbcData = new DbcData( curFileName );

                if ( dbcData->initFromDbcStruct( dbcStruct, url.fileName( ) ) )
                {
                    dbcList.append( dbcData );
                    qDebug( "DbcData initialization succes" );
                    QMessageBox::information( this, "DBC file loaded", "DBC file succesfully loaded: " + curFileName );
                }
            }
            else
            {
                qDebug( "Unable to read dbc file" );
                QMessageBox::warning( this, "DBC file not valid: ", "File is not valid: " + curFileName );
            }
        }
    }
}

 QList < QDbcItem* > DBCFileBrowserDialog::getSelectedItems( )
{
    return selectedMsgLst;
}

void  DBCFileBrowserDialog::addDbcFileBtn_clicked()
{
    QDbcModel* qdbcModel = nullptr;
    //launch dialog
    add_newDbcFiles();

    //update file view / model
    if( dbcList.size() > 0 )
    {
        dbcFileList->clear();

        for( DbcData* curItem : dbcList )
        {
            dbcFileList->addItem( curItem->name );
        }

        // show first item
        qdbcModel = getDbcModelFromList( dbcList.at( 0 )->name, dbcList );

        if( qdbcModel != nullptr )
        {
            dbcTreeView->setModel( qdbcModel );
            dbcTreeView->show();
        }
    }
}

void DBCFileBrowserDialog::deleteDbcFileBtn_clicked()
{
     QList<QListWidgetItem*> selectedItems;
     QString fileName = "";

     selectedItems = dbcFileList->selectedItems();

    if( selectedItems.size() > 0 )
    {
        // remove all selected items
        for( QListWidgetItem* curItem : selectedItems )
        {
             fileName = curItem->data( Qt::DisplayRole ).toString();

             for(  DbcData* curDbc :  dbcList  )
             {
                if( curDbc->name == fileName  )
                {
                    dbcList.removeOne( curDbc );
                    break;
                }
             }
        }
    }
}

void DBCFileBrowserDialog::selectMsgBtn_clicked()
{
    //QModelIndex curIdx;
    QModelIndexList selectedIndexList;
    QDbcItem* curDbcItem;
    QItemSelectionModel* selectionModel = dbcTreeView->selectionModel();

    //curIdx = dbcTreeView->currentIndex( );

    selectedIndexList = selectionModel->selectedIndexes();

    //curDbcItem = static_cast< QDbcItem* >( curIdx.internalPointer( ) );

    //if( ( curDbcItem != nullptr ) && curDbcItem->type == MESSAGE_TYPE )
    //{
      //  selectedMsgLst = curDbcItem->name;

        //done( 0 );
    //}

    for( QModelIndex curSelIdx : selectedIndexList )
    {
        // first column is message name
        if( curSelIdx.column( ) == 0 )
        {
            curDbcItem = static_cast< QDbcItem* >( curSelIdx.internalPointer( ) );

            if( curDbcItem != nullptr &&
                    curDbcItem->type == MESSAGE_TYPE )
            {
                selectedMsgLst.append( curDbcItem );
            }
        }
    }

    done( 0 );
}

void DBCFileBrowserDialog::treeIndexActivated( QModelIndex idx )
{
    QDbcItem* activeItem = nullptr;

    activeItem = static_cast< QDbcItem* >( idx.internalPointer() );

    if( ( activeItem != nullptr ) && ( activeItem->type == MESSAGE_TYPE ) )
    {
        selectedMsgLst.append( activeItem );
        done( 0 );
    }
}

void DBCFileBrowserDialog::treeItemClicked( QModelIndex idx )
{
    QDbcItem* activeItem = nullptr;

    activeItem = static_cast< QDbcItem* >( idx.internalPointer() );

    if( ( activeItem != nullptr ) && ( activeItem->type == MESSAGE_TYPE ) )
    {
        selectMsgBtn->setEnabled( true );
    }
    else
    {
        selectMsgBtn->setEnabled( false );
    }

}


void DBCFileBrowserDialog::treeItemDoubleClicked( QModelIndex idx )
{
    QDbcItem* item = nullptr;

    item = static_cast< QDbcItem* >( idx.internalPointer() );

    if( ( item != nullptr ) && ( item->type == MESSAGE_TYPE  ) )
    {
        selectedMsgLst.append( item );
    }
    done( 0 );
}


void DBCFileBrowserDialog::dbcFileList_currentChanged( QListWidgetItem *wdgt )
{
    QString fileName = "";
    QDbcModel* qdbcModel;

    if( wdgt != nullptr )
    {
        qDebug( "Dbc file changed" );
        fileName = wdgt->data( Qt::DisplayRole ).toString();
        qdbcModel = getDbcModelFromList( fileName, dbcList );

        if( qdbcModel != nullptr )
        {
            dbcTreeView->reset();
            dbcTreeView->setModel( qdbcModel );
            dbcTreeView->show();
        }
    }

}

DBCFileBrowserDialog::~DBCFileBrowserDialog()
{
    delete ui;
}
