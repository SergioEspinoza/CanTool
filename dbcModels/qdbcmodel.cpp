#include "qdbcmodel.h"
#include <QDebug>
QDbcModel::QDbcModel(dbc_t *database, QObject *parent)
    : QAbstractItemModel(parent), database(database){
 //   qDebug()<<"Model Constructor ";
    rootItem = new QDbcItem(NO_TYPE, 0, 0);
    validModel = setupModel(database);
    rootItem->setupItem(database->filename);
  //  qDebug()<<"Is valid? "<<validModel;
}

QDbcModel::~QDbcModel(){
  //  qDebug()<<"Model Destructor ";
    delete rootItem;
}

QVariant QDbcModel::data(const QModelIndex &index, int role) const{
    if((!index.isValid()) || !validModel){
     //   qDebug()<<"data() >> Invalid index or model ";
        return QVariant();
    }
    if(role == DBItemTypeRole){
        qDebug()<<"data() >> DBItemTypeRole";
        QDbcItem *mdlitem = static_cast<QDbcItem*>(index.internalPointer());
        if((mdlitem->type != MESSAGE_TYPE) && (mdlitem->type != SIGNAL_TYPE)){
         //   qDebug()<<"data() >> DB Item CLASS or no type" << mdlitem->type;
            return QVariant();
        }else{
            return mdlitem->type;
        }
    }
    else if (role == DBItemIDRole){
     //   qDebug()<<"data() >> DBItemIDRole";
        QDbcItem *mdlitem = static_cast<QDbcItem*>(index.internalPointer());
        if((mdlitem->type != MESSAGE_TYPE) && (mdlitem->type != SIGNAL_TYPE)){
        //    qDebug()<<"data() >> DB Item CLASS or no type" << mdlitem->type;
            return QVariant();
        }
        else if(mdlitem->type == MESSAGE_TYPE){
            return mdlitem->id;
        }
        else{
            return mdlitem->getParent()->id;
        }
    }
    else if (role == DBItemRole){
        QVariant var = QVariant();
      //  qDebug()<<"data() >> DBItemRole";
        QDbcItem *item = static_cast<QDbcItem*>(index.internalPointer());
        if((item->type != MESSAGE_TYPE) && (item->type != SIGNAL_TYPE)){
        //    qDebug()<<"data() >> DB Item CLASS or no type" << item->type;
            return QVariant();
        }
        else if(item->type == MESSAGE_TYPE){
         //  qDebug()<<"data() >> DB Item Message type" << item->name;
           var.setValue(*item);
        }
        else{
            QDbcItem *parent = item->getParent();
            if ((parent == rootItem) || (!parent)){
        //        qDebug()<<"parent() >> Null parent or root parent";
                return QVariant();
            }
       //     qDebug()<<"data() >> DB Item SIGNAL type";
       //     qDebug()<<"data() >> from message" << parent->name;
            var.setValue(*parent);
        }
        return var;
    }
    else if(role == Qt::TextAlignmentRole){
      //  qDebug()<<"data() >> TextAlignmentRole";
        return Qt::AlignAbsolute + Qt::LeftToRight;
    }
    else if(role == Qt::DisplayRole){
     //   qDebug()<<"data() >> DisplayRole";
        QDbcItem *item = static_cast<QDbcItem*>(index.internalPointer());
        QString a = QString("r %1 c %2").arg(QString::number(index.row()),QString::number(index.column()));
        switch(index.column()){
        case 0:
      //      qDebug()<< a;
      //      qDebug()<<"Name "<< item->name;
            return item->name;
        case 1:
       //     qDebug()<< a;
       //     qDebug()<<"comment " << item->comment;
            return item->comment;
        default:
       //     qDebug()<< a;
       //     qDebug()<<"data() >> Outside boundaries";
            return QVariant();
        }
    }
    else{
    //   qDebug()<<"data() >> Invalid role "<< role;
       return QVariant();
    }
}

QModelIndex QDbcModel::index(int row, int column, const QModelIndex &parent) const{
    if (!hasIndex(row, column, parent) || !validModel){
      //  qDebug()<<"index() >> Invalid index or model";
        return QModelIndex();
    }
    QDbcItem *parentItem = nullptr;
    QDbcItem *childItem = nullptr;
    if (!parent.isValid()){
     //   qDebug()<<"index() >> Invalid parent";
        parentItem = rootItem;
    }
    else{
      //  qDebug()<<"index() >> Getting parent internal pointer";
        parentItem = static_cast<QDbcItem*>(parent.internalPointer());
    }
    if(parentItem->type == CLASS_TYPE){
     //   qDebug()<<"index() >> message type";
        childItem = parentItem->getChildren().values().at(row);
    }
    else{
      //  qDebug()<<"index() >> signal or class type " << parentItem->getChild(row)->type;
        childItem = parentItem->getChild(row);
    }
    if (childItem != nullptr){
      //  qDebug()<<"index() >> Create Index";
        return createIndex(row, column, childItem);
    }
    else{
      //  qDebug()<<"index() >> no child";
        return QModelIndex();
    }
}

