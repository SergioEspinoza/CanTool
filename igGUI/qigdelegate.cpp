#include "qigdelegate.h"
#include <QDebug>
#include <QLineEdit>
#include <QListView>
#include <QHash>
#include <QSpinBox>
#include <QCheckBox>
#include "igGUI/qigitem.h"
#include "widgets/qdatafield.h"
#include "widgets/qtransmissionsetup.h"
#include <QPainter>
static QWidget *viewport_addr; 
QIGDelegate::QIGDelegate(QObject *parent, QWidget *viewport)
    :QStyledItemDelegate(parent){
    viewport_addr = viewport;
}

QWidget* QIGDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    int c = index.column();
    QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
    if(index.isValid()){
        bool type = item->data(0).toBool();
        if(type){
            if(c == 0){
                QLineEdit *editor = new QLineEdit(parent);
                editor->setFrame(false);
                return editor;
            }
            else if(c ==1){
                QLineEdit *editor = new QLineEdit(parent);
                editor->setFrame(false);
                return editor;
            }
            else if (c==2){
                QSpinBox *editor = new QSpinBox(parent);
                editor->setMinimum(0);
                editor->setMaximum(8);
                editor->setFrame(false);
                return editor;
            }
            else if (c==3){
                QTransmissionSetup *editor = new QTransmissionSetup(parent);
                editor->resize(option.rect.size());
                return editor;

            }
            else if (c==4){
                int value = item->data(3).toInt();
                if (value >0){
                    QDataField *editor = new QDataField(parent, 8);
                    editor->setEnable(value);
                    editor->resize(option.rect.size());
                    return editor;
                }
                return nullptr;
            }
        }
        else{
            if(c == 0){
                QLineEdit *editor = new QLineEdit(parent);
                editor->setFrame(false);
                return editor;
            }
            else if(c ==1){
                QLineEdit *editor = new QLineEdit(parent);
                editor->setFrame(false);
                return editor;
            }
        }
    }
    return QStyledItemDelegate::createEditor(parent,option,index);
}
void QIGDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const{
    int c = index.column();
    QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
    if(index.isValid()){
        bool type = item->data(0).toBool();
        if(type){
            if(c==0){
                QString value = item->data(c+1).toString();
                QLineEdit *widget  = static_cast<QLineEdit*>(editor);
                widget->setText(value);
            }
            else if(c==1){
                QString value = item->data(c+1).toString();
                QLineEdit *widget  = static_cast<QLineEdit*>(editor);
                widget->setText(value);
            }
            else if(c==2){
                int value = item->data(c+1).toInt();
                QSpinBox *widget = static_cast<QSpinBox*>(editor);
                widget->setValue(value);
            }
            else if(c==3){
                int value = item->data(c+1).toInt();
                QTransmissionSetup *widget = static_cast<QTransmissionSetup *>(editor);
                widget->setData(value);
            }
            else if(c==4){
                QByteArray value(item->data(c+1).toByteArray());
                QDataField *widget = static_cast<QDataField*>(editor);
                widget->setData(value);
            }
        }
        else{
            if(c==0){
                QString value = item->data(c+1).toString();
                QLineEdit *widget  = static_cast<QLineEdit*>(editor);
                widget->setText(value);
            }
            else if (c==1){

            }
        }
    }
}
void QIGDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    Q_UNUSED(model);
    int c = index.column();
    QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
    if(index.isValid()){
        bool type = item->data(0).toBool();
        if(type){
            if(c==0){
                QLineEdit *widget  = static_cast<QLineEdit*>(editor);
                QString value = widget->text();
                item->setData(c+1,value);
            }
            else if(c==1){
                QLineEdit *widget  = static_cast<QLineEdit*>(editor);
                QString value = widget->text();
                item->setData(c+1,value);
            }
            else if(c==2){
                QSpinBox *widget = static_cast<QSpinBox*>(editor);
                widget->interpretText();
                int value = widget->value();
                item->setData(c+1,value);
            }
            else if(c==3){
                QTransmissionSetup *widget = static_cast<QTransmissionSetup *>(editor);
                int value = widget->data();
                item->setData(c+1,value);
            }
            else if(c==4){
                QDataField *widget = static_cast<QDataField*>(editor);
                QByteArray value(widget->getData());
                item->setData(c+1,QVariant(value));
                qDebug() << "QIGDelegate >> setModelData Datafield"; 
            }
        }
        else{
            if(c==0){
                QLineEdit *widget  = static_cast<QLineEdit*>(editor);
                QString value = widget->text();
                item->setData(c+1,value);
            }
            else if(c==1){

            }
        }
    }
}
void QIGDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const{
    editor->setGeometry(option.rect);
}

void QIGDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    int c = index.column();
    QIGItem *item = static_cast<QIGItem*>(index.internalPointer());
    if(index.isValid()){
        bool type = item->data(0).toBool();
        painter->save(); 
        qDebug() << "QIGDelegate paint >> option.state" << QString(QString::number(option.state,16));        
        if(type){
            if (option.state & QStyle::State_Selected){
                painter->fillRect(option.rect, option.palette.highlight());
            }
            if (c == 3){
                int value = item->data(c+1).toInt();
                QTransmissionSetup widget(viewport_addr);
                widget.setData(value);
                widget.resize(option.rect.size());
                painter->translate(option.rect.topLeft());
                widget.render(painter,QPoint(),QRegion(),
                              QWidget::DrawChildren);
            }
            else if (c==4){
                QByteArray value(item->data(c+1).toByteArray());
                QDataField widget(viewport_addr,8);
                int enable = item->data(3).toInt();
                widget.setEnable(enable);
                widget.setData(value);
                widget.resize(option.rect.size());
                painter->translate(option.rect.topLeft());
                widget.render(painter, QPoint(),QRegion(),
                              QWidget::DrawChildren);
            }   
            else{ 
                QStyledItemDelegate::paint(painter, option, index);
            }
        }
        else{
            QStyledItemDelegate::paint(painter, option, index);
        }
        painter->restore();
    }
}

QSize QIGDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{
    if (index.column()==3){
        return QSize(50,25);
    }
    else{
        return QStyledItemDelegate::sizeHint(option,index);
    }
}
