#include "dbcModels/dbcdata.h"

DbcData::DbcData()
{
}

DbcData::DbcData(QString dbcName)
{
    name = dbcName;
}


bool DbcData::initFromModel(QDbcModel* model )
{
    bool ret = false;
    QHash<int, QDbcItem*>  dbcMsgList;
    QHash<int, QDbcItem*>  dbcSigList;
    QHash<int, QDbcItem*>  dbcClassList;
    QDbcItem* dbcMsg;

    dbcClassList = model->getRootItem()->getChildren();

    for( QDbcItem* classFirstItem : dbcClassList )
    {
        dbcMsgList = classFirstItem->getChildren();

        // add every message
        for( QDbcItem* dbcItem : dbcMsgList )
        {
            dbcMessage msg;
            msg.name = dbcItem->name;
            msg.msgId = dbcItem->id.toInt();

            dbcMsg = dbcMsgList.value( msg.msgId );

            if( dbcMsg != nullptr )
            {
                // get signals
                dbcSigList = dbcMsg->getChildren();

                // add every dbc signal, min & max values
                for( QDbcItem* dbcItem : dbcSigList )
                {
                    dbcSignal sig;

                    sig.name = dbcItem->name;
                    sig.min = dbcItem->min.toInt();
                    sig.max = dbcItem->max.toInt();
                    sig.bit_start = dbcItem->bit_start.toInt();
                    sig.bit_len = dbcItem->bit_len.toInt();
                    sig.value = 0;

                    msg.signalList.append( sig );
                }

                // add message
                dbcMessageList.append( msg );
            }
        }
    }

    init = true;
    dbcModel = model;

    ret = true;

    return ret;
}

bool DbcData::initFromDbcStruct( dbc_t *database, QString fileName )
{
    bool ret = false;
    QDbcModel* model;

    //TODO: initialize directly from dbc structure
    //       and skip QDbcModel step
    model = new QDbcModel( database );

    if( model != nullptr && initFromModel( model ) )
    {
        name = fileName;
        ret = true;
    }

    return ret;
}

QDbcModel* DbcData::getDbcModel( void )
{
   return dbcModel;
}


QList< DbcData::dbcMessage > DbcData::getMessages( void )
{
    QList< DbcData::dbcMessage > messageList;

    if( dbcMessageList.size() >  0 )
    {
        messageList = dbcMessageList;
    }
    return messageList;
}

bool DbcData::getMessage(QString name, DbcData::dbcMessage& msg )
{
    bool ret = false;

    for( dbcMessage dbcMsg : dbcMessageList )
    {
        if( msg.name == name )
        {
            msg = dbcMsg;
            ret = true;
        }
    }
    return ret;
}


bool DbcData::getMessage( quint16 Id, DbcData::dbcMessage& msg )
{
    bool ret = false;

    for( dbcMessage dbcMsg : dbcMessageList )
    {
        if( msg.msgId == Id )
        {
            msg = dbcMsg;
            ret = true;
        }
    }
    return ret;
}


QList< DbcData::dbcSignal > DbcData::getSignals( quint16 msgId )
{
    QList< DbcData::dbcSignal > sigList;

    for( DbcData::dbcMessage msg : dbcMessageList )
    {
        if( msg.msgId == msgId )
        {
            sigList = msg.signalList;
            break;
        }
    }
    return sigList;
}

QList< DbcData::dbcSignal > DbcData::getSignals(QString msgName )
{
    QList< DbcData::dbcSignal > sigList;

    for( DbcData::dbcMessage msg : dbcMessageList )
    {
        if( msg.name == msgName )
        {
            sigList = msg.signalList;
            break;
        }
    }

    return sigList;
}

bool DbcData::getSignal( QString name, dbcMessage msg, dbcSignal& sig )
{
    bool ret = false;
    QList< dbcSignal > sigList =  msg.signalList;

    for( dbcSignal dbcSig : sigList )
    {
        if( dbcSig.name == name )
        {
            sig = dbcSig;
            ret = true;
        }
    }
    return ret;
}


bool DbcData::getSignal( QString name, QList< dbcSignal>& sigList, dbcSignal& Signal )
{
    bool ret = false;

    for( dbcSignal  sig : sigList  )
    {
        if( sig.name == name )
        {
            Signal = sig;
            ret = true;
        }
    }
    return ret;
}
