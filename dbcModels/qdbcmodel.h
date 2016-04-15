#ifndef QDBCMODEL_H
#define QDBCMODEL_H

#include <QAbstractItemModel>
#include <QTreeView>
#include <QString>
#include <QDebug>
#include <dbcModel.h>
#include "qdbcitem.h"


class QDbcModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QDbcModel(dbc_t *database, QObject *parent = 0);
    ~QDbcModel();
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    dbc_t* getDatabase() const;
    QString getFilename() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QDbcItem* getRootItem() const;
private:
    bool setupModel(dbc_t *database);
    QDbcItem *rootItem;
    bool validModel;
    dbc_t *database;
};

#endif // QDBCMODEL_H
