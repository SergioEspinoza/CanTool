#include "com/messageworker.h"

#include "canMsg/canmsg.h"


MessageWorker::MessageWorker( QObject *parent ) : qsem( 1 ), QThread( parent )
{
}

bool MessageWorker::msgToPdu( CanMsg& msg, QByteArray &pdu )
{
    QByteArray ba;
    char intMsgIdx;
    bool ret = false;

    intMsgIdx = (char) getInternalMsgIdx( msg.getMsgId() );

    if( intMsgIdx != -1 )
    {
        ba.resize( CAN_MAX_LENGHT_PACKET );
        // build header data
        ba.insert( START_TX_IDX,  ( char ) ( TX_START_VALUE ) );

        ba.insert( MSG_IDX_IDX,  intMsgIdx );

        ba.insert( MSG_TIME_IDX, ( quint8 ) msg.getPeriod() );

        //TODO: check full lenght to be ok
        ba.insert( MSG_FULL_LEN_IDX, ( quint8 ) CAN_MAX_LENGHT_PACKET );

        ba.insert( CAN_MSG_ID_H_IDX, ( quint8 ) ( ( msg.getMsgId() ) >> 8 ) & ( 0x00FF ) );

        ba.insert( CAN_MSG_ID_L_IDX, ( quint8 ) ( msg.getMsgId() ) & ( 0x00FF ) );

        ba.insert( CAN_MSG_DLC_IDX, msg.getDLC() );

        for( int idx = 0 ; idx < CAN_MAX_DATA_LENGTH ; idx++ )
        {
            ba.insert( idx + CAN_MSG_DATA_0_IDX,  msg.toByteArray()->at( idx )  );
        }
        ba.trimmed();

        pdu = ba;

        ret = true;
    }

    return ret;
}


void MessageWorker::enqueuegGroup(CanMsgGroup& group, ComInterface *com )
{

    for( CanMsg msg : group.messages )
    {
        enqueueMsg( msg, com );
    }

    for( CanMsg customMsg : group.custom_messages )
    {
        enqueueMsg( customMsg, com );
    }
}


void MessageWorker::enqueueMsg(CanMsg& msg, ComInterface *com )
{

    if( msg.validate( true ) )
    {
        msg.dbcSync();

        msg.setComInterface( com );

        qsem.acquire();

        queue.enqueue( msg );

        qsem.release();
    }
    else
    {
        qDebug( "Message validation failed: %s ", msg.getName().toStdString().c_str() );
    }

}

int MessageWorker::getInternalMsgIdx( quint16 idx )
{
    int retIdx = -1;

        if( msgIdxTable.keys().contains( idx ) )
        {
            // internal index currently assigned
            retIdx = msgIdxTable.value( idx );
        }
        else if ( curMsgIdx <= MAX_MSG_IDX )
        {
            // assign new internal index
            msgIdxTable.insert( idx, curMsgIdx++ );
            retIdx = curMsgIdx;
        }

    return retIdx;
}


MessageWorker::~MessageWorker()
{
}

