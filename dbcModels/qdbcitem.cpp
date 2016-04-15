#include "qdbcitem.h"
#include <QDebug>
QDbcItem::QDbcItem(const QDbcItem *item)
    :attribute_list(item->attribute_list),type(item->type),name(item->name),
  comment(item->comment),id(item->id), len(item->len),sender(item->sender), 
  trasmitter_list(item->trasmitter_list),mux_type(item->mux_type), mux_value(item->mux_value),
  bit_start(item->bit_start), bit_len(item->bit_len), endianess(item->endianess), 
  signedness(item->signedness), scale(item->scale), offset(item->offset),min(item->min), 
  max(item->max), signal_val_type(item->signal_val_type),unit(item->unit),
  receiver_list(item->receiver_list), val_map(item->val_map), row(item->getRow()),
  parentItem(item->getParent()){
 //   qDebug() << "QDbcItem constructor copy from another QDbcItem";
    for(QDbcItem *child : item->getChildren()){
        QDbcItem *clone = new QDbcItem(child);
        addChild(clone);
    }
}

QDbcItem::QDbcItem(const QDbcItem &item)
    :attribute_list(item.attribute_list),type(item.type),name(item.name),
  comment(item.comment),id(item.id), len(item.len),sender(item.sender),
  trasmitter_list(item.trasmitter_list),mux_type(item.mux_type), mux_value(item.mux_value),
  bit_start(item.bit_start), bit_len(item.bit_len), endianess(item.endianess),
  signedness(item.signedness), scale(item.scale), offset(item.offset),min(item.min),
  max(item.max), signal_val_type(item.signal_val_type),unit(item.unit),
  receiver_list(item.receiver_list), val_map(item.val_map), row(item.getRow()),
  parentItem(item.getParent()){
    //   qDebug() << "QDbcItem constructor copy from another QDbcItem";
       for(QDbcItem *child : item.getChildren()){
           QDbcItem *clone = new QDbcItem(child);
           addChild(clone);
       }
}

QDbcItem::QDbcItem(item_val_type type, int row, QDbcItem *parent)
    :type(type), row(row), parentItem(parent){
 //   qDebug() << "QDbcItem constructor";
    trasmitter_list = QStringList();
    receiver_list = QStringList();
    val_map = QHash<int,QString>();
    attribute_list = QList<QAttributeDbc*>();
    childrenTable = QHash<int, QDbcItem*>();
    id     = QVariant(static_cast<qulonglong>(0));
    name   = QString(" ");
    len    = QVariant(0);
    sender = QString(" ");
    comment= QString(" ");
    mux_type = QVariant(0);
    mux_value = QVariant(static_cast<qulonglong>(0));
    bit_start = QVariant(0);
    bit_len = QVariant(0);
    endianess = QVariant(0);
    signedness = QVariant(0);
    scale = QVariant(0);
    offset = QVariant(0);
    min = QVariant(0);
    max = QVariant(0);
    signal_val_type = QVariant(0);
    unit = QString(" ");
    trasmitter_list.clear();
    receiver_list.clear();
    attribute_list.clear();
    childrenTable.clear();
    val_map.clear();
}

QDbcItem::~QDbcItem(){
 //   qDebug() << "QDbcItem destructor";
   // int j = 0;
    for (int i : childrenTable.keys()){
    //    qDebug() << "QDbcItem destructor i=" <<i;
     //   qDebug() << "QDbcItem destructor delete " << childrenTable.value(i)->name;
     //   qDebug() << "QDbcItem destructor #"<<j;
        delete childrenTable.value(i);
     //   j++;
    }
    childrenTable.clear();
}

bool QDbcItem::setupItem(string_t str){
    bool ret = false;
    if(str != 0){
        name   = QString(str);
        qDebug() << "New Database " << name;
        ret = true;
    }
    return ret;
}

bool QDbcItem::setupMessage(message_t *message){
    bool ret = false;
    if((message != 0)&&(type == MESSAGE_TYPE)){
        ret    = true;
        id     = QVariant(static_cast<qulonglong>(message->id));
        name   = QString(message->name);
        len    = QVariant(message->len);
        sender = QString(message->sender);
        comment= QString(message->comment);
        for(string_list_t *itr = message->transmitter_list; itr !=0; itr = itr->next){
            trasmitter_list.append(itr->string);
        }
        for(attribute_list_t *itr= message->attribute_list; itr != 0; itr = itr->next){
            QAttributeDbc *attribute = new QAttributeDbc(itr->attribute);
            attribute_list.append(attribute);
        }
    }
    return ret;
}

bool QDbcItem::setupSignal(signal_t *signal){
    bool ret = false;
    if((signal != 0) && (type == SIGNAL_TYPE)){
        ret = true;
        name = QString(signal->name);
        mux_type = QVariant(signal->mux_type);
        mux_value = QVariant(static_cast<qulonglong>(signal->mux_value));
        bit_start = QVariant(signal->bit_start);
        bit_len = QVariant(signal->bit_len);
        endianess = QVariant(signal->endianess);
        signedness = QVariant(signal->signedness);
        scale = QVariant(signal->scale);
        offset = QVariant(signal->offset);
        min = QVariant(signal->min);
        max = QVariant(signal->max);
        signal_val_type = QVariant(signal->signal_val_type);
        unit = QString(signal->unit);
        comment = QString(signal->comment);
        for (string_list_t *itr = signal->receiver_list; itr != 0; itr = itr->next){
            receiver_list.append(itr->string);
        }
        for(val_map_t *itr = signal->val_map; itr !=0; itr = itr->next){
            val_map.insert(itr->val_map_entry->index,QString(itr->val_map_entry->value));
        }
        for(attribute_list_t *itr = signal->attribute_list; itr !=0; itr = itr->next){
            QAttributeDbc *attribute = new QAttributeDbc(itr->attribute);
            attribute_list.append(attribute);
        }
    }
    return ret;
}

void QDbcItem::addChild(QDbcItem *child){
    childrenTable.insert(child->getRow(),child);
}

QDbcItem* QDbcItem::getChild(int i){
    QDbcItem* childptr = nullptr;
    if(childrenTable.contains(i)){
        childptr = childrenTable.value(i);
    }
    return childptr;
}

QHash<int, QDbcItem*> QDbcItem::getChildren() const{
    return childrenTable;
}

QDbcItem* QDbcItem::getChildbyName(QString &name){
    QDbcItem *childptr = nullptr;
    if(!name.isNull()){
        for(QDbcItem *itr : childrenTable.values()){
            if(itr->name==name){
                childptr = itr;
                break;
            }
        }
    }
    return childptr;
}

QDbcItem* QDbcItem::getParent() const{
    return parentItem;
}

int QDbcItem::getRow() const{
    return row;
}

int QDbcItem::rowCount(){
    return childrenTable.count();
}

