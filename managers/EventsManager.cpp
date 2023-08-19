#include "EventsManager.h"
#include "PQManager.h"

#define DB_INCEPTION_ID 1
#define DB_PITSTOP_ID 2

EventsManager* EventsManager::sInstance = nullptr;

EventsManager* EventsManager::Get()
{
    if (!sInstance) sInstance = new EventsManager();

    return sInstance;
}

bool EventsManager::GetEvents(int userId, std::vector<DBEvent*>& events)
{
    std::string sql = "select id, title, status, value1, value2, image, limits from events ORDER BY status ASC;";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Failed to get events: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }

    int numEvents = PQntuples(res);
    char* temp = (char*)calloc(4096, sizeof(char));
	  for (int i = 0; i < numEvents; i++)
	  {
        DBEvent* pEvent = new DBEvent();
		    strcpy(temp, PQgetvalue(res, i, 0));
		    pEvent->Id = atoi(temp);

		    strcpy(temp, PQgetvalue(res, i, 1));
		    pEvent->Title = temp;

        strcpy(temp, PQgetvalue(res, i, 2));
		    pEvent->Status = (EEventStatus)atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 3));
		    pEvent->Value1 = temp;

        strcpy(temp, PQgetvalue(res, i, 4));
		    pEvent->Value2 = temp;

        strcpy(temp, PQgetvalue(res, i, 5));
		    pEvent->Image = temp;

        strcpy(temp, PQgetvalue(res, i, 6));
		    pEvent->Limits = atoi(temp);

        sql = "select user_id, address, card, awarded from events_users where event_id = " + std::to_string(pEvent->Id) + ";";
        PGresult* pRes = PQexec(pg, sql.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            fprintf(stderr, "Failed to get Inception user count: %s", PQerrorMessage(pg));
            PQclear(pRes);
            continue;
        }

        int numUsers = PQntuples(pRes);
        for (int j = 0; j < numUsers; j++)
	      {
            EventUser u;
            strcpy(temp, PQgetvalue(pRes, j, 0));
            u.Id = atoi(temp);
            strcpy(temp, PQgetvalue(pRes, j, 1));
            u.Address = temp;
            strcpy(temp, PQgetvalue(pRes, j, 2));
            u.Card = temp;
            strcpy(temp, PQgetvalue(pRes, j, 3));
            u.Awarded = atoi(temp);

            pEvent->Users.push_back(u);
        }
        pEvent->RemainingNumUsers = pEvent->Limits - numUsers;
        PQclear(pRes);

        if (userId > 0 && pEvent->Id == DB_PITSTOP_ID) 
        {
            sql = "select count(*) from events_users where event_id = " + std::to_string(DB_INCEPTION_ID) + " and user_id = " + std::to_string(userId) + ";";
            PGresult* pRes = PQexec(pg, sql.c_str());
            if (PQresultStatus(res) != PGRES_TUPLES_OK)
            {
                fprintf(stderr, "Failed to get Inception user count: %s", PQerrorMessage(pg));
                PQclear(pRes);
                continue;
            }
            strcpy(temp, PQgetvalue(pRes, 0, 0));
            bool exists = atoi(temp) > 0;
            if (exists) pEvent->Access = EEventAccess::Denied;
            else pEvent->Access = EEventAccess::Allowed;
        }


        events.push_back(pEvent);
    }

    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

DBEvent* EventsManager::GetInception() 
{
    std::string sql = "select status from events where id = " + std::to_string(DB_INCEPTION_ID) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Failed to get Inception status: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }
    char* temp = (char*)calloc(256, sizeof(char));
    DBEvent* pI = new DBEvent();
    strcpy(temp, PQgetvalue(res, 0, 0));
    pI->Status = (EEventStatus)atoi(temp);
    PQclear(res);

    sql = "select user_id, address, card from events_users where event_id = " + std::to_string(DB_INCEPTION_ID) + ";";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Failed to get Inception user count: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }

   // char* temp = (char*)calloc(256, sizeof(char));

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
    // pI->Status = EEventStatus::Started;
    pI->RemainingNumUsers = 10 - numUsers;
    ConnectionPool::Get()->releaseConnection(pg);
    return pI;
}

