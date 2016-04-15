#ifndef CANMSGGROUP_H
#define CANMSGGROUP_H

#include <QJsonObject>


#include "canMsg/canmsg.h"

class CanMsgGroup
{
public:
    CanMsgGroup();

    bool readJson( QJsonObject jobject );
    bool writeJson( QJsonObject& jobject );    

    void addMsg( CanMsg& msg );

    QList< CanMsg > getMessages( bool custom = false );
    void setMessages( QList< CanMsg >& msgs, bool custom = false );

    QString name;
    QString description;
    qint32 delay;

    QList< CanMsg > messages;
    QList< CanMsg > custom_messages;
};

#endif // CANMESSAGEGROUP_H
