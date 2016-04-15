#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

#include "editscriptdialog.h"
#include "ui_editscriptdialog.h"
#include "dbcModels/dbcfilebrowserdialog.h"

EditScriptDialog::EditScriptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditScriptDialog)
{
    ui->setupUi(this);

       //set local members
    addGroupBtn =  ui->addGroupBtn;
    addMsgBtn =  ui->addMsgBtn;
    deleteGroupBtn = ui->deleteGroupBtn;
    deleteMsgBtn = ui->deleteMsgButton;
    groupListWidget =  ui->groupListWidget;
    loadScriptBtn = ui->loadScriptBtn;
    newScriptBtn =  ui->newScriptBtn;
    saveScriptBtn = ui->saveScriptBtn;
    scriptListWidget = ui->scriptListWidget;
    groupDescLineEdit = ui->groupDescLineEdit;
    groupDelaySpinBox = ui->groupDelaySpinBox;

    groupTableView =  ui->GroupTableView;
    msgTableView = ui->SignalTableView;

   connect( addGroupBtn, SIGNAL(clicked()), this, SLOT(addGroupBtn_clicked()) );
   connect( addMsgBtn, SIGNAL(clicked()), this, SLOT(addMsgBtn_clicked()) );
   connect( deleteGroupBtn, SIGNAL(clicked()), this, SLOT( deleteGrpBtn_clicked() ) );
   connect( deleteMsgBtn, SIGNAL(clicked()), this, SLOT(deletemsgBtn_clicked() ) );
   connect( loadScriptBtn, SIGNAL(clicked()), this, SLOT( loadScriptBtn_clicked() ) );
   connect( newScriptBtn, SIGNAL(clicked()), this, SLOT(newScriptBnt_clicked()) );
   connect( saveScriptBtn, SIGNAL(clicked()), this, SLOT(saveScriptBtn_clicked() ) );
   connect( groupDescLineEdit, SIGNAL(editingFinished()), SLOT( groupDescEdit_finished() ) );
   connect( groupDelaySpinBox, SIGNAL(editingFinished()), this, SLOT( groupDelaySpinBox_finished() ) );

   connect( scriptListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(scriptListSelection_changed()) );


}

void EditScriptDialog::init(QMap< QString, JSONScript > &scriptListParam , QList< DbcData* > dbcDataParam )
{
    QList< CanMsgGroup > groupList;

    if( !scriptListParam.isEmpty() )
    {
        scriptListWidget->addItems( QStringList( scriptListParam.keys() ) );

        //grab first script first group as default
        currentScript =  scriptListParam.values().at( 0 );

        groupList = currentScript.getGroupList();

        // add groups to list
        if( !groupList.isEmpty() )
        {
            for( CanMsgGroup curGroup : groupList )
            {
                groupListWidget->addItem( curGroup.name );
            }

            currentGroup = groupList.at( 0 );
            scriptList = scriptListParam;

            updateViews();
        }
    }

    dbcList = dbcDataParam;
}

void EditScriptDialog::updateViews( )
{
    groupDescLineEdit->setText( currentGroup.description );

    // TODO: complete
    groupTableModel.setModelData( currentGroup );

    groupTableView->setModel( &groupTableModel );

    groupTableView->show();

    if( !currentGroup.messages.isEmpty() )
    {
        //TODO uncomment when model is complete
        //msgTableModel.setModelData( currentGroup.messages.at( 0 ) );
        //msgTableView->setModel( &msgTableModel );
        //msgTableView->show();
    }

    qDebug( ) << "Group view updated to show script: " + currentScript.getFileName();
    qDebug( ) << "Group view updated to show group: " + currentGroup.name;
}



void EditScriptDialog::getEditedScripts( QMap< QString, JSONScript >& scriptListParam )
{
    scriptListParam = scriptList;
}


void EditScriptDialog::addGroupBtn_clicked()
{
    QString groupName = "";
    CanMsgGroup newGroup;

        // valid script loaded?
    if ( currentScript.getFileName() != "" )
    {
        groupName = QInputDialog::getText( this, "Group Name", "Enter group Name: " );
        newGroup.name = groupName;

        currentScript.addGroup( newGroup );
        currentGroup = newGroup;

        // update group and message models
        updateViews();

        qDebug() << "New group added : " << groupName;
    }
    else
    {
       qDebug() << "Current script empty";
    }
}

void EditScriptDialog::addMsgBtn_clicked()
{
    DBCFileBrowserDialog dbcBrowser;
    QString msgName;
    QList< QDbcItem* > selItemList;
    CanMsg canMsg;
    DbcData::dbcMessage dbcMsg;

    dbcBrowser.init( dbcList );
    dbcBrowser.exec();

    selItemList = dbcBrowser.getSelectedItems();

    if( ( selItemList.size() > 0 ) )
    {
        // get / build can message
        for( DbcData* curData : dbcList )
        {
            for( QDbcItem* curItem : selItemList )
            {
                 msgName = curItem->name;

                if ( curData->getMessage( msgName, dbcMsg ) )
                {
                    canMsg.setupMsg( msgName, dbcMsg.signalList, curData );

                    // add to current group
                    currentGroup.addMsg( canMsg );
                    qDebug() << "msg added to group";
                    qDebug() << msgName;
                }
                else
                {
                    qDebug( ) << "message not found";
                    qDebug() << msgName;
                }
            }
        }

        groupTableModel.setModelData( currentGroup );
        groupTableView->setModel( &groupTableModel );
        groupTableView->show();
    }
}

void EditScriptDialog::deleteGrpBtn_clicked( )
{
    QListWidgetItem* curItem = nullptr;
    QString groupName;

    curItem = groupListWidget->currentItem();

    if( curItem != nullptr )
    {
       groupName = curItem->data( Qt::DisplayRole ).toString();
       groupListWidget->removeItemWidget( curItem );

       if( currentGroup.name == groupName )
       {
           groupTableView->reset();
           groupTableView->setModel( nullptr );
       }
    }
}

