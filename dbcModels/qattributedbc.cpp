#include "qattributedbc.h"

QAttributeDbc::QAttributeDbc(attribute_t *attribute)
{
    this->attribute = attribute;
    if(attribute !=0){
        value_type = QVariant(attribute->value->value_type);
        switch(attribute->value->value_type){
        case vt_integer:
        case vt_enum:
        case vt_hex:
            value = QVariant(static_cast<int>(attribute->value->value.int_val));
            break;
        case vt_float:
            value = QVariant(static_cast<float>(attribute->value->value.double_val));
            break;
        case vt_string:
            value = QVariant(QString(attribute->value->value.string_val));
            break;
        default:
            value = QVariant(0);
            break;
        };
        name = QString(attribute->name);
    }
    else{
        value_type = QVariant(0);
        value = QVariant(0);
        name = QString(" ");
    }
}
