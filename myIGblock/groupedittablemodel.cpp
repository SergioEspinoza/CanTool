#include <QAbstractTableModel>
#include "scriptEditGUI/groupedittablemodel.h"

#define SND_BTN_TEXT "NOW"


const QString GroupEditTableModel::MSG_NAME_HEADER = "MESSAGE NAME";
const QString GroupEditTableModel::MSG_ID_HEADER = "IDENTIFIER";
const QString GroupEditTableModel::MSG_CHANNEL_HEADER = "CHANNEL";
const QString GroupEditTableModel::MSG_FRAME_HEADER = "FRAME";
const QString GroupEditTableModel::MSG_DLC_HEADER = "DLC";
const QString GroupEditTableModel::MSG_SEND_HEADER = "SEND";
const QString GroupEditTableModel::MSG_KEY_HEADER = "KEY";
const QString GroupEditTableModel::MSG_CYCLE_HEADER = "CYCLE TIME [mS]";
const QString GroupEditTableModel::MSG_BURST_HEADER = "BURST";
const QString GroupEditTableModel::MSG_HILOAD_HEADER = "HI LOAD";
const QString GroupEditTableModel::MSG_GTWY_HEADER = "GATEWAY";
const QString GroupEditTableModel::MSG_DATA_HEADER0 = "0";
const QString GroupEditTableModel::MSG_DATA_HEADER1 = "1";
const QString GroupEditTableModel::MSG_DATA_HEADER2 = "2";
const QString GroupEditTableModel::MSG_DATA_HEADER3 = "3";
const QString GroupEditTableModel::MSG_DATA_HEADER4 = "4";
const QString GroupEditTableModel::MSG_DATA_HEADER5 = "5";
const QString GroupEditTableModel::MSG_DATA_HEADER6 = "6";
const QString GroupEditTableModel::MSG_DATA_HEADER7 = "7";


GroupEditTableModel:: GroupEditTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{

    // total columns
    msgHeaderLst.insert( MSG_TABLE_COL_NAME, MSG_NAME_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_IDENTIFIER, MSG_ID_HEADER);
    msgHeaderLst.insert( MSG_TABLE_COL_CHANNEL, MSG_CHANNEL_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_FRAME, MSG_FRAME_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_DLC, MSG_DLC_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_SEND, MSG_SEND_HEADER );

    msgHeaderLst.insert( MSG_TABLE_COL_KEY1, MSG_KEY_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_KEY2, MSG_KEY_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_KEY3, MSG_KEY_HEADER );

    msgHeaderLst.insert( MSG_TABLE_COL_CYCLE1, MSG_CYCLE_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_CYCLE2, MSG_CYCLE_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_CYCLE3, MSG_CYCLE_HEADER );

    msgHeaderLst.insert( MSG_TABLE_COL_BURST, MSG_BURST_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_HIGHLOAD, MSG_HILOAD_HEADER );

    msgHeaderLst.insert( MSG_TABLE_COL_GTWY1, MSG_GTWY_HEADER );
    msgHeaderLst.insert( MSG_TABLE_COL_GTWY2, MSG_GTWY_HEADER );

    msgHeaderLst.insert( MSG_TABLE_COL_DATA0, MSG_DATA_HEADER0 );
    msgHeaderLst.insert( MSG_TABLE_COL_DATA1, MSG_DATA_HEADER1 );
    msgHeaderLst.insert( MSG_TABLE_COL_DATA2, MSG_DATA_HEADER2 );
    msgHeaderLst.insert( MSG_TABLE_COL_DATA3, MSG_DATA_HEADER3 );
    msgHeaderLst.insert( MSG_TABLE_COL_DATA4, MSG_DATA_HEADER4 );
    msgHeaderLst.insert( MSG_TABLE_COL_DATA5, MSG_DATA_HEADER5 );
    msgHeaderLst.insert( MSG_TABLE_COL_DATA6, MSG_DATA_HEADER6 );
    msgHeaderLst.insert( MSG_TABLE_COL_DATA7, MSG_DATA_HEADER7 );

    emit( headerDataChanged( Qt::Horizontal, 0, 24 ) );

}


int GroupEditTableModel::rowCount(const QModelIndex &parent  /* = QModelIndex() */ ) const
{
        return msgModel.size();
}

int GroupEditTableModel::columnCount(const QModelIndex &parent /* = QModelIndex() */ ) const
{
       return MSG_MODEL_COL_NUM;
}

/*! \name data
 *
 *  \brief override function, get data from data model associated with given index
 *
 */
