#include <QJsonArray>
#include <QJsonValue>

#include "canMsg/canmsggroup.h"


CanMsgGroup::CanMsgGroup()
{
}

bool  CanMsgGroup::readJson( QJsonObject jobject )
{
    QJsonArray msgArray;
    QJsonArray customMsgArray;
    bool ret = false;

    if( jobject.contains( "name" ) &&
            jobject.contains( "description" ) &&
            jobject.contains( "delay" ) )
    {
        name = jobject[ "name" ].toString();
        description = jobject[ "description" ].toString();
        delay = jobject[ "delay" ].toInt();


        if( jobject.contains( "messages" ) ||
                jobject.contains( "custom_messages" ) )
        {

            msgArray = jobject[ "messages" ].toArray();
            customMsgArray = jobject[ "custom_messages" ].toArray();

            messages.clear();
            custom_messages.clear();

            // add messages
            for( QJsonValue jval : msgArray )
            {
                CanMsg curMsg;

                if ( curMsg.readJson( jval.toObject(), false ) )
                {
                    messages.append( curMsg );
                }
                else
                {
                    qDebug( "error reading can msg" );
                }
            }

            // add custom messages (if not present just skip)
            for( QJsonValue jval : customMsgArray  )
            {
                CanMsg customMsg;

                if ( customMsg.readJson( jval.toObject(), true ) )
                {
                    custom_messages.append( customMsg );
                }
                else
                {
                    qDebug( "error reading custom can msg" );
                }
            }

            ret = true;
        }
        else
        {
            qDebug( "missing message definitions from group" );
            ret = false;
        }
    }
    else
    {
        qDebug( " JSON Incomplete message group entity" );
        ret= false;
    }

    return ret;
}

bool  CanMsgGroup::writeJson( QJsonObject& jobject )
{
    bool ret = false;

    return ret;
}


void CanMsgGroup::addMsg( CanMsg& msg )
{
    messages.append( msg );
}


QList< CanMsg > CanMsgGroup::getMessages(bool custom )
{
    QList< CanMsg > msgs;
    if( !custom )
    {
        msgs = messages;
    }
    else
    {
       msgs = custom_messages;
    }
    return msgs;
}


void CanMsgGroup::setMessages( QList< CanMsg > &msgs, bool custom )
{
    if( !custom )
    {
        messages = msgs;
    }
    else
    {
       custom_messages = msgs;
    }

}
