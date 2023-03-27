#ifndef __PQ_MANAGER__
#define __PQ_MANAGER__

#include <libpq-fe.h>

PGconn* GetPQConnection();
void CloseConnection(PGconn* pConn);

#endif // __PQ_MANAGER__
