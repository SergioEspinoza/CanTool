#include "maingeneratorwindow.h"
#include "canMsg/canmsg.h"


CanMsg::CanMsg() : valid( false ) , period( false ), dbcName( "" ), dbcData( nullptr )
{
}

int  CanMsg::setupCustomMsg(quint64 Id, qint8 DLC, QByteArray& msgBytes )
{
   custom = true;

   msgId = Id;
   dlc = DLC;
   dataBytes = msgBytes;

   return true;
}

int CanMsg::setupMsg( QString msgName, QList<DbcData::dbcSignal> &msgSignalList, DbcData* dbcDataParam )
{
   custom = false;

   name = msgName;
   signalList = msgSignalList;
   dbcData = dbcDataParam;

   return true;
}

bool CanMsg::validate( bool validateSigValues )
{
    QList< DbcData::dbcMessage > messageList;
    QList< DbcData::dbcSignal > sigList;

    bool ret = false;

    if( dbcData != nullptr ||
        dbcName.size() > 0 )
    {

        // set model if missing
      if( dbcData == nullptr )
      {
          for ( DbcData* curData : MainGeneratorWindow::dbcDataList )
          {
              if( curData->name == dbcName )
              {
                  dbcData = curData;
              }
          }
      }

      if( dbcData != nullptr )
      {

       messageList = dbcData->getMessages();
        if( messageList.size() > 0 )
        {
            // check message name / id
            for( DbcData::dbcMessage dbcMsg : messageList )
            {
                // validate name and dlc
                if( !custom && ( dbcMsg.name == name ) &&
                    ( dbcMsg.msgId == msgId ) &&
                    ( dbcMsg.dlc == dlc ) )
                {
                    sigList = dbcData->getSignals( name );
                    ret = true;
                }
                // case custom msg validate id only
                else if( custom && ( dbcMsg.msgId == msgId ) )
                {
                   sigList = dbcData->getSignals( msgId );
                   ret = true;
                }
            }

            // check signals
            if( ret == true && ( sigList.size() > 0 )  )
            {

                for( DbcData::dbcSignal mySig : signalList )
                {
                    for( DbcData::dbcSignal dbcSig : sigList )
                    {
                        if( mySig.name == dbcSig.name )
                        {
                            if( validateSigValues )
                            {
                                if ( mySig.value < dbcSig.max &&
                                    mySig.value > dbcSig.min  )
                                {
                                    qDebug() << "signal not in range: " << mySig.name;
                                    qDebug() << "max : " << mySig.max;
                                    qDebug() << "min : " << mySig.min;
                                    ret = false;
                                }
                                else
                                {
                                    ret = true;
                                }
                            }
                            else
                            {
                                ret = true;
                                break;
                            }
                        }
                    }

                    if( !ret )
                    {
                        qDebug() << "Signal not found " << mySig.name;
                        break;
                    }
                }
            }
        }
      }
      else
      {
          qDebug() << " DBC model with name: " << dbcName << " not found / not loaded";
          ret = false;
      }
    }
    else
    {
        qDebug( "No dbc model set " );
        ret = false;
    }

    return ret;
}


bool CanMsg::dbcSync( void )
{
    bool ret = false;
    QList< DbcData::dbcMessage > msgs;

    if( dbcData != nullptr )
    {
        msgs = dbcData->getMessages();

        if( !custom && ( msgs.size() > 0 ) )
        {
            for( DbcData::dbcMessage msg : msgs )
            {
                if( msg.name == name )
                {
                    msgId = msg.msgId;
                    msgId = msg.dlc;
                    ret = true;
                }
            }
        }
    }
    else
    {
        qDebug( "DBC model not set" );
    }
    return ret;
}


bool CanMsg::setSignalValue( QString name, quint64 value )
{
    bool ret = 0;

    for( DbcData::dbcSignal sig : signalList )
    {
        if( sig.name == name )
        {
            sig.value = value;
            ret = true;
        }
    }

    if( !ret )
    {
        qDebug( "Signal not found" );
    }

    return ret;
}