void EditScriptDialog::deletemsgBtn_clicked()
{
    QModelIndex curMsgIdx;
    QList< CanMsg > messageList;
    CanMsg* msg = nullptr;


    curMsgIdx = groupTableView->currentIndex();

    if( curMsgIdx.isValid() )
    {
        msg = static_cast< CanMsg* >( curMsgIdx.internalPointer() );

        if( msg != nullptr )
        {            
            messageList = currentGroup.getMessages();

            //remove message option 1
            messageList.removeAt(  curMsgIdx.row() );

            currentGroup.setMessages( messageList );
            groupTableModel.setModelData( currentGroup );
            groupTableView->setModel( &groupTableModel);
            groupTableView->show();

            // search and remove message option 2
//            for( int curIdx = 0 ; curIdx < messageList.size() ; curIdx++ )
//            {

//                if( messageList.at( curIdx ).name == msg->name )
//                {
//                    messageList.removeAt( curIdx  );
//                    break;
//                }
//            }
        }
    }

}

void EditScriptDialog::loadScriptBtn_clicked()
{
    QString filePath = "";
    JSONScript script;
    QUrl url;
    QString errStr;

    filePath = QFileDialog::getOpenFileName( this,
                                            tr("Open json script file"), "./", tr("JSON Files (*.json)"));

    if( filePath != "" )
    {
        url.setUrl( filePath );
        script.setUrl( url );

        if ( script.parse( errStr ) )
        {
            scriptList.insert( script.getFileName(), script );

            currentScript = script;

            // set script data into group / message views
            if( !script.getGroupList().isEmpty() )
            {
                //TODO: make it work for custom messages
                currentGroup = script.getGroupList().at( 0 );
                groupTableModel.setModelData( currentGroup );
                // set new group view model
                groupTableView->setModel( &groupTableModel );

                if( !currentGroup.getMessages().isEmpty() )
                {
                    currentMsg = currentGroup.getMessages( ).at( 0 );
                    msgTableModel.setModelData( currentMsg );
                    //set new msg view model
                    msgTableView->setModel( &msgTableModel );
                }
            }
            else
            {
                qDebug( "empty script loaded" );
            }
        }
        else
        {
           QMessageBox::warning( this, tr("Script not valid"), tr("An error occurred while parsing script file") + errStr );
        }
    }
}

void EditScriptDialog::newScriptBnt_clicked()
{
    QString scriptName;
    JSONScript newScript;
    CanMsgGroup newEmptyGroup;
    CanMsg newEmptyMsg;
    QListWidgetItem* listItem;


    scriptName = QInputDialog::getText( this, "Script Name", "Enter script name: " );

    if( scriptName != "" )
    {
        listItem->setData( Qt::DisplayRole, QVariant( scriptName ) );
        // add empty script
        scriptList.insert( scriptName, newScript );
        scriptListWidget->addItem( listItem );
        scriptListWidget->setCurrentItem( listItem );

        // update to empty views
        groupTableModel.setModelData( newEmptyGroup );
        msgTableModel.setModelData( newEmptyMsg );

        groupTableView->reset();
        groupTableView->setModel( &groupTableModel );

        msgTableView->reset();
        msgTableView->setModel( &msgTableModel );
    }

}

void EditScriptDialog::saveScriptBtn_clicked()
{
    QString err;

    for( JSONScript script : scriptList.values() )
    {
        if( !script.save( err ) )
        {
           qDebug( ) << "error while saving script file: " + script.getFileName();
        }
    }

    //exit dialog
    done(0);

    qDebug( "edit script dialog saved all" );
}

void EditScriptDialog::groupDescEdit_finished()
{

}

void EditScriptDialog::groupDelaySpinBox_finished()
{

}

void EditScriptDialog::scriptListSelection_changed( )
{
    QString newScriptName;

    {
        newScriptName = scriptListWidget->currentIndex().data( Qt::DisplayRole ).toString();
        groupListWidget->clear();
        setCurrentScript( newScriptName );
    }
}

bool EditScriptDialog::setCurrentScript( QString scriptName )
{
    JSONScript curScript;
    bool ret;
    QList< CanMsgGroup > groupList;


    if ( scriptList.keys().contains(scriptName ) )
    {

        curScript = scriptList.value( scriptName );
        groupList = curScript.getGroupList();

        groupListWidget->clear();

        // set group names
        for( CanMsgGroup curGroup : groupList )
        {
            groupListWidget->addItem(curGroup.name);
        }

        currentScript = curScript;
        setCurrentGroup( curScript.getGroupList().at( 0 ).name );

    }
    else
    {
        qDebug() << "Script not found: " << scriptName;
        ret = false;
    }
    return ret;
}

bool EditScriptDialog::setCurrentGroup( QString  groupName )
{
    bool ret = true;
    QList< CanMsg > msgList;
    QList< CanMsgGroup > groupList;

    groupList = currentScript.getGroupList();

    for( CanMsgGroup curGroup : groupList )
    {

        if( curGroup.name == groupName  )
        {
            currentGroup = curGroup;
            msgList = curGroup.getMessages();

            if( !msgList.isEmpty() )
            {
            // set first message as default
                currentMsg = msgList.at( 0 );
            }
        }
    }

    if( currentGroup.name == groupName )
    {
        updateViews();
        qDebug( "current group updated" );
    }
    else
    {
       qDebug( ) << "group is not valid: " << groupName;
       ret = false;
    }
    return ret;
}

EditScriptDialog::~EditScriptDialog()
{
    delete ui;
}
