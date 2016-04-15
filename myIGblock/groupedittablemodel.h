#ifndef GROUPEDITTABLEMODEL_H_
#define GROUPEDITTABLEMODEL_H_

#include <QByteArray>
#include <QAbstractTableModel>
#include <QModelIndex>

#include "canMsg/canmsggroup.h"
#include "canMsg/canmsg.h"

class GroupEditTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit GroupEditTableModel(QObject *parent = 0);

    // update this count if adding more columns
    static const int MSG_MODEL_COL_NUM = 24;
    // column contents

    //text edit
    static const int MSG_TABLE_COL_NAME = 0;
    static const QString MSG_NAME_HEADER;

    // text edit
    static const int MSG_TABLE_COL_IDENTIFIER = 1;
    static const QString MSG_ID_HEADER;

    //combo box
    static const int MSG_TABLE_COL_CHANNEL = 2;
    static const QString MSG_CHANNEL_HEADER;

    // combo box
    static const int MSG_TABLE_COL_FRAME = 3;
    static const QString MSG_FRAME_HEADER;

    // combo box
    static const int MSG_TABLE_COL_DLC = 4;
    static const QString MSG_DLC_HEADER;

    // push button
    static const int MSG_TABLE_COL_SEND = 5;
    static const QString MSG_SEND_HEADER;

    //check box
    static const int MSG_TABLE_COL_KEY1 = 6;
    //check box
    static const int MSG_TABLE_COL_KEY2 = 7;
    //check box
    static const int MSG_TABLE_COL_KEY3 = 8;
    static const QString MSG_KEY_HEADER;

    // check box
    static const int MSG_TABLE_COL_CYCLE1 = 9;
    // text edit
    static const int MSG_TABLE_COL_CYCLE2 = 10;
    //slider
    static const int MSG_TABLE_COL_CYCLE3 = 11;
    static const QString MSG_CYCLE_HEADER;

    // text edit
    static const int MSG_TABLE_COL_BURST = 12;
    static const QString MSG_BURST_HEADER;

    //check box
    static const int MSG_TABLE_COL_HIGHLOAD = 13;
    static const QString MSG_HILOAD_HEADER;


    //check box
    static const int MSG_TABLE_COL_GTWY1 = 14;
    // combo box
    static const int MSG_TABLE_COL_GTWY2 = 15;
    static const QString MSG_GTWY_HEADER;

    //text edit
    static const int MSG_TABLE_COL_DATA0 = 16;
    static const QString MSG_DATA_HEADER0;

    //text edit
    static const int MSG_TABLE_COL_DATA1 = 17;
    static const QString MSG_DATA_HEADER1;

    //text edit
    static const int MSG_TABLE_COL_DATA2 = 18;
    static const QString MSG_DATA_HEADER2;
    //text edit
    static const int MSG_TABLE_COL_DATA3 = 19;
    static const QString MSG_DATA_HEADER3;

    //text edit
    static const int MSG_TABLE_COL_DATA4 = 20;
    static const QString MSG_DATA_HEADER4;
    //text edit
    static const int MSG_TABLE_COL_DATA5 = 21;
    static const QString MSG_DATA_HEADER5;

    //text edit
    static const int MSG_TABLE_COL_DATA6 = 22;
    static const QString MSG_DATA_HEADER6;

    //text edit
    static const int MSG_TABLE_COL_DATA7 = 23;
    static const QString MSG_DATA_HEADER7;

   int rowCount( const QModelIndex &parent = QModelIndex()) const override;
   int columnCount( const QModelIndex &parent = QModelIndex()) const override;
   QVariant data( const QModelIndex & index, int role = Qt::DisplayRole) const override;
   bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ) override;
   Qt::ItemFlags flags(const QModelIndex & index) const override;
   bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;
   QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
   QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
   QModelIndex parent(const QModelIndex &index) const override;

   void setModelData(CanMsgGroup& msgGroupParam );
   CanMsgGroup getModelData( );
   void addMsg( CanMsg& msg );


   struct MsgModelRow
    {
        QString name;
        qint32  id;
        qint32  channel;
        QString frame;
        qint32  dlc;

        bool    key_enabled;
        bool    key_test;
        char    key;

        bool    ciclying_enabled;
        qint32  cicleTime;

        qint32  burst;
        bool    highLoad;

        bool    gateway_enabled;
        qint32  gateway_channel;

        QByteArray pdbytes;

        MsgModelRow( void )
        {
            pdbytes.resize( MSG_DATA_FIELDS );
            pdbytes.fill( 0x00 );
        }
    };

signals:
    //void headerDataChanged(Qt::Orientation orientation, int first, int last) const;

public slots:


private:
    QList< QVariant > msgHeaderLst;
    QString sendBtnTxt;
    static const int MSG_DATA_FIELDS = 8;

    /*
    enum channel_t
    {
        //support only one channel
        channel_one,
        //channel_two,
        channel_max
    }eChannel;
*/



    // main data source
    CanMsgGroup msgGroup;

    QList< MsgModelRow > msgModel;

};

#endif // GROUPEDITTABLEMODEL_H_
