#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "scriptMainGUI/jsonscript.h"


JSONScript::JSONScript( ) : url("")
{
}

JSONScript::JSONScript(QUrl path )
{
   url = path;
}

void JSONScript::setUrl(QUrl path )
{
    url = path;
}

QString JSONScript::getFileName( )
{    
    return url.fileName();
}

bool JSONScript::parse( QString& errorString )
{
    QJsonParseError jsonError;
    QJsonDocument jsonDocument;
    QFile jsonFile;
    bool ret = false;

    if( url.path() != "" )
    {
        jsonFile.setFileName( url.path() );
        jsonFile.open( QFile::ReadOnly );

        if( !jsonFile.exists() )
        {
            QString path = url.path();

            qDebug() << "File url malformed!!: " << path;
        }

        jsonDocument = QJsonDocument::fromJson( jsonFile.readAll(), &jsonError );

        switch( jsonError.error )
        {

        case  QJsonParseError::NoError:
        {
            jrootObject = jsonDocument.object();
            QJsonObject canScript;
            QJsonArray  msgGroupArray;

            if ( jrootObject.contains( "YazCANScript" ) )
            {
                canScript = jrootObject[ "YazCANScript" ].toObject();

                if( canScript.contains( "groups" ) )
                {

                    msgGroupArray = canScript[ "groups" ].toArray();

                    for( QJsonValue jval : msgGroupArray )
                    {
                        CanMsgGroup msgGroup;

                        if( !msgGroup.readJson( jval.toObject() ) )
                        {
                            qDebug( "error reading can msg group " );
                        }

                        msgGroupList.append( msgGroup );
                    }
                }
                else
                {
                    qDebug( "Missing group definitions" );
                }

            }
            else
            {
                qDebug( ) << "Invalid document : " << url.path() ;
            }
            ret = true;
        }
            break;

        case QJsonParseError::UnterminatedObject:
        case QJsonParseError::MissingNameSeparator:
        case QJsonParseError::UnterminatedArray:
        case QJsonParseError::MissingValueSeparator:
        case QJsonParseError::IllegalValue:
        case QJsonParseError::TerminationByNumber:
        case QJsonParseError::IllegalNumber:
        case QJsonParseError::IllegalUTF8String:
        case QJsonParseError::UnterminatedString:
        case QJsonParseError::MissingObject:
        case QJsonParseError::DeepNesting:
        case QJsonParseError::DocumentTooLarge:
        case QJsonParseError::IllegalEscapeSequence:

            errorString.append( jsonError.errorString() );
            ret = false;
            break;

        default:
            break;
        }

        jsonFile.close();

    }
    return ret;
}


bool JSONScript::execute( MessageWorker &worker, ComInterface* com )
{
    bool ret = false;

   for( CanMsgGroup curGroup : msgGroupList )
   {
      worker.enqueuegGroup( curGroup, com );

      if( !worker.isRunning() )
      {
          worker.start();
      }

   }

   return ret;
}

bool JSONScript::save( QString& errorString )
{
    bool ret = true;
    QFile scriptFile;
    QJsonObject rootObject;
    QJsonDocument jsonDoc;
    QJsonArray jgrouparray;

    if( !url.isEmpty() && url.isValid() )
   {
        scriptFile.setFileName( url.toLocalFile() );

        if( scriptFile.open( QFile::ReadWrite ) )
        {

            for( CanMsgGroup curGroup : msgGroupList )
            {
                QJsonObject jgroupObject;
                curGroup.writeJson( jgroupObject );
                jgrouparray.append( jgroupObject );
            }

            rootObject[ "groups" ] = jgrouparray;

            jsonDoc.setObject( rootObject );
            scriptFile.write( jsonDoc.toJson() );

            scriptFile.flush();
            scriptFile.close();

            qDebug( ) << "New Json script file wirten : " + scriptFile.fileName();



            ret = true;
        }
        else
        {
            qDebug( "Unable to open script file" );
            errorString = "Unable to open script file";
            ret = false;
        }
    }
    else
    {
        qDebug( "script url not set or not valid" );
        errorString = "script url not set or not valid";
        ret = false;
    }

    return ret;
}

QList< CanMsgGroup > JSONScript::getGroupList( void )
{
   return msgGroupList;
}

void JSONScript::addGroup(CanMsgGroup grp)
{
    msgGroupList.append( grp );
}
