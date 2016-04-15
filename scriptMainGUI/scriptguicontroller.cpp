
#include "ui_maingeneratorwindow.h"
#include "scriptMainGUI/scriptguicontroller.h"
#include "maingeneratorwindow.h"

ScriptGUIController::ScriptGUIController(QObject *parent) : QObject(parent),
  selectionListWidget( nullptr ), execListWidget( nullptr ), groupMsgListWidget( nullptr )
{
}


bool ScriptGUIController::init( Ui::MainGeneratorWindow *ui )
{
    bool ret = false;

    //List Widgets
     selectionListWidget = ui->scriptSelectionList;
     execListWidget = ui->scriptExecutionList;
     groupMsgListWidget = ui->scriptMsgGroupList;

     //script Control buttons

     playBtn = ui->playScriptBtn;
     pauseBtn = ui->pauseScriptBtn;
     stopBtn = ui->stopScriptBtn;
     prevBtn = ui->prevScriptBtn;
     nextBtn = ui->nextScriptBtn;

     addScript = ui->addScriptBtn;
     clearScript = ui->clearScriptBtn;
     playAllCkBox = ui->playAllCheckBox;
     scriptNameLabel = ui->scriptNameLabel;

     scriptNameLabel->setText( "" );

     btnList.append( playBtn );
     btnList.append( pauseBtn );
     btnList.append( stopBtn );
     btnList.append( prevBtn );
     btnList.append( nextBtn );

    connectScriptGUISignals();

    setScriptComInterface( MainGeneratorWindow::activeComInterface );

    scriptController.start();

    return ret;
}

void ScriptGUIController::connectScriptGUISignals()
{
    // list items
     connect ( selectionListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selListItem_doubleClicked( QListWidgetItem* )) );

     connect ( execListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(execListItem_doubleClicked( QListWidgetItem* )) );
     connect ( execListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)) , this, SLOT(execListItem_currentChanged(QListWidgetItem*)));

     connect ( groupMsgListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(groupMsgItem_doubleClicked( QListWidgetItem* )) );

     //script control
     connect( playBtn, SIGNAL(clicked()), this, SLOT(playButton_clicked()) );
     connect( pauseBtn, SIGNAL(clicked()), this, SLOT(pauseButton_clicked()) );
     connect( stopBtn, SIGNAL(clicked()), this, SLOT(stopButton_clicked()) );
     connect( prevBtn, SIGNAL(clicked()), this, SLOT(prevButton_clicked()) );
     connect( nextBtn, SIGNAL(clicked()), this, SLOT(nexButton_clicked()) );

    // add / clear
     connect( addScript, SIGNAL(clicked()), this, SLOT(addScriptBtn_clicked()) );
     connect( clearScript, SIGNAL(clicked()), this, SLOT(clearScriptBtn_clicked()) );

     //play all check box
     connect( playAllCkBox, SIGNAL(toggled(bool)), SLOT(playAllCheckBox_toggled(bool)) );

     // custom signal / slots
     connect ( &scriptController, SIGNAL(groupExecChanged(int)), this, SLOT(on_groupExecChanged(int)) );
     connect( &scriptController, SIGNAL(scriptExecChanged(int)), this, SLOT(on_scriptExecChanged(int)) );

}



bool ScriptGUIController::loadDefaultSelectionList( )
{
    bool ret = false;
    QStringList selectionList;

        // load scripts in "scripts/"
    scriptController.loadDefaultScripts();

    if( selectionListWidget != nullptr &&
            scriptController.getScriptSelectionList().size() > 0 )
    {
        selectionList << scriptController.getScriptSelectionList();
        //selectionList( scriptController.getScriptLoadedList() );

        selectionListWidget->addItems( selectionList );
    }
    else
    {
        qDebug( "ScriptGUIController not initialized" );
    }
    return ret;
}


bool ScriptGUIController::addToSelectionList( QUrl& url )
{
    bool ret = false;
    JSONScript script(url) ;
    QString errStr;

    if( script.parse( errStr ) )
    {

        selectionListWidget->addItem( script.getFileName() );
        scriptController.addScriptToLoadList( script  );

        ret = true;
    }
    else
    {
        qDebug( "Error %s ", errStr.toStdString().c_str() );
    }

    return ret;
}



bool  ScriptGUIController::addToExecList( QString name )
{
    bool ret = false;
    JSONScript script;

    script = scriptController.getScriptFromSelectionList( name );

    if ( script.getFileName() != "" )
    {
        execListWidget->addItem( script.getFileName() );
        scriptController.addScriptToExecList( script );

        // first item?
        if ( scriptController.getScriptExecList().size() == 1 )
        {
            //enable buttons
            setEnableControlButtons( PLAY_BTN_MASK |
                              PREV_BTN_MASK | NEXT_BTN_MASK );

        }

        qDebug() << "script added to execution list : " <<  script.getFileName();
        ret = true;
    }

    return ret;
}

bool ScriptGUIController::loadGroupMsgList( QString scriptName )
{
    bool ret = false;
    JSONScript script;
    QList< CanMsgGroup > msgGrpList;

    script = scriptController.getScriptFromExecList( scriptName );

   if( script.getFileName() != ""  )
   {
       msgGrpList = script.getGroupList();

       for( CanMsgGroup grp : msgGrpList  )
       {
           groupMsgListWidget->addItem( grp.description );
       }
   }
   else
   {
       qDebug( "script not found in exec list" );
   }

    return ret;
}