QByteArray* CanMsg::toByteArray( void )
{
    QByteArray* ba = nullptr;
    DbcData::dbcMessage dbcMsg;
    QList<DbcData::dbcSignal> dbcSignalList;
    DbcData::dbcSignal sig;
    //QDbcItem* canMsg;
    int curBitEnd = 0;
    int curByte = 0;
    char builtBytes[ 8 ] = { 0 };
    qint8 curBitMaskPosition;
    qint8 curBitMask;

    // fixed size byte array
    ba = new QByteArray( );
    ba->resize( dlc + ID_SZ + DLC_SZ );

    //TODO: check if needing to decompose
    ba->append( msgId );
    ba->append( dlc );

    if( custom == true )
    {
        ba->append( dataBytes );
    }
    else
    {
        if( valid && dbcData->getMessage( msgId, dbcMsg ) )
        {
            dbcSignalList = dbcMsg.signalList;

            // build 8 data bytes, bit per bit
            for( uint8 i = 0 ; i < 64 ; i++ )
            {
                curByte = floor( i / 8 );
                curBitMaskPosition = i % 8;
                curBitMask = ( 1 << curBitMaskPosition );

                for( DbcData::dbcSignal signal : dbcSignalList )
                {
                    curBitEnd = signal.bit_start + signal.bit_len;

                    if( ( i > signal.bit_start ) && ( i < curBitEnd ) )
                    {
                        if ( DbcData::getSignal( signal.name, signalList, sig ) )
                        {
                            builtBytes[ curByte ] |= (  sig.value & curBitMask );
                        }
                        else
                        {
                            qDebug( "dbc model mismatch" );
                        }
                    }
                }
            }
        }
        else
        {
            delete ba;
            ba = nullptr;
            qDebug( "Message has no valid dbc model" );
        }
    }

    return ba;
}


void CanMsg::setDbcModel( DbcData* model )
{
    dbcName = model->name;
    dbcData = model;
}

bool CanMsg::readJson( QJsonObject jobject , bool custom )
{
    QJsonArray jarray;
    bool ret = true;

    // case custom msg
    if( !custom &&
        jobject.contains( "name" ) &&
        jobject.contains( "period" ) &&
        jobject.contains( "signals" ) &&
        jobject.contains( "dbc_name" ) )
    {
        dbcName = jobject[ "dbc_name" ].toString();
        name = jobject[ "name" ].toString();
        period = jobject[ "period" ].toInt();

        // following 2 cases are not mandatory
        if( jobject.contains( "id" ) )
        {
            msgId = jobject[ "id" ].toInt();
        }

        if( jobject.contains( "dlc" ) )
        {
            dlc = jobject[ "dlc" ].toInt();
        }

        jarray = jobject[ "signals" ].toArray();

        for( QJsonValue arrEl : jarray )
        {
            if( arrEl.toObject().contains( "name" ) &&
                    arrEl.toObject().contains( "value" ) )
            {
                DbcData::dbcSignal sig;

                sig.name = arrEl.toObject().value( "name" ).toString();
                sig.value =  arrEl.toObject().value( "value" ).toInt();

                signalList.append( sig );
            }
            else
            {
                qDebug( "JSON document signal incomplete" );
                ret = false;
            }
        }

    }
    else if( custom &&
            jobject.contains( "id" ) &&
            jobject.contains( "dlc" ) &&
            jobject.contains( "bytes" ) &&
            jobject.contains( "period" ) )
    {
       msgId = jobject[ "id" ].toInt();
       dlc = jobject[ "dlc" ].toInt();
       period = jobject[ "period" ].toInt();

       jarray = jobject[ "bytes" ].toArray();

       for( QJsonValue arrEl : jarray )
       {
           int dbyte = arrEl.toInt();

           if( dbyte <= 255 )
           {
                dataBytes.append( dbyte );
           }
       }
    }
    else
    {

        qDebug( "Incomplete json message entity" );
        ret = false;
    }
        return ret;
}


quint8 CanMsg::getPeriod()
{
    return period;
}


quint8 CanMsg::getDLC()
{
    return dlc;
}

bool CanMsg::getCustom()
{
    return custom;
}

quint16 CanMsg::getMsgId()
{
    return msgId;
}

QString CanMsg::getName()
{
    return name;
}


bool CanMsg::getServiced( )
{
    return serviced;
}

void CanMsg::setServiced( bool service )
{
    serviced = service;
}

ComInterface* CanMsg::getComInterface( )
{
    return comInterface;
}

void CanMsg::setComInterface( ComInterface* com )
{
    comInterface = com;
}


CanMsg::~CanMsg()
{
}
