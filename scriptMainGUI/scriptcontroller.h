#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H

#include <QObject>
#include <QThread>

#include "com/cominterface.h"
#include "scriptMainGUI/jsonscript.h"
#include "com/messageworker.h"

class ScriptController : public  QThread
{
    Q_OBJECT
public:
    explicit ScriptController(QObject *parent = 0);
    virtual ~ScriptController();


    void run( ) Q_DECL_OVERRIDE
    {
        int cmd;

        if( worker == nullptr )
        {
            worker = new MessageWorker();
            worker->start();
        }

        while( !isInterruptionRequested() )
        {

            if( !cmdQueue.isEmpty() )
            {
                cmdSema.acquire();
                cmd = cmdQueue.dequeue();
                cmdSema.release();

                execCmd( cmd );
            }

            scriptIterate();
        }
    }

    enum
    {
        PLAY_CMD,
        PAUSE_CMD,
        STOP_CMD,
        PREV_CMD,
        NEXT_CMD
    }CMD_type;

    void addScript( JSONScript& script );
    JSONScript getScript( QString filename );
    void deleteScript( QString filename );

    void setScriptComInterface( ComInterface* com );

    int loadDefaultScripts( );

    void addScriptToLoadList( JSONScript script );
    void addScriptToExecList( JSONScript script );

    void resetScriptExecList( );
    void clearScritpExecList( );

    void setExecAll( bool execAll );
    bool isExecAll( );

    bool isFreeRun( );

    void setCurrentScriptName( QString name);
    QString getCurrentScriptName();


    QList<QString> getScriptSelectionList( );
    JSONScript getScriptFromSelectionList( QString name );

    void getLoadedScripts( QMap< QString, JSONScript >& scripts  );
    void loadScripts( QMap< QString, JSONScript >& scripts );

    QList<QString> getScriptExecList(  );
    JSONScript getScriptFromExecList( QString name );

    // Script control
    void play();
    void pause();
    void stop();
    void prev();
    void next();

private slots:

signals:
    void scriptExecChanged( int scriptExecIdx );
    void groupExecChanged( int groupExecIdx );

private:

    void  scriptIterate();
    void  execCmd( int cmd  );


    MessageWorker* worker;

    ComInterface* scriptComInterface;

    QMap< QString, JSONScript > scriptExecList;

    QMap< QString, JSONScript > scriptSelectionList;

    QSemaphore cmdSema;
    QQueue< int > cmdQueue;

    //active script name
    QString curScriptName;
    //active script index
    int curScriptIdx;
    //active group inside active script
    int  curGroupIdx;

    int stepsToStop;
    int freeRun;

    bool execAllScripts;
};

#endif // SCRIPTCONTROLLER_H
