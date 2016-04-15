#ifndef SIGNALEDITTABLEMODEL_H
#define SIGNALEDITTABLEMODEL_H

#include <QAbstractTableModel>
#include "canMsg/canmsg.h"

class MsgEditTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MsgEditTableModel(QObject *parent = 0);

    void setModelData( CanMsg msg );
    CanMsg getModelData( );

   int rowCount( const QModelIndex &parent = QModelIndex()) const override;
   int columnCount( const QModelIndex &parent = QModelIndex()) const override;
   QVariant data( const QModelIndex & index, int role = Qt::DisplayRole) const override;



signals:

public slots:


private:

    // main data source
    CanMsg message;

};

#endif // SIGNALEDITTABLEMODEL_H
