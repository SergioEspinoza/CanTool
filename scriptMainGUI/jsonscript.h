#ifndef JSONSCRIPTPARSER_H
#define JSONSCRIPTPARSER_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

#include "canMsg/canmsggroup.h"
#include "com/cominterface.h"
#include "com/messageworker.h"

class JSONScript
{
public:

    JSONScript();
    JSONScript( QUrl path );

    void setUrl( QUrl path );
    QString getFileName( );

    bool parse( QString& errorString );
    bool execute( MessageWorker &worker, ComInterface* com );
    bool save( QString& errorString );

    QList<CanMsgGroup> getGroupList( void );
    void addGroup( CanMsgGroup grp);

private:
    QJsonObject jrootObject;
    QList< CanMsgGroup > msgGroupList;
    QUrl    url;
};

#endif // JSONSCRIPTPARSER_H
