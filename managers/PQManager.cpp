#include "PQManager.h"

PGconn* GetPQConnection()
{
    PGconn* pg = PQconnectdb("host=127.0.0.1 port=5432 dbname=cars user=postgres password=postgres");
    return pg;
}

void CloseConnection(PGconn* pConn)
{
    PQfinish(pConn);
}
