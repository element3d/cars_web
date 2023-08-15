#include "EventsManager.h"
#include "PQManager.h"

#define DB_INCEPTION_ID 1

EventsManager* EventsManager::sInstance = nullptr;

EventsManager* EventsManager::Get()
{
    if (!sInstance) sInstance = new EventsManager();

    return sInstance;
}

DBInception* EventsManager::GetInception() 
{
    std::string sql = "select user_id from events_users where event_id = " + std::to_string(DB_INCEPTION_ID) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Failed to get Inception user count: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }

    char* temp = (char*)calloc(256, sizeof(char));

    DBInception* pI = new DBInception();
    int numUsers = PQntuples(res);
    for (int i = 0; i < numUsers; i++)
	  {
        strcpy(temp, PQgetvalue(res, i, 0));
        int userId = atoi(temp);
        pI->Users.push_back(userId);
    }

    free(temp);
    
    pI->NumUsers = numUsers;
    pI->Status = EEventStatus::Started;
    pI->RemainingNumUsers = 10 - numUsers;
    ConnectionPool::Get()->releaseConnection(pg);
    return pI;
}

void EventsManager::InceptionAddUser(int userId)
{
    std::string sql = "INSERT INTO events_users(user_id, event_id) VALUES(" + std::to_string(userId) + "," + std::to_string(DB_INCEPTION_ID) + ");";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Failed to add Inception user: %s", PQerrorMessage(pg));
    }

    PQclear(res);    
    ConnectionPool::Get()->releaseConnection(pg);
    return;
}