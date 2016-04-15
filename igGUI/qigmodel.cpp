#include "qigmodel.h"
#include <QDebug>
#include <QBrush>
#include <QLinearGradient>
QIGModel::QIGModel(QVariant value, QObject *parent) :
    QAbstractItemModel(parent), modelData(value){
    //qDebug() << "IG Model constructor";
    modelRoot = new QIGItem();
}

QIGModel::~QIGModel(){
   // qDebug() << "IG Model Destructor";
    delete modelRoot;
}

QVariant QIGModel::data(const QModelIndex &index, int role) const{
    if((!index.isValid())){
     //   qDebug()<< "IG data() >> Invalid index or model";
        return QVariant();
    }
    if(role == MODELINFO){
     //   qDebug() << "IG data() >> Model info";
        return modelData;
    }
    else if(role == TYPE){
        QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
     //   qDebug() << "IG data() >> IG item type";
        return item->data(0);
    }
    else if(role == Qt::DisplayRole){
     //   qDebug() << "IG data() >> DisplayRole";
        QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
     //   qDebug() << "IG data () >>"<<item->data(index.column()+1);
        return item->data(index.column()+1);
    }
    else if (role == Qt::BackgroundRole){
     //   qDebug() << "IG data() >> BackgroundRole";
        QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
        if (item->data(0).toBool()){
            return QBrush();
        }else{
            if((index.row()==0)&&(index.column()<3)){
                QLinearGradient gradient = QLinearGradient(QPointF(10,10),QPointF(10,50));
                gradient.setSpread(QGradient::ReflectSpread);
                gradient.setColorAt(0, Qt::white);
                gradient.setColorAt(1, Qt::gray);
                QBrush brush_settings = QBrush(gradient);
                return brush_settings;
            }else if (index.column()<3){
                QLinearGradient gradient = QLinearGradient(QPointF(10,10),QPointF(10,50));
                gradient.setSpread(QGradient::ReflectSpread);
                gradient.setColorAt(0,Qt::black);
                gradient.setColorAt(1,Qt::darkRed);
                return QBrush(gradient);
            }else{
                return QBrush();
            }
        }
    }
    else if (role == Qt::ForegroundRole){
     //   qDebug() << "IG data() >> ForegroundRole";
        QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
        if (item->data(0).toBool()){
            return QBrush();
        }else{
            if((index.row()==0)&&(index.column()<3)){
                return QBrush(Qt::black);
            }else{
                return QBrush(Qt::white);
            }
        }
    }
    return QVariant();
}
QModelIndex QIGModel::index(int row, int column, const QModelIndex &parent) const{
    if(!hasIndex(row, column, parent)){
    //    qDebug() << "IG index() >> invalid index";
        return QModelIndex();
    }
    QIGItem *itemParent;
    if(!parent.isValid()){
     //   qDebug() << "IG index() >> Invalid parent";
        itemParent = modelRoot;
    }
    else{
        itemParent = static_cast<QIGItem*>(parent.internalPointer());
    }
    QIGItem *itemChild = itemParent->child(row);
    if(itemChild){
        return createIndex(row, column, itemChild);
    }
    else{
     //   qDebug()<<"IG index() >> Invalid child";
        return QModelIndex();
    }
}
QModelIndex QIGModel::parent(const QModelIndex &index) const{
    if(!index.isValid()){
        return QModelIndex();
    }
    QIGItem *childItem = static_cast<QIGItem*>(index.internalPointer());
    QIGItem *parentItem = childItem->parent();
    if (parentItem == nullptr || parentItem == modelRoot){
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant QIGModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch (section){
            case 0:
                return tr("Message ID");
            case 1:
                return tr( "Message Name");
            case 2:
                return tr("DLC");
            case 3:
                return tr("Transmission Settings");
            case 4:
                return tr("Data Field");
            case 5:
                return tr("");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

int QIGModel::rowCount(const QModelIndex &parent) const{
    QIGItem *itemParent;
    if(parent.column() > 0){
        return 0;
    }
    if(!parent.isValid()){
        itemParent = modelRoot;
    }
    else{
        itemParent = static_cast<QIGItem*>(parent.internalPointer());
    }
    return itemParent->childrenCount();
}

int QIGModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return MAX_NDATA;
}

Qt::ItemFlags QIGModel::flags(const QModelIndex &index) const{
    if(!index.isValid()){
        return 0;
    }
    QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
    if (item->data(0).toBool()){
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }else{
        if ((index.column()>2) || (index.row()==0)){
            return Qt::NoItemFlags;
        }
        else{
            return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
        }
    }
}
bool QIGModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!index.isValid() | (role != Qt::EditRole)){
        return false;
    }
    QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
    item->setData(index.column()+1,value);
    return true;
}

bool QIGModel::insertChild(int row, QIGItem *child, const QModelIndex &parent){
    QIGItem *itemParent;
    bool res = true;
    if(parent.column() > 0){
        return 0;
    }
    if(!parent.isValid()){
        itemParent = modelRoot;
    }
    else{
        itemParent = static_cast<QIGItem*>(parent.internalPointer());
    }
    beginInsertRows(parent, row, row);
    res &= itemParent->insertChild(row,child);
    endInsertRows();
    return res;
}

bool QIGModel::insertRows(int row, int count, const QModelIndex &parent){
    QIGItem *itemParent;
    bool res = true;
    if(parent.column() > 0){
        return 0;
    }
    if(!parent.isValid()){
        itemParent = modelRoot;
    }
    else{
        itemParent = static_cast<QIGItem*>(parent.internalPointer());
    }
    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++){
        QIGItem *child = new QIGItem(row+i,itemParent);
        res &= itemParent->insertChild(row+i,child);
    }
    endInsertRows();
    return res;
}

bool QIGModel::removeRows(int row, int count, const QModelIndex &parent){
    QIGItem *itemParent;
    bool res = true;
    if(parent.column() > 0){
        return 0;
    }
    if(!parent.isValid()){
        itemParent = modelRoot;
    }
    else{
        itemParent = static_cast<QIGItem*>(parent.internalPointer());
    }
    beginRemoveRows(parent,row, row+count-1);
    for(int i = 0; i < count; i++){
     //   qDebug() << " new row" << row + i;
        res &= itemParent->removeChild(row+i);
    }
    endRemoveRows();
    return res;
}
