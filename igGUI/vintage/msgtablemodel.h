#ifndef MSGTABLEMODEL_H
#define MSGTABLEMODEL_H

#include <QByteArray>
#include <QAbstractTableModel>
#include <QModelIndex>

class MsgTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:


    static const int MSG_MODEL_COL_NUM = 24;
        // column contents

    //text edit
    static const int MSG_TABLE_COL_NAME = 0;
   // text edit
    static const int MSG_TABLE_COL_IDENTIFIER = 1;
   //combo box
    static const int MSG_TABLE_COL_CHANNEL = 2;
    // combo box
    static const int MSG_TABLE_COL_FRAME = 3;
    // combo box
    static const int MSG_TABLE_COL_DLC = 4;
    // push button
    static const int MSG_TABLE_COL_SEND = 5;
    //check box
    static const int MSG_TABLE_COL_KEY1 = 6;
    //check box
    static const int MSG_TABLE_COL_KEY2 = 7;
    //check box
    static const int MSG_TABLE_COL_KEY3 = 8;
    //check box
    static const int MSG_TABLE_COL_CYCLE1 = 9;
    // text edit
    static const int MSG_TABLE_COL_CYCLE2 = 10;
    //slider
    static const int MSG_TABLE_COL_CYCLE3 = 11;
    // text edit
    static const int MSG_TABLE_COL_BURST = 12;
    //check box
    static const int MSG_TABLE_COL_HIGHLOAD = 13;
    //check box
    static const int MSG_TABLE_COL_GTWY1 = 14;
    // combo box
    static const int MSG_TABLE_COL_GTWY2 = 15;
    //text edit
    static const int MSG_TABLE_COL_DATA0 = 16;
    //text edit
    static const int MSG_TABLE_COL_DATA1 = 17;
    //text edit
    static const int MSG_TABLE_COL_DATA2 = 18;
    //text edit
    static const int MSG_TABLE_COL_DATA3 = 19;
    //text edit
    static const int MSG_TABLE_COL_DATA4 = 20;
    //text edit
    static const int MSG_TABLE_COL_DATA5 = 21;
    //text edit
    static const int MSG_TABLE_COL_DATA6 = 22;
    //text edit
    static const int MSG_TABLE_COL_DATA7 = 23;




    explicit MsgTableModel(QObject *parent = 0);


   int rowCount( const QModelIndex &parent = QModelIndex()) const override;
   int columnCount( const QModelIndex &parent = QModelIndex()) const override;
   QVariant data( const QModelIndex & index, int role = Qt::DisplayRole) const override;
   bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ) const override;
   Qt::ItemFlags flags(const QModelIndex & index) const override;
   bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;
   QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

signals:

public slots:


private:

    static const int MSG_DATA_FIELDS = 8;

    enum channel_t
    {
        //support only one channel
        channel_one,
        //channel_two,
        channel_max
    }eChannel;

    struct IgMsgCustomRow
    {
        QString name;
        qint32  id;
        channel_t channel;
        QString frame;
        qint32  data;

        bool    key_enabled;
        bool    key_test;
        char    key;

        bool    ciclying_enabled;
        qint32  cicleTime;

        qint32  burst;
        bool    highLoad;

        bool    gateway;

        QByteArray pdbytes;

        IgMsgCustomRow::IgMsgCustomRow( void )
        {
            pdbytes.resize( MSG_DATA_FIELDS );
            pdbytes.fill( 0x00 );
        }
    };

    QList< IgMsgCustomRow > msgModel;
};

#endif // MSGTABLEMODEL_H
