#include "MessagesManager.h"
#include "PQManager.h"

MessagesManager* MessagesManager::sInstance = nullptr;

MessagesManager* MessagesManager::Get() 
{
    if (!sInstance) sInstance = new MessagesManager();
    return sInstance;
}

bool MessagesManager::MessagesGet(int from, int to, rapidjson::Document& d)
{
    std::string sql = "SELECT id FROM conversations WHERE (user1 = "
        + std::to_string(from) + " AND user2 = " + std::to_string(to) + ") OR ( user1 = " 
        + std::to_string(to) + " AND user2 = " + std::to_string(from) + ");";


    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        fprintf(stderr, "Get messages failed: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return false;
    }

    if (PQntuples(res) < 1) 
    {
        return false;    
    }

    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    PQclear(res);
    // free(temp);

    sql = "SELECT * FROM messages WHERE conv_id = " + std::to_string(id) + ";";
    res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        fprintf(stderr, "Get messages failed: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return false;
    }
    d.SetObject();
    d.AddMember("id", id, d.GetAllocator());

    rapidjson::Value vMessages;
    vMessages.SetArray();
    int rec_count = PQntuples(res);
	for (int i = 0; i < rec_count; i++)
	{
        rapidjson::Value v;
        v.SetObject();
		strcpy(temp, PQgetvalue(res, i, 0));
		int mid = atoi(temp);
        v.AddMember("id", mid, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 1));
		int convId = atoi(temp);
        v.AddMember("conv_id", convId, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 2));
		int f = atoi(temp);
        v.AddMember("from", f, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 3));
		int t = atoi(temp);
        v.AddMember("to", t, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 4));
		std::string msg = temp;
        rapidjson::Document vMsg;
        vMsg.SetString(msg.c_str(), d.GetAllocator());
        v.AddMember("msg", vMsg, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 5));
		int status = atoi(temp);
        v.AddMember("status", status, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 6));
		int type = atoi(temp);
        v.AddMember("type", type, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 7));
		long long ts = atoll(temp);
        v.AddMember("ts", ts, d.GetAllocator());

        vMessages.PushBack(v, d.GetAllocator());
    }
    d.AddMember("messages", vMessages, d.GetAllocator());
    free(temp);
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
    return true;
}

int MessagesManager::MessagesPost(int convId, int from, int to, const std::string& msg, int type)
{
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    int newConvId = convId;
    if (convId < 0) 
    {
        newConvId = _CreateConversation(from, to);
    }

    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    std::string sql = "INSERT INTO messages(conv_id, from_user_id, to_user_id, msg, msg_type, status, msg_ts) VALUES ("
	    + std::to_string(newConvId) + ", "
	    + std::to_string(from) + ", "
        + std::to_string(to) + ", '"
        + msg + "', "
        + std::to_string(type) + ", "
        + std::to_string(0) + ", "
        + std::to_string(ms)
	    + ") RETURNING id;";

    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        fprintf(stderr, "Create message failed: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return -1;
	}
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
    return id;
    
}

void MessagesManager::MessagesSetImage(int msgId, const std::string& imagePath)
{
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    std::string sql = "UPDATE messages set msg = '" + imagePath + "' WHERE id = " + std::to_string(msgId) + ";";
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Update message image failed: %s", PQerrorMessage(pConn));
	    PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return;
    }

    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
}

bool MessagesManager::ConversationsGet(int from, rapidjson::Document& d)
{
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    std::string sql = "SELECT * from conversations where user1 = " + std::to_string(from) 
        + " OR user2 = " + std::to_string(from) + " ORDER BY create_ts DESC;";

    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        fprintf(stderr, "Get conversations failed: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return -1;
	}

    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);

    d.SetArray();
    for (int i = 0; i < rec_count; i++)
	{
        strcpy(temp, PQgetvalue(res, i, 0));
		int convId = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 1));
		int user1 = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 2));
		int user2 = atoi(temp);

        int userId;
        if (from == user1) userId = user2;
        else userId = user1;
        sql = "SELECT first_name, avatar from users WHERE id = " + std::to_string(userId) + ";";
        PGresult* pUsersRes = PQexec(pConn, sql.c_str());
        strcpy(temp, PQgetvalue(pUsersRes, 0, 0));
		std::string firstName = temp;

        strcpy(temp, PQgetvalue(pUsersRes, 0, 1));
		std::string avatar = temp;
        PQclear(pUsersRes);

        rapidjson::Value v;
        v.SetObject();
        v.AddMember("id", convId, d.GetAllocator());
        v.AddMember("user_id", userId, d.GetAllocator());
        rapidjson::Value fnv;
        fnv.SetString(firstName.c_str(), d.GetAllocator());
        rapidjson::Value av;
        av.SetString(avatar.c_str(), d.GetAllocator());
        v.AddMember("first_name", fnv, d.GetAllocator());
        v.AddMember("avatar", av, d.GetAllocator());
        d.PushBack(v, d.GetAllocator());
    }
    ConnectionPool::Get()->releaseConnection(pConn);
    PQclear(res);
    free(temp);
    return true;
}

int MessagesManager::ConversationsPost(int from, int to)
{
    return _CreateConversation(from, to);   
}

int MessagesManager::_CreateConversation(int from, int to)
{
    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    std::string sql = "INSERT INTO conversations(user1, user2, create_ts) VALUES ("
	        + std::to_string(from) + ", "
	        + std::to_string(to) + ", "
            + std::to_string(ms)
	        + ") RETURNING id;";

    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        fprintf(stderr, "Create conversation failed: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return -1;
	}

    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    PQclear(res);
    free(temp);
    ConnectionPool::Get()->releaseConnection(pConn);
    return id;
}
