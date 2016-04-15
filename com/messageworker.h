#ifndef MESSAGEWORKER_H
#define MESSAGEWORKER_H

#include <QObject>
#include <QThread>
#include <QAtomicInt>
#include <QQueue>
#include <QSemaphore>

#include "canMsg/canmsg.h"
#include "cominterface.h"
#include "canMsg/canmsggroup.h"

class MessageWorker : public QThread
{
    Q_OBJECT
public:
    explicit MessageWorker(QObject *parent = 0);
    virtual ~MessageWorker();

    void run() Q_DECL_OVERRIDE
    {
        while( !isInterruptionRequested() )
        {

            qsem.acquire();
            while( !queue.isEmpty() )
            {
                CanMsg msg;
                QByteArray ba;

                msg = queue.dequeue();
                msgToPdu( msg, ba );

               if ( msg.getComInterface() != nullptr  )
               {
                    msg.getComInterface()->send( ba );

                   qDebug( "Cannot send serial message" );
               }
            }
            qsem.release();

        }

    }



    void enqueueMsg(CanMsg& msg , ComInterface* com);
    void enqueuegGroup(CanMsgGroup& group , ComInterface* com);


private:

    bool msgToPdu( CanMsg& msg, QByteArray &pdu );
    int getInternalMsgIdx( quint16 msgId );



    QSemaphore qsem;
    QQueue< CanMsg > queue;

    static const int     MAX_MSG_TABLE_SZ = 255;

    static const int      START_TX_IDX = 0;
    static const int      MSG_IDX_IDX = 1;
    static const int      MSG_TIME_IDX = 2;
    static const int      MSG_FULL_LEN_IDX = 3;
    static const int      CAN_MSG_ID_H_IDX = 4;
    static const int      CAN_MSG_ID_L_IDX = 5;
    static const int      CAN_MSG_DLC_IDX = 6;
    static const int      CAN_MSG_DATA_0_IDX = 7;
    static const int      CAN_MSG_DATA_1_IDX = 8;
    static const int      CAN_MSG_DATA_2_IDX = 9;
    static const int      CAN_MSG_DATA_3_IDX = 10;
    static const int      CAN_MSG_DATA_4_IDX = 11;
    static const int      CAN_MSG_DATA_5_IDX = 12;
    static const int      CAN_MSG_DATA_6_IDX = 13;
    static const int      CAN_MSG_DATA_7_IDX = 14;

    static const int     CAN_MAX_LENGHT_PACKET = 15;
    static const int     CAN_MAX_DATA_LENGTH = 8;

    static const int TX_START_VALUE = 0x02;
    static const int NORMAL_MSG_CAN = 0x03;

    static const int MAX_MSG_IDX = 255;
    // internal msg index, used to track how many slots
    // left for sending new messages
    int curMsgIdx;
    // msgId / Idx
    QHash< quint16, int > msgIdxTable;



};

#endif // MESSAGEWORKER_H
