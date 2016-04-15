#ifndef INCLUDE_DBCWRITER_H
#define INCLUDE_DBCWRITER_H

#include "libcandbc/dbcModel.h"

#ifdef __cplusplus
extern "C"
{
#endif

//void __declspec(dllimport) dbc_write(FILE *out, dbc_t *dbc);
//void dbc_write(FILE *out, dbc_t *dbc);
Q_DECL_IMPORT void dbc_write(FILE *out, dbc_t *dbc);

#ifdef __cplusplus
}
#endif //extern "C"

#endif
