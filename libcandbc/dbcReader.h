#ifndef INCLUDE_DBCREADER_H
#define INCLUDE_DBCREADER_H

//#ifdef __cplusplus
//extern "C"
//{
//#endif

#include "libcandbc/dbcModel.h"

////dbc_t __declspec(dllimport) *dbc_read_file(char *filename);
////dbc_t* dbc_read_file(char *filename);
//Q_DECL_IMPORT dbc_t *dbc_read_file(const char *filename);
dbc_t *dbc_read_file(char *filename);



//#ifdef __cplusplus
//}
//#endif //extern "C"

//#endif // __cplusplus
#endif