QVariant GroupEditTableModel::data( const QModelIndex & index, int role /* = Qt::DisplayRole */) const
{
    QVariant retVariant;
    int col = index.column();
    int row = index.row();

    MsgModelRow customMsgRow = msgModel.at( row );

    if( row <  msgModel.size( ) )
    {
                // treat equally Display and Edit roles for now
            if( ( role == Qt::EditRole ) ||
                 ( role == Qt::DisplayRole ) )
            {
                    switch( col )
                    {
                    // text edit
                    case GroupEditTableModel::MSG_TABLE_COL_NAME:
                            retVariant = QVariant( customMsgRow.name );
                            break;

                            // text edit
                    case GroupEditTableModel::MSG_TABLE_COL_IDENTIFIER:
                            retVariant = QVariant( customMsgRow.id );
                            break;

                            // combo box
                    case GroupEditTableModel::MSG_TABLE_COL_CHANNEL:
                            retVariant = QVariant( customMsgRow.channel );
                            break;


                            // conbo box
                    case GroupEditTableModel::MSG_TABLE_COL_FRAME:
                            retVariant = QVariant( customMsgRow.frame );
                            break;

                            // combobox
                    case GroupEditTableModel::MSG_TABLE_COL_DLC:
                            retVariant = QVariant( customMsgRow.dlc );

                            break;

                            // push button (display only)
                    case GroupEditTableModel::MSG_TABLE_COL_SEND:
                            retVariant = QVariant( sendBtnTxt );
                            break;

                            // checkbox
                    case GroupEditTableModel::MSG_TABLE_COL_KEY1:
                            retVariant = QVariant( customMsgRow.key_enabled );
                            break;

                            // checkbox
                    case GroupEditTableModel::MSG_TABLE_COL_KEY2:
                            retVariant = QVariant( customMsgRow.key_test );
                            break;

                            // text edit
                    case GroupEditTableModel::MSG_TABLE_COL_KEY3:
                            retVariant = QVariant( customMsgRow.key );
                            break;


                            //checkbox
                    case GroupEditTableModel::MSG_TABLE_COL_CYCLE1:
                            retVariant = QVariant( customMsgRow.ciclying_enabled  );
                            break;

                            // text edit
                    case  GroupEditTableModel::MSG_TABLE_COL_CYCLE2:
                            //slider
                    case GroupEditTableModel::MSG_TABLE_COL_CYCLE3:
                            //TODO: check for paint
                            retVariant = QVariant( customMsgRow .cicleTime );
                            break;


                            // text edit
                    case GroupEditTableModel::MSG_TABLE_COL_BURST:
                            retVariant = QVariant( customMsgRow.burst );
                            break;

                            // checkbox
                    case GroupEditTableModel::MSG_TABLE_COL_HIGHLOAD:
                            retVariant = QVariant( customMsgRow.highLoad );
                            break;

                            //checkbox
                    case GroupEditTableModel::MSG_TABLE_COL_GTWY1:
                            retVariant = QVariant( customMsgRow.gateway_enabled );
                            break;

                            // combobox
                    case GroupEditTableModel::MSG_TABLE_COL_GTWY2:
                            retVariant = QVariant( customMsgRow.gateway_channel );
                            break;

                            //text edit
                    case GroupEditTableModel::MSG_TABLE_COL_DATA0:
                    case GroupEditTableModel::MSG_TABLE_COL_DATA1:
                    case GroupEditTableModel::MSG_TABLE_COL_DATA2:
                    case GroupEditTableModel::MSG_TABLE_COL_DATA3:
                    case GroupEditTableModel::MSG_TABLE_COL_DATA4:
                    case GroupEditTableModel::MSG_TABLE_COL_DATA5:
                    case GroupEditTableModel::MSG_TABLE_COL_DATA6:
                    case GroupEditTableModel::MSG_TABLE_COL_DATA7:

                            retVariant = QVariant( customMsgRow.pdbytes.at( col - GroupEditTableModel::MSG_TABLE_COL_DATA0 ) );
                            break;

                    }
            }
    }
    return retVariant;
}

/*! \name setData
 *
 *  \brief set data back into model
 *
 */
bool GroupEditTableModel::setData(const QModelIndex & index, const QVariant & value, int role /* = Qt::EditRole */ )
{


}
/*! \name headerData
 *
 *  \brief  Returns the data for the given role and section in the header
 *          with the specified orientation
 *
 */
QVariant GroupEditTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant retVariant = QVariant();

    if( ( orientation == Qt::Horizontal ) && ( role == Qt::DisplayRole ) )
    {
        // section is colum number
        retVariant = msgHeaderLst.value( section, QVariant() );

        // non-const signal not allowed from const method
    }

    emit( SIGNAL( headerDataChanged( Qt::Horizontal, 0, 24 ) ) );

    return retVariant;
}

QModelIndex GroupEditTableModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

Qt::ItemFlags GroupEditTableModel::flags(const QModelIndex & index) const
{


}

bool GroupEditTableModel::insertRows(int row, int count, const QModelIndex & parent /* = QModelIndex() */ )
{
    bool inserted = false;

    //beginInsertRows();

    for( int curRow = count ; count > 0 ; count-- )
    {


    }

    endInsertRows();

   return inserted;
}


QModelIndex GroupEditTableModel::index( int row, int column, const QModelIndex &parent /* = QModelIndex() */ ) const
{
    //TODO: complete
    return createIndex( row, column, nullptr );
}


void GroupEditTableModel::setModelData( CanMsgGroup& msgGroupParam )
{
    msgGroup = msgGroupParam;
}

CanMsgGroup GroupEditTableModel::getModelData( )
{
    return msgGroup;
}

void GroupEditTableModel::addMsg( CanMsg& msg )
{
   msgGroup.addMsg( msg );

   //TODO: check this
   emit( SIGNAL(rowsInserted(QModelIndex(), 0 , msgGroup.getMessages().size() )) );

}


