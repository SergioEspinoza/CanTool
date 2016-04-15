#ifndef QTRANSMISSIONSETUP_H
#define QTRANSMISSIONSETUP_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

class QTransmissionSetup : public QWidget
{
    Q_OBJECT
public:
    explicit QTransmissionSetup(QWidget *parent = 0);
    ~QTransmissionSetup();
    int data() const;
    void setData(int value);

private:
    QHBoxLayout *layout;
    QSlider *slider;
    QPushButton *execute;
    QSpinBox *period;
    bool run;
private slots:
    void sliderTrigger();
    void spinboxTrigger(int value);
    void buttonTrigger();
};

#endif // QTRANSMISSIONSETUP_H
