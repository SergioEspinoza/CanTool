#include "qtransmissionsetup.h"
#include <QDebug>
QTransmissionSetup::QTransmissionSetup(QWidget *parent) :
    QWidget(parent)
{
    this->setAutoFillBackground(true);
    run = false;
    layout = new QHBoxLayout(parent);
    slider = new QSlider(Qt::Horizontal,this);
    period = new QSpinBox(this);
    period->setValue(0);
    slider->setValue(0);
    period->setMaximum(5000);
    period->setMinimum(0);
    slider->setMaximum(500);
    slider->setMinimum(0);
    slider->setMinimumWidth(100);
    period->setSingleStep(10);
    period->setFixedWidth(50);
    period->setFixedHeight(20);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(20);
    slider->setTracking(true);
    execute = new QPushButton("Send", this);
    connect(slider,SIGNAL(sliderReleased()),this,SLOT(sliderTrigger()));
    connect(period, SIGNAL(valueChanged(int)),this, SLOT(spinboxTrigger(int)));
    connect(execute,SIGNAL(clicked()),this, SLOT(buttonTrigger()));
    layout->addWidget(period);
    layout->addWidget(slider);
    layout->addWidget(execute);
    layout->setSpacing(20);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);
}

QTransmissionSetup::~QTransmissionSetup(){
    delete slider;
    delete period;
    delete execute;
    delete layout;
}

void QTransmissionSetup::sliderTrigger(){
    period->setValue(slider->value()*10);
    qDebug() << "QTransmissionSetup -> slider value" << slider->value();
    run = false;
    if (!(slider->value())){
        execute->setText(QString("Send"));
    }else{
        execute->setText("Start");
    }
}

void QTransmissionSetup::spinboxTrigger(int value){
    slider->setValue(value/10);
    qDebug() << "QTransmissionSetup -> spinbox value" << period->value();
    run = false;
    if (!value){
        execute->setText(QString("Send"));
    }else{
        execute->setText("Start");
    }
}

void QTransmissionSetup::buttonTrigger(){
    if (period->value()){
        if(!run){
            period->setEnabled(false);
            slider->setEnabled(false);
            run = true;
            //TODO: START PERIODIC TRANSMISSION
            execute->setText("Stop");

        }
        else{
            period->setEnabled(true);
            slider->setEnabled(true);
            run = false;
            //TODO: STOP PERIODIC TRANSMISSION
            execute->setText("Start");
        }
    }
    else{
        //TODO: SINGLE TRANSMISSION.
    }
}

int QTransmissionSetup::data() const{
    return period->value();
}

void QTransmissionSetup::setData(int value){
    period->setValue(value);
    slider->setValue(value/10);
}
