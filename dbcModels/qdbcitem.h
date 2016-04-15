#ifndef QDBCITEM_H
#define QDBCITEM_H

#include <dbcModel.h>
#include "qattributedbc.h"

#define DBItemRole Qt::UserRole
#define DBItemIDRole Qt::UserRole+10
#define DBItemTypeRole Qt::UserRole+20
typedef enum item_type{
    NO_TYPE,
    CLASS_TYPE,
    MESSAGE_TYPE,
    SIGNAL_TYPE
} item_val_type;

class QDbcItem
{
public:
    QDbcItem(const QDbcItem &item);
    QDbcItem(const QDbcItem *item);
    QDbcItem(item_val_type type=NO_TYPE, int row=0, QDbcItem *parent = 0);
    ~QDbcItem();
    void addChild(QDbcItem *child);
    QDbcItem* getChild(int i);
    QDbcItem* getChildbyName(QString &name);
    QHash<int, QDbcItem*> getChildren() const;
    QDbcItem* getParent() const;
    bool setupItem(string_t str);
    bool setupMessage(message_t *message);
    bool setupSignal(signal_t *signal);
    int getRow() const;
    int rowCount();
    //General parameters
    QList<QAttributeDbc*> attribute_list;
    item_val_type type;   //{None, MessageClass, Message, Signal}
    QString     name;
    QString     comment;
    //Message parameters
    QVariant    id;
    QVariant    len;
    QString     sender;
    QStringList trasmitter_list;
    //Signal parameters
    QVariant    mux_type;
    QVariant    mux_value;
    QVariant    bit_start;
    QVariant    bit_len;
    QVariant    endianess;
    QVariant    signedness;
    QVariant    scale;
    QVariant    offset;
    QVariant    min;
    QVariant    max;
    QVariant    signal_val_type;
    QString     unit;
    QStringList receiver_list;
    QHash<int, QString> val_map;
private:
    QHash<int, QDbcItem*> childrenTable;
    int row;
    QDbcItem*   parentItem;

};
Q_DECLARE_METATYPE(QDbcItem)
#endif // QDBCITEM_H
