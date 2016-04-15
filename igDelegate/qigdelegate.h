#ifndef QIGDELEGATE_H
#define QIGDELEGATE_H
#include <QStyledItemDelegate>
#include "dbcModels/qdbcitem.h"
class QIGDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QIGDelegate(QObject *parent = nullptr, QWidget *viewport = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // QIGDELEGATE_H
