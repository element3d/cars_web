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
    std::string sql = "select user_id, address, card from events_users where event_id = " + std::to_string(DB_INCEPTION_ID) + ";";
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
        EventUser u;
        strcpy(temp, PQgetvalue(res, i, 0));
        u.Id = atoi(temp);
        strcpy(temp, PQgetvalue(res, i, 1));
        u.Address = temp;
        strcpy(temp, PQgetvalue(res, i, 2));
        u.Card = temp;
        pI->Users.push_back(u);
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

void EventsManager::EventsSetUser(int userId, int eventId, const std::string& userJson)
{
    rapidjson::Document d;
    d.Parse(userJson.c_str());

    const std::string fName = d["first_name"].GetString();
    const std::string lName = d["last_name"].GetString();
    const std::string province = d["province"].GetString();
    const std::string address = d["address"].GetString();

    const std::string fullAddress = fName + ", " + lName + ", " + province + ", " + address;
    std::string sql = "UPDATE events_users SET address = '" + fullAddress + "' WHERE user_id = " + std::to_string(userId) + " AND event_id = " + std::to_string(eventId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Failed to add user info to event: %s", PQerrorMessage(pg));
    }

}

void EventsManager::EventsSetUserCard(int userId, int eventId, const std::string& cardJson)
{
    rapidjson::Document d;
    d.Parse(cardJson.c_str());

    const std::string card = d["card"].GetString();
 
    std::string sql = "UPDATE events_users SET card = '" + card + "' WHERE user_id = " + std::to_string(userId) + " AND event_id = " + std::to_string(eventId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Failed to add user card to event: %s", PQerrorMessage(pg));
    }

}