void ScriptGUIController::setScriptComInterface( ComInterface* com )
{
    scriptController.setScriptComInterface( com );
}

ScriptController*  ScriptGUIController::getScriptController()
{
    return &scriptController;
}

void ScriptGUIController::playButton_clicked()
{

    QListWidgetItem* currentItem;

    // first selected item on list
    currentItem = execListWidget->selectedItems().at( 0 );

    scriptController.stop();

    if( !scriptController.isExecAll()  )
    {
        scriptController.setCurrentScriptName( currentItem->data(Qt::DisplayRole ).toString() );
    }
    else
    {
        scriptController.resetScriptExecList();
    }



   setEnableControlButtons( PLAY_BTN_MASK | PAUSE_BTN_MASK |
                             STOP_BTN_MASK );


   if( MainGeneratorWindow::dbcDataList.size() == 0  )
   {
       QMessageBox::warning( nullptr, "DBC File not loaded", "No dbc file loaded only custom messages enabled" );
   }
       

   scriptController.play();

}

void ScriptGUIController::pauseButton_clicked()
{
    scriptController.pause();

    setEnableControlButtons( PLAY_BTN_MASK |
                             STOP_BTN_MASK | PREV_BTN_MASK | NEXT_BTN_MASK );
}

void ScriptGUIController::stopButton_clicked()
{
    scriptController.stop();

    setEnableControlButtons( PLAY_BTN_MASK |
                             PREV_BTN_MASK | NEXT_BTN_MASK );
}

void ScriptGUIController::nexButton_clicked()
{
    scriptController.next();

    setEnableControlButtons( PLAY_BTN_MASK |
                             STOP_BTN_MASK | PREV_BTN_MASK | NEXT_BTN_MASK );

}

void ScriptGUIController::prevButton_clicked()
{
    scriptController.prev();

    setEnableControlButtons( PLAY_BTN_MASK |
                             STOP_BTN_MASK | PREV_BTN_MASK | NEXT_BTN_MASK );
}

void ScriptGUIController::setEnableControlButtons( quint32 mask)
{
    quint32 testBit = 0x01;

    for( QPushButton* btn:  btnList )
    {
       if( mask & testBit )
       {
           btn->setEnabled( true );
       }
       else
       {
           btn->setEnabled( false );
       }

       testBit <<= 1;
    }
}



void ScriptGUIController::selListItem_doubleClicked( QListWidgetItem* wdgt )
{
    QString scriptName;

    scriptName = wdgt->data( Qt::DisplayRole ).toString();

    if( scriptName.size() > 0 )
    {
        addToExecList( scriptName );

        if( !scriptController.isExecAll()  )
        {
            scriptController.setCurrentScriptName( scriptName );
        }
    }
}

void ScriptGUIController::execListItem_doubleClicked( QListWidgetItem* wdgt )
{
    QString execItemName = wdgt->data(Qt::DisplayRole ).toString();

    scriptController.stop();

    scriptController.setExecAll( false );

    playAllCkBox->setChecked( false );

    scriptNameLabel->setText( execItemName );

    scriptController.setCurrentScriptName( execItemName );

   scriptController.play();

   setEnableControlButtons( PLAY_BTN_MASK | PAUSE_BTN_MASK |
                             STOP_BTN_MASK );

}

void ScriptGUIController::execListItem_currentChanged( QListWidgetItem *wdgt )
{
    QString execItemName;

    if( wdgt != nullptr )
    {
        execItemName = wdgt->data( Qt::DisplayRole ).toString();

        if( !scriptController.isFinished() )
        {
            groupMsgListWidget->clear();

            loadGroupMsgList(  execItemName );
            scriptNameLabel->setText( execItemName );
        }
    }
}


void ScriptGUIController::groupMsgItem_doubleClicked(QListWidgetItem *wdgt)
{
    // **** TODO: Open script editor here!! *******

}


void ScriptGUIController::addScriptBtn_clicked()
{
    QList< QListWidgetItem* > items;
    QString scriptName;

    items = selectionListWidget->selectedItems();


    for( QListWidgetItem* curItem : items )
    {
        scriptName = curItem->data( Qt::DisplayRole ).toString();


        if ( addToExecList( scriptName ) )
        {
            qDebug() << scriptName << " script added";
        }
    }

}

void ScriptGUIController::clearScriptBtn_clicked()
{
    groupMsgListWidget->clear();
    execListWidget->clear();

    stopButton_clicked();

    scriptController.clearScritpExecList();

    //disable all
    setEnableControlButtons( 0 );

}

void ScriptGUIController::playAllCheckBox_toggled( bool checked )
{           
    scriptController.setExecAll( checked );

    if( checked )
    {
        stopButton_clicked();

        // start from the begining
        scriptController.resetScriptExecList();
    }
}

void ScriptGUIController::on_scriptExecChanged( int scriptExecIdx )
{
    QListWidgetItem* curExecItem = nullptr;

    curExecItem = execListWidget->item( scriptExecIdx );

    if( curExecItem != nullptr )
    {
        scriptNameLabel->setText( curExecItem->data(Qt::DisplayRole).toString() );
    }

}

void ScriptGUIController::on_groupExecChanged( int groupExecIdx )
{
    groupMsgListWidget->setCurrentRow( groupExecIdx );
}

ScriptGUIController::~ScriptGUIController()
{

    if( scriptController.isRunning() )
    {
        scriptController.requestInterruption();
        scriptController.wait( );
    }
}

