#include "qigitem.h"

QIGItem::QIGItem(int r, QIGItem *parent)
    :itemParent(parent), itemRow(r){
    childrenList = QList<QIGItem*>();
    childrenList.empty();
    dataList = QList<QVariant>();
    dataList.empty();
}

QIGItem::QIGItem(QIGItem &item)
    :itemParent(item.parent()), itemRow(item.row()){
    QList<QIGItem*> refchildrenList = item.children();
    dataList = QList<QVariant>(item.data());
    for(QIGItem *itr:refchildrenList){
        QIGItem * clone = new QIGItem(*itr);
        insertChild(itr->row(),clone);
    }
}

QIGItem::~QIGItem(){
    qDeleteAll(childrenList);
}

QIGItem* QIGItem::parent() const{
    return itemParent;
}
int QIGItem::row() const{
    return itemRow;
}
void QIGItem::setData(int c,QVariant value){
    if (c<dataCount()){
        dataList[c] = value;
    }
}
QVariant QIGItem::data(int c) const{
    if (c >= dataCount()){
        return QVariant();
    }
    return dataList.at(c);
}
QList<QVariant> QIGItem::data() const{
    return dataList;
}
QList<QIGItem*> QIGItem::children() const{
    return childrenList;
}
QIGItem* QIGItem::child(int r) const{
    if(r >= childrenCount()){
        return nullptr;
    }
    return childrenList.at(r);
}
bool QIGItem::insertChild(int r, QIGItem* item){
    if((r <0) || (r>childrenCount())){
       return false;
    }else{
        childrenList.insert(r,item);
        return true;
    }
}
void QIGItem::insertChild(QIGItem* item){
    childrenList.append(item);
}
bool QIGItem::removeChild(int r){
    if((r<0) || (r >= childrenCount())){
        return false;
    }else{
        childrenList.removeAt(r);
        return true;
    }
}
bool QIGItem::insertData(int c, QVariant value){
    if((c < 0) || (c>dataCount())){
        return false;
    }
    else{
        dataList.insert(c,value);
        return true;
    }
}
void QIGItem::insertData(QVariant value){
    dataList.append(value);
}
bool QIGItem::removeData(int c){
    if((c < 0) || (c>=dataCount())){
        return false;
    }else{
        dataList.removeAt(c);
        return true;
    }
}
int QIGItem::childrenCount() const{
    return childrenList.size();
}
int QIGItem::dataCount() const{
    return dataList.size();
}
