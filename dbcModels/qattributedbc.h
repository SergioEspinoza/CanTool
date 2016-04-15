#ifndef QATTRIBUTEDBC_H
#define QATTRIBUTEDBC_H

#include <QString>
#include <QVariant>
#include <dbcModel.h>
class QAttributeDbc
{
public:
    explicit QAttributeDbc(attribute_t *attribute);
    attribute_t *attribute;
    QVariant value;
    QVariant value_type;
    QString  name;
};

#endif // QATTRIBUTEDBC_H
