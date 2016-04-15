#include <QDir>

#include "scriptMainGUI/scriptcontroller.h"

ScriptController::ScriptController( QObject* parent  ) :
    worker( nullptr ), scriptComInterface( nullptr ), curScriptName( "" ),
    curScriptIdx( 0 ), curGroupIdx( 0 ), execAllScripts( false ),
    freeRun( false ),  cmdSema(1),
    stepsToStop( 0 ), QThread( parent )
{
}

ScriptController::~ScriptController()
{
    if( worker->isRunning() )
    {
        worker->requestInterruption();
        worker->wait( );
    }
}


void ScriptController::addScript( JSONScript& script )
{
   if( script.getFileName() != "" )
   {
       scriptExecList.insert( script.getFileName(), script );
   }
}

JSONScript ScriptController::getScript( QString filename )
{
    return scriptExecList.value( filename );
}

void ScriptController::deleteScript( QString filename )
{
    scriptExecList.remove( filename );
}

void ScriptController::setScriptComInterface(ComInterface *com )
{
    scriptComInterface = com;
}

void  ScriptController::execCmd( int cmd  )
{

    switch( cmd )
    {
    case PLAY_CMD:
        // begin execute all scripts in list
        // or continue from previous

        if( curScriptName == "" )
        {
            curScriptIdx = 0;
            curScriptName = scriptExecList.keys().at( curScriptIdx );
            curGroupIdx = 0;
        }

        freeRun = true;

        break;
    case PAUSE_CMD:

        freeRun = false;
        stepsToStop = 0;

        break;
    case STOP_CMD:

        // start agai current script
        curGroupIdx = 0;

        freeRun = false;
        stepsToStop = 0;

        break;

    case PREV_CMD:
    {

        //TODO: implement previous
        // callback

        //still groups left in current
        // script
        if ( curGroupIdx > 0 )
        {
            curGroupIdx--;
        }
        else if( ( curScriptIdx > 0 ) && ( execAllScripts == true ) )
        {
            curScriptIdx--;
            curScriptName = scriptExecList.keys().at( curScriptIdx );
        }

        freeRun = false;
        stepsToStop = 1;

        break;
    }
    case NEXT_CMD:
        freeRun = false;
        stepsToStop = 1;
        break;
    }
}


void  ScriptController::scriptIterate()
{
    CanMsgGroup curGrp;
    JSONScript curScr;

    if( freeRun || ( stepsToStop > 0 ) )
    {
        // get active script
        curScr = scriptExecList.value( curScriptName );

        //get active group inside acive script
        curGrp = curScr.getGroupList().at( curGroupIdx );

        // transmit next CAN message group

        worker->enqueuegGroup( curGrp, scriptComInterface );

        curGroupIdx++;

        emit ( SIGNAL( groupExecChanged( curGroupIdx ) ) );

       // done with all groups in current script?
       if( curGroupIdx >= curScr.getGroupList().size() )
       {

            curGroupIdx = 0;
            curScriptIdx++;

            // still more sripts left?
            if(  execAllScripts &&
              ( curScriptIdx < scriptExecList.keys().size() ) )
            {
                // load next script
                curScriptName = scriptExecList.keys().at( curScriptIdx );
                emit ( SIGNAL( groupExecChanged( curGroupIdx ) ) );
                emit ( SIGNAL( scriptExecChanged( curScriptIdx ) ) );

            }
            else
            {
                //finished execution
                curScriptName = "";
                curScriptIdx = 0;
                curGroupIdx = 0;

                freeRun = false;
                stepsToStop = 0;
            }
       }

       QThread::msleep( curGrp.delay );

       if( stepsToStop > 0 )
       {
           stepsToStop--;
       }
    }
}

int ScriptController::loadDefaultScripts( )
{
    int ret = 0;

    QDir scriptsDir( "scripts/" );
    QStringList filters;
    QStringList  scriptFileNameList;

    filters << "*.json" << "*.JSON";

    if( scriptsDir.exists( ) )
    {
        scriptsDir.setNameFilters( filters );

        // get all file names matching filter
        scriptFileNameList = scriptsDir.entryList( );

        for( QString scriptName : scriptFileNameList )
        {
            JSONScript script;
            QString errorStr;
            QUrl url;

            url.setPath( scriptsDir.path() + "/" + scriptName );

            script.setUrl( url );

            if( script.parse( errorStr ) )
            {
                scriptSelectionList.insert( url.fileName() , script );
                qDebug() << "Script Loaded " << script.getFileName();
            }
            else
            {
                qDebug() << "Script failed to load: " << script.getFileName();
            }
        }
    }
    else
    {
        qDebug( "scripts dir not present" );
    }

    return ret;
}

void ScriptController::addScriptToLoadList( JSONScript script )
{
    scriptSelectionList.insert( script.getFileName(), script );

}

void  ScriptController::addScriptToExecList( JSONScript script )
{
    scriptExecList.insert( script.getFileName(), script );
}

void ScriptController::clearScritpExecList( )
{
    scriptExecList.clear();
}

void  ScriptController::resetScriptExecList( )
{
   curScriptName = scriptExecList.keys().at( 0 );

    curScriptIdx = 0;
    curGroupIdx = 0;
}

void ScriptController::setExecAll( bool execAll )
{
   execAllScripts = execAll;
}

bool ScriptController::isExecAll( )
{
    return execAllScripts;
}

bool ScriptController::isFreeRun( )
{
    return freeRun;
}


void ScriptController::setCurrentScriptName(QString name )
{
    curScriptName = name;
}

QString ScriptController::getCurrentScriptName()
{
    return curScriptName;
}

QList<QString> ScriptController::getScriptSelectionList( )
{
    return scriptSelectionList.keys();
}

void ScriptController::getLoadedScripts( QMap< QString, JSONScript >& scripts  )
{
    scripts = scriptSelectionList;
}

void ScriptController::loadScripts( QMap< QString, JSONScript >& scripts )
{
    scriptSelectionList = scripts;
}

JSONScript ScriptController::getScriptFromSelectionList( QString name )
{
    JSONScript script;

    if( scriptSelectionList.keys().contains( name ) )
    {
        script = scriptSelectionList.value( name );
    }

    return script;
}

QList<QString> ScriptController::getScriptExecList(  )
{
    return scriptExecList.keys();
}

JSONScript ScriptController::getScriptFromExecList( QString name )
{
    JSONScript script;

    if( scriptSelectionList.keys().contains( name ) )
    {
        script = scriptSelectionList.value( name );
    }

    return script;
}

// Script control
void ScriptController::play()
{
    cmdSema.acquire();
    cmdQueue.enqueue( PLAY_CMD );
    cmdSema.release();
}

void ScriptController::pause()
{
    cmdSema.acquire();
    cmdQueue.enqueue( PAUSE_CMD );
    cmdSema.release();
}

void ScriptController::stop()
{
    cmdSema.acquire();
    cmdQueue.enqueue( STOP_CMD );
    cmdSema.release();
}

void ScriptController::prev()
{
    cmdSema.acquire();
    cmdQueue.enqueue( PREV_CMD );
    cmdSema.release();
}

void ScriptController::next()
{
    cmdSema.acquire();
    cmdQueue.enqueue( NEXT_CMD );
    cmdSema.release();
}