DBEvent* EventsManager::GetPitStop(int userId)
{
    // check for inception
    std::string sql = "select count(*) from events_users where event_id = " + std::to_string(DB_INCEPTION_ID) + " AND user_id=" + std::to_string(userId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();

    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Failed to get Inception user count: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }
    
    DBEvent* pI = new DBEvent();
    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    bool exists = atoi(temp) != 0;
    if (exists) 
    {
      pI->Access = EEventAccess::Denied;
      PQclear(res);
      free(temp);
      ConnectionPool::Get()->releaseConnection(pg);
      return pI;
    }

    PQclear(res);
    sql = "select status from events where id = " + std::to_string(DB_PITSTOP_ID) + ";";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Failed to get Inception status: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }
    
   
    strcpy(temp, PQgetvalue(res, 0, 0));
    pI->Status = (EEventStatus)atoi(temp);
    PQclear(res);

    sql = "select user_id, address, card from events_users where event_id = " + std::to_string(2) + ";";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Failed to get Inception user count: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }

    // char* temp = (char*)calloc(256, sizeof(char));

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
    // pI->Status = EEventStatus::Started;
    pI->RemainingNumUsers = 100 - numUsers;
    ConnectionPool::Get()->releaseConnection(pg);
    return pI;
}

void EventsManager::PitStopAddUser(int userId)
{
    std::string sql = "INSERT INTO events_users(user_id, event_id) VALUES(" + std::to_string(userId) + "," + std::to_string(DB_PITSTOP_ID) + ");";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Failed to add Inception user: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return;
    }

    PQclear(res);
    sql = "SELECT count(*) FROM events_users WHERE event_id=" + std::to_string(DB_PITSTOP_ID) + ";";
    res = PQexec(pg, sql.c_str());
    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    int numUsers = atoi(temp);
    if (numUsers >= 100) 
    {
        sql = "UPDATE events SET status = 2 WHERE id=" + std::to_string(DB_PITSTOP_ID) + ";";
        PQclear(res);
        res = PQexec(pg, sql.c_str());        
    }

    PQclear(res);    
    ConnectionPool::Get()->releaseConnection(pg);
    return;
}

void EventsManager::InceptionAddUser(int userId)
{
    std::string sql = "INSERT INTO events_users(user_id, event_id) VALUES(" + std::to_string(userId) + "," + std::to_string(DB_INCEPTION_ID) + ");";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Failed to add Inception user: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return;
    }

    PQclear(res);
    sql = "SELECT count(*) FROM events_users WHERE event_id=" + std::to_string(DB_INCEPTION_ID) + ";";
    res = PQexec(pg, sql.c_str());
    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    int numUsers = atoi(temp);
    if (numUsers >= 10) 
    {
        sql = "UPDATE events SET status = 2 WHERE id=" + std::to_string(DB_INCEPTION_ID) + ";";
        PQclear(res);
        res = PQexec(pg, sql.c_str());
        PQclear(res);
        sql = "UPDATE events SET status = 0 WHERE id=" + std::to_string(DB_PITSTOP_ID) + ";";
        res = PQexec(pg, sql.c_str());
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
    std::string sql = "UPDATE events_users SET address = '" + fullAddress + "', awarded = 1 WHERE user_id = " + std::to_string(userId) + " AND event_id = " + std::to_string(eventId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Failed to add user info to event: %s", PQerrorMessage(pg));
    }
    PQclear(res);    
    ConnectionPool::Get()->releaseConnection(pg);
}

void EventsManager::EventsSetUserCard(int userId, int eventId, const std::string& cardJson)
{
    rapidjson::Document d;
    d.Parse(cardJson.c_str());

    const std::string card = d["card"].GetString();
 
    std::string sql = "UPDATE events_users SET card = '" + card + "', awarded = 1 WHERE user_id = " + std::to_string(userId) + " AND event_id = " + std::to_string(eventId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Failed to add user card to event: %s", PQerrorMessage(pg));
    }
    PQclear(res);    
    ConnectionPool::Get()->releaseConnection(pg);
}