#ifndef QIGITEM_H
#define QIGITEM_H
#include <QList>
#include <QVariant>
class QIGItem
{
public:
    explicit QIGItem(int r = 0, QIGItem *parent = nullptr);
    QIGItem(QIGItem &item);
    ~QIGItem();
    QIGItem* parent() const;
    int row() const;
    void setData(int c, QVariant value);
    QVariant data(int c) const;
    QList<QVariant> data() const;
    QList<QIGItem*> children() const;
    QIGItem *child(int r) const;
    bool insertChild(int r,QIGItem* item);
    void insertChild(QIGItem* item);
    bool removeChild(int r);
    bool insertData(int c, QVariant value);
    void insertData(QVariant value);
    bool removeData(int c);
    int childrenCount() const;
    int dataCount() const;
private:
    QIGItem* itemParent;
    int itemRow;
    QList<QVariant> dataList;
    QList<QIGItem*> childrenList;
};

#endif // QIGITEM_H
