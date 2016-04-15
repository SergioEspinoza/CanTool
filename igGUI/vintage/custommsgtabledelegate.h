#ifndef MSGTABLEVIEWDELEGATE_H
#define MSGTABLEVIEWDELEGATE_H

#include <QStyledItemDelegate>
#include <QHash>

class CustomMsgTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CustomMsgTableDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:

public slots:

private:


};

#endif // MSGTABLEVIEWDELEGATE_H
