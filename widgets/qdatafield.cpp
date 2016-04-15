#include "qdatafield.h"
#include <QDebug>
QDataField::QDataField(QWidget *parent, unsigned int n) :
    QWidget(parent)
{
    this->setAutoFillBackground(true);
    if (n > sizeof(int)*8){
        n = sizeof(int)*8;
    }
    n_fields = n;
    layout = new QHBoxLayout();
    QLabel* msb = new QLabel(QString("%1").arg(n-1),this);
    QLabel* lsb = new QLabel(QString("0"),this);
    layout->addWidget(msb);
    for (unsigned int i = 0; i < n; i++){
        QLineEdit *line_edit = new QLineEdit("0x00",this);
        line_edit->setAlignment(Qt::AlignHCenter);
        layout->addWidget(line_edit);
    }
    layout->addWidget(lsb);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
}
QDataField::~QDataField(){
    for (QObject *item : layout->children()){
        delete item;
    }
    delete layout;
}

QByteArray QDataField::getData() const{
    QByteArray data(n_fields+1,'\0');
    data.clear();
    bool ok;
    for (int i = 0; i < n_fields; i++){
        QLineEdit* line = static_cast<QLineEdit*>(layout->itemAt(i+1)->widget());
        if(line->isEnabled()){
            unsigned int value = line->text().toUInt(&ok,0);
          //  qDebug()<< "datafield -> getData()" << line->text() << value;
            data[i] = (value&0xFF);
        }
    }
    return data;
}

void QDataField::setData(QByteArray data){
    for (int i = 0; i < n_fields; i++){
        QLineEdit* line = static_cast<QLineEdit*>(layout->itemAt(i+1)->widget());
        if(line->isEnabled()){
            QString value = "0x";
            unsigned char n = (data.at(i));
            value.append(QString::number(n/16,16));
            value.append(QString::number(n%16,16));
            value.append(0);
         //   qDebug() << "datafield >> setData" << value << i;
            line->setText(value);
        }
    }
}

void QDataField::setEnable(int n){
  //  qDebug() << "QDataField set enable"<< n << n_fields;
    if ((n>=0) && (n<n_fields)){
        int n_disable = n_fields - n;
        for (int i = 0; i < n_fields; i++){
            bool enable = n_disable>0?0:1;
            n_disable--;
            layout->itemAt(i+1)->widget()->setEnabled(enable);
            layout->itemAt(i+1)->widget()->setVisible(enable);
        }
        if (n){
            static_cast<QLabel*>(layout->itemAt(0)->widget())->setText(QString("%1").arg(n-1));
        }
        else {
            static_cast<QLabel*>(layout->itemAt(0)->widget())->setText(QString(" "));            
        }
    }
}