QModelIndex QDbcModel::parent(const QModelIndex &index) const{
    if(!index.isValid() || !validModel){
     //   qDebug()<<"parent() >> Invalid index or model";
        return QModelIndex();
    }
    QDbcItem *childItem = static_cast<QDbcItem*>(index.internalPointer());
    QDbcItem *parentItem = childItem->getParent();
    if (parentItem == nullptr || parentItem == rootItem){
      //  qDebug()<<"parent() >> Null parent or root parent";
        return QModelIndex();
    }
    return createIndex(parentItem->getRow(), 0, parentItem);
}

QVariant QDbcModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch (section){
            case 0:
                return tr("Item");
            case 1:
                return tr("Description");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

int QDbcModel::rowCount(const QModelIndex &parent) const{
    if(parent.column() > 0){
     //   qDebug()<<"Out of boundary." << parent.column();
        return 0;
    }
    QDbcItem *parentItem;
    if(!parent.isValid() || !validModel){
     //   qDebug()<<"rowCount() >> Invalid parent or model";
        parentItem = rootItem;
    }else{
     //   qDebug()<<"rowCount() >> parent internal pointer";
        parentItem = static_cast<QDbcItem *>(parent.internalPointer());
    }
    return parentItem->rowCount();
}

int QDbcModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return 2;
}

Qt::ItemFlags QDbcModel::flags(const QModelIndex &index) const{
    if(!index.isValid() || !validModel){
     //   qDebug()<<"flags() >> Invalid index or model";
        return 0;
    }
   // qDebug()<<"flags() >> flags" << QAbstractItemModel::flags(index);
    return QAbstractItemModel::flags(index);
}

bool QDbcModel::setupModel(dbc_t *database){
    bool ret = true;
    if((database != 0)&& rootItem != nullptr){
        qDebug()<<"setupModel() >> Begin building model";
        int class_row = 0;
        for(message_list_t *itr = database->message_list; (itr!= 0)&&(ret == true); itr = itr->next){
            QString sender;
            if(!itr->message){
                qDebug()<<"setupModel() >> null message";
                continue;
            }
            if(!itr->message->sender){
                qDebug()<<"setupModel() >> null sender";
                sender = QString("Non-specific sender");
            }
            else{
                sender = QString(itr->message->sender);
                qDebug()<<"setupModel() >> sender " << sender;
            }

            QDbcItem *parent = rootItem->getChildbyName(sender);
            if(parent == nullptr){
                qDebug()<<"setupModel() >> new sender";
                parent = new QDbcItem(CLASS_TYPE,class_row,rootItem);
                parent->setupItem(itr->message->sender);
                rootItem->addChild(parent);
                class_row++;
                qDebug()<<"setupModel() >> class row " << class_row;
            }
            qDebug()<<"setupModel() >> message " << itr->message->id << QString(itr->message->name);
            QDbcItem *message = new QDbcItem(MESSAGE_TYPE,itr->message->id,parent);
            if(message == nullptr){
                qDebug()<<"setupModel() >> failed in building message";
                ret = false;
                break;
            }
            message->setupMessage(itr->message);
            parent->addChild(message);
            int signal_row = 0;
            for(signal_list_t *signal_itr = itr->message->signal_list; signal_itr!=0; signal_itr = signal_itr->next){
                QDbcItem *signal = new QDbcItem(SIGNAL_TYPE,signal_row,message);
                if(signal == nullptr){
                    qDebug()<<"setupModel() >> failed in building signal";
                    ret = false;
                    break;
                }
                signal->setupSignal(signal_itr->signal);
                message->addChild(signal);
                signal_row++;
                qDebug()<<"setupModel() >> signal row " <<signal_row;
            }
        }
    }
    else{
        ret = false;
    }
    return ret;
}

dbc_t* QDbcModel::getDatabase() const{
    return database;
}

QString QDbcModel::getFilename() const{
    return rootItem->name;
}

QDbcItem* QDbcModel::getRootItem() const{
    return rootItem;
}
