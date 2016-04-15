#ifndef DBCDATA_H
#define DBCDATA_H

#include <QList>

#include "dbcModels/qdbcitem.h"
#include "dbcModels/qdbcmodel.h"

class DbcData
{
public:
    DbcData();
    DbcData( QString dbcName  );

    bool initFromModel( QDbcModel *model );
    bool initFromDbcStruct(dbc_t *database , QString fileName = "");

    QString name;

    class dbcSignal
    {
    public:
        QString name;
        quint64 min;
        quint64 max;
        quint64 value;
        quint32 bit_start;
        quint32 bit_len;
    };

    class dbcMessage
    {
    public:
        QString name;
        quint16 msgId;
        quint8 dlc;
        QList< dbcSignal > signalList;
    };

    QDbcModel* getDbcModel( void );

    QList< dbcMessage > getMessages( void );

    bool getMessage(QString name,  dbcMessage& msg );
    bool getMessage(quint16 Id, dbcMessage& msg );

    QList< dbcSignal > getSignals( quint16 msgId );
    QList< dbcSignal > getSignals( QString msgName );
    static bool getSignal( QString name, dbcMessage msg, dbcSignal& sig );
    static bool getSignal( QString name, QList< dbcSignal>& sigList, dbcSignal& Signal );

    bool init;

private:

    QList< dbcMessage > dbcMessageList;

    dbc_t database;
    QDbcModel* dbcModel;

    QString databaseFileName;
};

#endif // DBCDATA_H
