#include "PQManager.h"

ConnectionPool* ConnectionPool::sInstance = nullptr;

/*PGconn* ConnectionPool::Get()->getConnection()
{
    PGconn* pg = PQconnectdb("host=127.0.0.1 port=5432 dbname=cars user=postgres password=Narek_28");
    return pg;
}

void ConnectionPool::Get()->releaseConnection(PGconn* pConn)
{
    PQfinish(pConn);
}*/
