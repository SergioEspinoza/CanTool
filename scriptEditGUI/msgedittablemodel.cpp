#include "scriptEditGUI/msgedittablemodel.h"

MsgEditTableModel::MsgEditTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void MsgEditTableModel::setModelData( CanMsg msg )
{
    message = msg;
}

CanMsg MsgEditTableModel::getModelData( )
{
    return message;
}


int MsgEditTableModel::rowCount( const QModelIndex &parent ) const
{
   //TODO
}

int MsgEditTableModel::columnCount( const QModelIndex &parent ) const
{
   //TODO
}

QVariant MsgEditTableModel::data( const QModelIndex & index, int role ) const
{
   //TODO
}


