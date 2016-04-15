/**
 * \class CanMsg
 *
 * \brief
 *          Represents can message to be sent.
 * \note
 *          For message to be complete it should be associated with a
 *          given dbc file and validated, otherwise information sent
 *          wouldn't be correctly interpreted
 *
 * \author  $Author: Sergio Espinoza (10012599) $
 *
 * \version $Revision: 0.1 initial implementation $
 *
 * \date $Date: 12/15/2014 $
 *
 */

#ifndef CANMSG_H
#define CANMSG_H

#include <QByteArray>
#include <QPair>
#include <QHash>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonValueRef>
#include <QJsonArray>

#include "dbcModels/qdbcmodel.h"
#include "dbcModels/qdbcitem.h"
#include "dbcModels/dbcdata.h"
#include "com/cominterface.h"

class CanMsg
{
public:
    CanMsg();
    ~CanMsg();

     /** \brief
     *     Initialize method for custom message
     *  \note
     *     custom message only
     *
     *  \param Id
     *     message id, should be valid within dbc model
     *  \param DLC
     *      number of data bytes
     *  \param msgBytes
     *      data bytes to be sent
     *
     *  \return  true: valid false: not valid
     */
    int setupCustomMsg( quint64 Id, qint8 DLC, QByteArray& msgBytes );

    /** \brief
     *     Initialize method for 'normal' message
     *  \note
     *     normal messages with strings attributes for
     *     name and signals
     *
     *  \param msgName
     *     message string name
     *
     *  \param msgSignalList
     *      Hash map with paired name / value for each one of the
     *      signals for this message (no more than DLC bytes )
     *
     *  \return  true: valid , false: not valid
     */

    int setupMsg(QString msgName, QList< DbcData::dbcSignal > &msgSignalList , DbcData *dbcDataParam);

     /** \brief
      *     check if message parameters are valid
      *     given for current dbc model (message name, id,
      *     signal names and ranges)
     *
     *  \return  true: valid false: not valid
     */
    bool validate(bool validateSigValues = false );

    /** \brief
     *    add missing information from current dbc
     *    model to message parameters.
     *  \note
     *    in case custom message this method does nothing,
     *    attributes to be updated are: msgId, DLC
     *
     *
     *  \return  true: valid false: not valid
     */
    bool dbcSync( void );

    bool setSignalValue( QString name, quint64 value );

    /** \brief
     *    Convert message information into byte sequence
     *
     * \note
     *    Message should be valid given current dbc model
     *
     *  \return  byte array
     */
    QByteArray *toByteArray();



     /** \brief
     *    set dbc model and related members
     *
     * \note
     *    one dbc model per loaded *.dbc file
     *
     *  \return true: success
     */
    void setDbcModel(DbcData *model );

     /** \brief
     *     read message parameters out of JSON object
     *
     * \note
     *     useful while loading scripts or poulating generator blocks
     *
     *  \return true: success
     */
    bool readJson(  QJsonObject jobject, bool custom = false );



    quint8 getPeriod();

    quint8 getDLC();

    bool getCustom();

    quint16 getMsgId();

    QString getName();




    /** \brief
     *     in case this message part of a tx queue, tell wheter
     *     it has been trasmitted or not.
     *
     *  \return true: transmitted false: pending
     */
    bool getServiced( );
    void setServiced( bool service );


    void setComInterface( ComInterface* com );
    ComInterface* getComInterface( );

private:
    static const qint32 ID_SZ = 2;
    static const qint32 DLC_SZ = 1;

    bool custom;
    bool valid;
    bool serviced;

    bool  period;

    QString dbcName;
    QString name;
    quint16 msgId;
    quint32 dlc;

    ComInterface* comInterface;

    QList< DbcData::dbcSignal > signalList;

    QByteArray dataBytes;

    QDbcModel* model;
    DbcData* dbcData;

};

#endif // CANMSG_H
