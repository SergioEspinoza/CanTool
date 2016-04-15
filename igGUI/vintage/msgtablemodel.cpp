#include <QAbstractTableModel>
#include "msgtablemodel.h"

MsgTableModel::MsgTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{


}


int MsgTableModel::rowCount(const QModelIndex &parent = QModelIndex()) const
{
        return msgModel.size();
}

int MsgTableModel::columnCount(const QModelIndex &parent = QModelIndex()) const
{
       return MSG_MODEL_COL_NUM;
}

/*! \name data
 *
 *  \brief override function, get data from data model associated with given index
 *
 */
QVariant MsgTableModel::data(const QModelIndex & index, int role = Qt::DisplayRole) const
{
    QVariant retVariant;
    int col = index.column();
    int row = index.row();


    if( row <  msgModel.size() )
    {
            switch( role )
            {
            // maybe used later
            case Qt::DisplayRole:

                    break;

                    // edit item
            case Qt::EditRole:
                    switch( col )
                    {
                    // edit text
                    case MsgTableModel::MSG_TABLE_COL_NAME:
                            retVariant = QVariant( msgModel.at( row ).name );
                            break;

                            //edit text
                    case MsgTableModel::MSG_TABLE_COL_IDENTIFIER:

                        retVariant = QVariant( msgModel.at( row ).id );

                        break;

                    case MsgTableModel::MSG_TABLE_COL_CYCLE2:

                        retVariant = QVariant( msgModel.at(row).cicleTime );
                        break;


                    case MsgTableModel::MSG_TABLE_COL_BURST:
                    case MsgTableModel::MSG_TABLE_COL_DATA0:
                    case MsgTableModel::MSG_TABLE_COL_DATA1:
                    case MsgTableModel::MSG_TABLE_COL_DATA2:
                    case MsgTableModel::MSG_TABLE_COL_DATA3:
                    case MsgTableModel::MSG_TABLE_COL_DATA4:
                    case MsgTableModel::MSG_TABLE_COL_DATA5:
                    case MsgTableModel::MSG_TABLE_COL_DATA6:
                    case MsgTableModel::MSG_TABLE_COL_DATA7:

                            break;

                            //Combo box
                    case MsgTableModel::MSG_TABLE_COL_CHANNEL:
                    case MsgTableModel::MSG_TABLE_COL_FRAME:
                    case MsgTableModel::MSG_TABLE_COL_DLC:
                    case MsgTableModel::MSG_TABLE_COL_GTWY2:


                            break;

                    case MsgTableModel::MSG_TABLE_COL_SEND:
                            break;

                            //checkbox
                    case MsgTableModel::MSG_TABLE_COL_KEY1:
                    case MsgTableModel::MSG_TABLE_COL_KEY2:
                    case MsgTableModel::MSG_TABLE_COL_KEY3:
                    case MsgTableModel::MSG_TABLE_COL_CYCLE1:
                    case MsgTableModel::MSG_TABLE_COL_HIGHLOAD:
                    case MsgTableModel::MSG_TABLE_COL_GTWY1:

                            break;

                            //TODO: set resolution
                    case MsgTableModel::MSG_TABLE_COL_CYCLE3:

                            break;
                    }


                    break;

            default:

                    break;
            }

    }
    return retVariant;

}

bool MsgTableModel::setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ) const
{


}


Qt::ItemFlags MsgTableModel::flags(const QModelIndex & index) const
{


}





bool MsgTableModel::insertRows(int row, int count, const QModelIndex & parent = QModelIndex())
{
    bool inserted = false;

    beginInsertRows();

    for( int curRow = count ; count > 0 ; count-- )
    {


    }

    endInsertRows();

   return inserted;
}


QModelIndex MsgTableModel::index( int row, int column, const QModelIndex &parent = QModelIndex()) const
{
    //TODO: complete
        return createIndex( row, column, nullptr );
}

