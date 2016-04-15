#ifndef DBVIEWER_H
#define DBVIEWER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QString>
class DBViewer : public QWidget
{
    Q_OBJECT
public:
    explicit DBViewer(QWidget *parent = 0);
//    ~DBViewer();
//    void setModel(QAbstractItemModel *model);
//    void setupSpinBox(QStringList options);
//    void appendtoSpinBox(QString option);
//    bool removefromSpinBox(QString option);

signals:

public slots:
private:
    QVBoxLayout *DBLayout;
    QSpinBox    *DBSelector;
    QTreeView   *DBView;
    QLineEdit   *DBSearch;
};

#endif // DBVIEWER_H
