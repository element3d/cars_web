#include "PQManager.h"

ConnectionPool* ConnectionPool::sInstance = nullptr;

PGconn* GetPQConnection()
{
    PGconn* pg = PQconnectdb("host=127.0.0.1 port=5432 dbname=cars user=postgres password=Narek_28");
    return pg;
}

void CloseConnection(PGconn* pConn)
{
    PQfinish(pConn);
}
