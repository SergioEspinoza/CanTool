#ifndef QDATAFIELD_H
#define QDATAFIELD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QList>
#include <QLineEdit>
#include <QLabel>
#include <QByteRef>
class QDataField : public QWidget
{
    Q_OBJECT
public:
    explicit QDataField(QWidget *parent = 0, unsigned int n = 8);
    ~QDataField();
    QByteArray getData() const;
    void setData(QByteArray data);
    void setEnable(int n);
signals:

public slots:

private:
    int n_fields;
    QHBoxLayout *layout;
};

#endif // QDATAFIELD_H
