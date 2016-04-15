#ifndef QIGMODEL_H
#define QIGMODEL_H
#include <QAbstractItemModel>
#include "qigitem.h"

#define MODELINFO Qt::UserRole+1
#define TYPE      Qt::UserRole+2
#define MAX_NDATA 5

class QIGModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QIGModel(QVariant data = QVariant(), QObject *parent = nullptr);
    ~QIGModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role =  Qt::DisplayRole) const;
    bool insertChild(int row, QIGItem *child, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
private:
    QIGItem* modelRoot;
    QVariant modelData;
};

#endif // QIGMODEL_H
