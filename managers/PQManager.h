#ifndef __PQ_MANAGER__
#define __PQ_MANAGER__

#include <libpq-fe.h>
#include <vector>
#include <mutex>

class ConnectionPool {
public:
    static ConnectionPool* Get() 
    {
      if (!sInstance) sInstance = new ConnectionPool("host=127.0.0.1 port=5432 dbname=cars user=postgres password=Narek_28", 20);
      return sInstance;
    }

    ConnectionPool(const char* connString, int poolSize) : connString(connString), poolSize(poolSize) {
        connections.reserve(poolSize);
        for (int i = 0; i < poolSize; ++i) {
            PGconn* conn = PQconnectdb(connString);
            if (PQstatus(conn) == CONNECTION_OK) {
                connections.push_back(conn);
            } else {
                // Handle connection error
                // You might want to log the error or take appropriate action
                PQerrorMessage(conn);
                PQfinish(conn);
            }
        }
    }

    ~ConnectionPool() {
        for (PGconn* conn : connections) {
            PQfinish(conn);
        }
    }

    PGconn* getConnection() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!connections.empty()) {
            PGconn* conn = connections.back();
            if (!conn) {
            printf("asdf");
            }
            connections.pop_back();
            return conn;
        }
        // Handle the case when no connection is available in the pool
        // You might want to wait or throw an error based on your use case
        return nullptr;
    }

    void releaseConnection(PGconn* conn) {
        std::lock_guard<std::mutex> lock(mutex);
        connections.push_back(conn);
    }

private:
    const char* connString;
    int poolSize;
    std::vector<PGconn*> connections;
    std::mutex mutex;
    static ConnectionPool* sInstance;
};

/*PGconn* ConnectionPool::Get()->getConnection();
void ConnectionPool::Get()->releaseConnection(PGconn* pConn);*/

#endif // __PQ_MANAGER__
