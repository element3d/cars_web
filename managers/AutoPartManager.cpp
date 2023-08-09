#include "AutoPartManager.h"
#include <iostream>
#include "PQManager.h"

AutoPartManager* AutoPartManager::sInstance = nullptr;

AutoPartManager* AutoPartManager::Get()
{
	if (!sInstance) sInstance = new AutoPartManager();

	return sInstance;
}

//void AutoPartManager::SetPG(PGconn* pPG)
//{
//	mPG = pPG;
//}

int AutoPartManager::CreateAutoPart(int userId, const std::string& autoPartJson)
{
	rapidjson::Document d;
	d.Parse(autoPartJson.c_str());

    std::string sql = "INSERT INTO auto_part_requests(user_id, category, make, serie, model, description) VALUES ("
        + std::to_string(userId) + ", "
        + std::to_string(d["category"].GetInt()) + ", '"
        + d["make"].GetString() + "', '"
        + d["serie"].GetString() + "', '"
        + d["model"].GetString() + "', '"
        + d["description"].GetString() + "'"
		");";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
//		char* err = PQerrorMessage(pConn);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
		PQclear(res);
		return -1;
	}

    sql = "SELECT currval('auto_part_requests_id_seq');";
    res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
//		char* err = PQerrorMessage(mPG);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
		PQclear(res);
		return -1;
	}
	char* temp = (char*)calloc(256, sizeof(char));
	int rec_count = PQntuples(res);
	strcpy(temp, PQgetvalue(res, 0, 0));
	int id = atoi(temp);

    // get auto part users
    sql = "SELECT id FROM users WHERE type=1 ORDER BY random() LIMIT 10;";
    res = PQexec(pConn, sql.c_str());
    rec_count = PQntuples(res);
    for (int i = 0; i < rec_count; ++i)
    {
        strcpy(temp, PQgetvalue(res, i, 0));
        int resUserId = atoi(temp);
        sql = std::string("INSERT INTO auto_part_notes(req_user_id, res_user_id, req_id) VALUES(")
                + std::to_string(userId) + ", "
                + std::to_string(resUserId) + ", "
                + std::to_string(id)
                + ");";

        PGresult* r = PQexec(pConn, sql.c_str());
        PQclear(r);

    }
    PQclear(res);
    free(temp);
	return id;
}

int AutoPartManager::CreateAutoPartResponse(int userId, const std::string& autoPartResponseJson)
{
    rapidjson::Document d;
    d.Parse(autoPartResponseJson.c_str());

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    int reqId = -1;
    {
        std::string sql = "SELECT req_id FROM auto_part_notes WHERE id = "
            + std::to_string(d["note_id"].GetInt()) + ";";

        PGresult* res = PQexec(pConn, sql.c_str());
        int rec_count = PQntuples(res);
        if (!rec_count)
        {
            PQclear(res);
            ConnectionPool::Get()->releaseConnection(pConn);
            return -1;
        }

        char* temp = (char*)calloc(256, sizeof(char));
        strcpy(temp, PQgetvalue(res, 0, 0));
        reqId = atoi(temp);
        free(temp);
        PQclear(res);
    }

    std::string sql = "INSERT INTO auto_part_responses(note_id, make, original, new, price, price_type, description, req_id, user_id) VALUES ("
        + std::to_string(d["note_id"].GetInt()) + ", '"
        + d["make"].GetString() + "', "
        + std::to_string(d["original"].GetInt()) + ", "
        + std::to_string(d["new"].GetInt()) + ", '"
        + std::to_string(d["price"].GetInt()) + "', "
        + std::to_string(d["price_type"].GetInt()) + ", '"
        + d["description"].GetString() + "', "
        + std::to_string(reqId) + ", "
        + std::to_string(userId) + ""
        ");";

    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
//		char* err = PQerrorMessage(pConn);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return -1;
    }

    sql = "SELECT currval('auto_part_responses_id_seq');";
    res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
//		char* err = PQerrorMessage(mPG);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
        PQclear(res);
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

void AutoPartManager::DeleteAutoPartNotification(int id, int resUserId)
{
    std::string sql = "DELETE FROM auto_part_notes WHERE id = "
        + std::to_string(id) + " AND res_user_id =" + std::to_string(resUserId) + ";";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    PQclear(res);
    sql = "DELETE FROM auto_part_responses WHERE note_id = "
           + std::to_string(id) + ";";
    res = PQexec(pConn, sql.c_str());
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
}

void AutoPartManager::GetAutoPartResponse(int userId, int id,  rapidjson::Document& d)
{
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    std::string sql = "SELECT * FROM auto_part_responses WHERE id = "
        + std::to_string(id) + ";";
    PGresult* res = PQexec(pConn, sql.c_str());

    rapidjson::Value v;
    int uId;
    int reqId;
    {
         char* temp = (char*)calloc(256, sizeof(char));
         strcpy(temp, PQgetvalue(res, 0, 0));
         int id = atoi(temp);
         v.SetObject();
         v.AddMember("id", id, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 2));
         std::string make = temp;
         rapidjson::Value tmp;
         tmp.SetString(make.c_str(), d.GetAllocator());
         v.AddMember("make", tmp, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 3));
         int original = atoi(temp);
         v.AddMember("original", original, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 4));
         int neww = atoi(temp);
         v.AddMember("new", neww, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 5));
         int price = atoi(temp);
         v.AddMember("price", price, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 6));
         int priceType = atoi(temp);
         v.AddMember("price_type", priceType, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 7));
         std::string description = temp;
         tmp.SetString(description.c_str(), d.GetAllocator());
         v.AddMember("description", tmp, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 9));
         std::string ava = temp;
         tmp.SetString(ava.c_str(), d.GetAllocator());
         v.AddMember("avatar", tmp, d.GetAllocator());

         strcpy(temp, PQgetvalue(res, 0, 10));
         uId = atoi(temp);

         strcpy(temp, PQgetvalue(res, 0, 8));
         reqId = atoi(temp);

         PQclear(res);
    }
    d.SetObject();
    d.AddMember("response", v, d.GetAllocator());
    {
        sql = "SELECT phone, first_name, second_name, avatar FROM users WHERE id = "
            + std::to_string(uId) + ";";
        res = PQexec(pConn, sql.c_str());
        v.SetObject();

        char* temp = (char*)calloc(256, sizeof(char));
        strcpy(temp, PQgetvalue(res, 0, 0));
        std::string phone = temp;
        rapidjson::Value tmp;
        tmp.SetString(phone.c_str(), d.GetAllocator());
        v.AddMember("phone", tmp, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, 0, 1));
        std::string first_name = temp;
        tmp.SetString(first_name.c_str(), d.GetAllocator());
        v.AddMember("first_name", tmp, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, 0, 2));
        std::string second_name = temp;
        tmp.SetString(second_name.c_str(), d.GetAllocator());
        v.AddMember("second_name", tmp, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, 0, 3));
        std::string avatar = temp;
        tmp.SetString(avatar.c_str(), d.GetAllocator());
        v.AddMember("avatar", tmp, d.GetAllocator());
        d.AddMember("user", v, d.GetAllocator());
        PQclear(res);
    }

    {
        sql = "SELECT make, serie, model, category FROM auto_part_requests WHERE id = "
            + std::to_string(reqId) + ";";
        res = PQexec(pConn, sql.c_str());
        v.SetObject();

        char* temp = (char*)calloc(256, sizeof(char));
        strcpy(temp, PQgetvalue(res, 0, 0));
        std::string make = temp;
        rapidjson::Value tmp;
        tmp.SetString(make.c_str(), d.GetAllocator());
        v.AddMember("make", tmp, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, 0, 1));
        std::string serie = temp;
        tmp.SetString(serie.c_str(), d.GetAllocator());
        v.AddMember("serie", tmp, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, 0, 2));
        std::string model = temp;
        tmp.SetString(model.c_str(), d.GetAllocator());
        v.AddMember("model", tmp, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, 0, 3));
        int cat = atoi(temp);
        v.AddMember("category", cat, d.GetAllocator());
        d.AddMember("request", v, d.GetAllocator());
        PQclear(res);
    }

    ConnectionPool::Get()->releaseConnection(pConn);
}

void AutoPartManager::GetAutoPartNotification(int id, rapidjson::Document& d)
{
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    std::string sql = "UPDATE auto_part_notes SET STATUS = 1 WHERE id = "
        + std::to_string(id) + ";";
    PGresult* res = PQexec(pConn, sql.c_str());

    sql = "SELECT id, req_id FROM auto_part_notes WHERE id = "
        + std::to_string(id) + ";";
    PQclear(res);

    res = PQexec(pConn, sql.c_str());

    d.SetObject();
    int rec_count = PQntuples(res);
    if (!rec_count)
    {
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return;
    }

    {
         char* temp = (char*)calloc(256, sizeof(char));
         strcpy(temp, PQgetvalue(res, 0, 0));
         int id = atoi(temp);

         strcpy(temp, PQgetvalue(res, 0, 1));
         int reqId = atoi(temp);

         sql = "SELECT * FROM auto_part_requests WHERE id = "
                 + std::to_string(reqId) + ";";

         PGresult* r = PQexec(pConn, sql.c_str());
         std::vector<DBAutoPartRequest*> reqs;
         _ParseGPResult(r, reqs);
         DBAutoPartRequest* pAutoPart = reqs[0];

         rapidjson::Value v;
         v.SetObject();
         v.AddMember("id", id, d.GetAllocator());
         v.AddMember("category", pAutoPart->Category, d.GetAllocator());

         rapidjson::Value make;
         make.SetString(pAutoPart->Make.c_str(), d.GetAllocator());
         v.AddMember("make", make, d.GetAllocator());

         rapidjson::Value serie;
         serie.SetString(pAutoPart->Serie.c_str(), d.GetAllocator());
         v.AddMember("serie", serie, d.GetAllocator());

         rapidjson::Value model;
         model.SetString(pAutoPart->Model.c_str(), d.GetAllocator());
         v.AddMember("model", model, d.GetAllocator());

         rapidjson::Value description;
         description.SetString(pAutoPart->Description.c_str(), d.GetAllocator());
         v.AddMember("description", description, d.GetAllocator());

         rapidjson::Value avatar;
         avatar.SetString(pAutoPart->Avatar.c_str(), d.GetAllocator());
         v.AddMember("avatar", avatar, d.GetAllocator());
         d.AddMember("request", v, d.GetAllocator());

         free(temp);
    }
    PQclear(res);
    rapidjson::Value responses;
    responses.SetArray();
    {
        sql = "SELECT * FROM auto_part_responses WHERE note_id = "
                + std::to_string(id) + ";";

        PGresult* r = PQexec(pConn, sql.c_str());
        rec_count = PQntuples(r);
        if (!rec_count)
        {
             PQclear(r);
             ConnectionPool::Get()->releaseConnection(pConn);
             return;
        }
        char* temp = (char*)calloc(256, sizeof(char));

        for (int i = 0; i < rec_count; i++)
        {
            rapidjson::Value response;
            response.SetObject();
            strcpy(temp, PQgetvalue(res, i, 0));
            int resId = atoi(temp);
            response.AddMember("id", resId, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 1));
            int noteId = atoi(temp);
            response.AddMember("note_id", noteId, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 2));
            std::string make = temp;
            rapidjson::Value makeV;
            makeV.SetString(make.c_str(), d.GetAllocator());
            response.AddMember("make", makeV, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 3));
            int o = atoi(temp);
            response.AddMember("original", o, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 4));
            int n = atoi(temp);
            response.AddMember("new", n, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 5));
            std::string price = temp;
            rapidjson::Value priceV;
            priceV.SetString(price.c_str(), d.GetAllocator());
            response.AddMember("price", priceV, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 6));
            int pt = atoi(temp);
            response.AddMember("price_type", pt, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 7));
            std::string desc = temp;
            rapidjson::Value dv;
            dv.SetString(desc.c_str(), d.GetAllocator());
            response.AddMember("description", dv, d.GetAllocator());

            strcpy(temp, PQgetvalue(res, i, 9));
            std::string avatar = temp;
            rapidjson::Value av;
            av.SetString(avatar.c_str(), d.GetAllocator());
            response.AddMember("avatar", av, d.GetAllocator());
            responses.PushBack(response, d.GetAllocator());
        }
        free(temp);
        d.AddMember("responses", responses, d.GetAllocator());
    }

    ConnectionPool::Get()->releaseConnection(pConn);
}

void AutoPartManager::GetAutoPartNotifications(int userId, rapidjson::Document& d)
{
    std::string sql = "SELECT id, req_id, status FROM auto_part_notes WHERE res_user_id = "
        + std::to_string(userId) + ";";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());

    d.SetArray();
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    for (int i = 0; i < rec_count; i++)
    {
        strcpy(temp, PQgetvalue(res, i, 0));
        int id = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 1));
        int reqId = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 2));
        int status = atoi(temp);

        sql = "SELECT * FROM auto_part_requests WHERE id = "
                + std::to_string(reqId) + ";";

        PGresult* r = PQexec(pConn, sql.c_str());
        std::vector<DBAutoPartRequest*> reqs;
        _ParseGPResult(r, reqs);
        if (!reqs.size())
        {
            continue;
        }

        DBAutoPartRequest* pAutoPart = reqs[0];

        rapidjson::Value v;
        v.SetObject();
        v.AddMember("id", id, d.GetAllocator());
        v.AddMember("category", pAutoPart->Category, d.GetAllocator());
        v.AddMember("status", status, d.GetAllocator());

        rapidjson::Value make;
        make.SetString(pAutoPart->Make.c_str(), d.GetAllocator());
        v.AddMember("make", make, d.GetAllocator());

        rapidjson::Value serie;
        serie.SetString(pAutoPart->Serie.c_str(), d.GetAllocator());
        v.AddMember("serie", serie, d.GetAllocator());

        rapidjson::Value model;
        model.SetString(pAutoPart->Model.c_str(), d.GetAllocator());
        v.AddMember("model", model, d.GetAllocator());

        rapidjson::Value description;
        description.SetString(pAutoPart->Description.c_str(), d.GetAllocator());
        v.AddMember("description", description, d.GetAllocator());

        rapidjson::Value avatar;
        avatar.SetString(pAutoPart->Avatar.c_str(), d.GetAllocator());
        v.AddMember("avatar", avatar, d.GetAllocator());
        d.PushBack(v, d.GetAllocator());
    }
    free(temp);
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
}

void AutoPartManager::DeleteAutoPart(int userId, int autoPartId)
{
    std::string sql = "DELETE FROM auto_part_requests WHERE user_id = "
        + std::to_string(userId) + " AND id = " + std::to_string(autoPartId) + ";";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
//		char* err = PQerrorMessage(pConn);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        //exit_nicely(conn);
        return;
    }

    sql = "DELETE FROM auto_part_notes WHERE req_user_id = "
           + std::to_string(userId) + " AND req_id = " + std::to_string(autoPartId) + ";";
    PQclear(res);
    res = PQexec(pConn, sql.c_str());

    sql = "DELETE FROM auto_part_responses WHERE req_id = " + std::to_string(autoPartId) + ";";
    PQclear(res);
    res = PQexec(pConn, sql.c_str());

    ConnectionPool::Get()->releaseConnection(pConn);
}

void AutoPartManager::GetAutoPart(int userId, int autoPartId, rapidjson::Document& d)
{
    std::string sql = "SELECT * FROM auto_part_requests WHERE user_id = "
        + std::to_string(userId) + " AND id = " + std::to_string(autoPartId) + ";";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
//		char* err = PQerrorMessage(pConn);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        //exit_nicely(conn);
        return;
    }

    d.SetObject();
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);

    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    d.AddMember("id", id, d.GetAllocator());

    rapidjson::Value v;
    strcpy(temp, PQgetvalue(res, 0, 1));
    std::string make = temp;
    v.SetString(make.c_str(), d.GetAllocator());
    d.AddMember("make", v, d.GetAllocator());

    strcpy(temp, PQgetvalue(res, 0, 2));
    v.SetString(temp, d.GetAllocator());
    d.AddMember("serie", v, d.GetAllocator());

    strcpy(temp, PQgetvalue(res, 0, 3));
    v.SetString(temp, d.GetAllocator());
    d.AddMember("model", v, d.GetAllocator());

    strcpy(temp, PQgetvalue(res, 0, 4));
    v.SetString(temp, d.GetAllocator());
    d.AddMember("description", v, d.GetAllocator());

    strcpy(temp, PQgetvalue(res, 0, 5));
    d.AddMember("userId", atoi(temp), d.GetAllocator());

    strcpy(temp, PQgetvalue(res, 0, 6));
    d.AddMember("category", atoi(temp), d.GetAllocator());

    strcpy(temp, PQgetvalue(res, 0, 7));
    v.SetString(temp, d.GetAllocator());
    d.AddMember("avatar", v, d.GetAllocator());
   // free(temp);
    PQclear(res);

    sql = "SELECT * FROM auto_part_responses WHERE req_id = " + std::to_string(id) + ";";
    res = PQexec(pConn, sql.c_str());
    rec_count = PQntuples(res);
    rapidjson::Value responses;
    responses.SetArray();
    for (int i = 0; i < rec_count; ++i)
    {
        rapidjson::Value response;
        response.SetObject();

        strcpy(temp, PQgetvalue(res, i, 0));
        int id = atoi(temp);
        response.AddMember("id", id, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 2));
        v.SetString(temp, d.GetAllocator());
        response.AddMember("make", v, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 3));
        response.AddMember("original", atoi(temp), d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 4));
        response.AddMember("new", atoi(temp), d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 5));
        v.SetString(temp, d.GetAllocator());
        response.AddMember("price", v, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 6));
        response.AddMember("price_type", atoi(temp), d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 7));
        v.SetString(temp, d.GetAllocator());
        response.AddMember("description", v, d.GetAllocator());

        strcpy(temp, PQgetvalue(res, i, 9));
        v.SetString(temp, d.GetAllocator());
        response.AddMember("avatar", v, d.GetAllocator());

        responses.PushBack(response, d.GetAllocator());
    }
    d.AddMember("responses", responses, d.GetAllocator());

     free(temp);
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
}

void AutoPartManager::GetAutoParts(int userId, std::vector<DBAutoPartRequest*>& autoParts)
{
    std::string sql = "SELECT * FROM auto_part_requests WHERE user_id = "
		+ std::to_string(userId) + ";";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
//		char* err = PQerrorMessage(pConn);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}

	_ParseGPResult(res, autoParts);
	/*char* temp = (char*)calloc(256, sizeof(char));

	int rec_count = PQntuples(res);
	for (int i = 0; i < rec_count; i++)
	{

		DBAutoPart* pAutoPart = new DBAutoPart();
		strcpy(temp, PQgetvalue(res, i, 0));
		pAutoPart->Id = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 1));
		pAutoPart->UserId = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 2));
		pAutoPart->Country = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 3));
		pAutoPart->Province = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 4));
		pAutoPart->SubProvince = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 5));
		pAutoPart->Type = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 6));
		pAutoPart->Subtype = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 7));
		pAutoPart->Originality = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 8));
		pAutoPart->Condition = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 9));
		pAutoPart->Price = atof(temp);

		strcpy(temp, PQgetvalue(res, i, 11));
		pAutoPart->Avatar = temp;

		{
			std::string sql = "SELECT image_path FROM auto_part_images WHERE auto_part_id = "
				+ std::to_string(pAutoPart->Id) + ";";

			PGresult* res = PQexec(mPG, sql.c_str());
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				char* err = PQerrorMessage(mPG);
				fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
				PQclear(res);
				//exit_nicely(conn);
			}
			else
			{
				int imageCount = PQntuples(res);
				char* tmp = (char*)calloc(256, sizeof(char));
				for (int img = 0; img < imageCount; img++)
				{
					strcpy(tmp, PQgetvalue(res, img, 0));
					pAutoPart->Images.push_back(std::string(tmp));
				}
				free(tmp);
			}
			PQclear(res);
		}

		autoParts.push_back(pAutoPart);
	}
	free(temp);
	PQclear(res);*/
}

void AutoPartManager::GetTopAutoPartsByType(int type, std::vector<DBAutoPart*>& autoParts)
{
	std::string sql;
	if (type == -1)
		sql = "SELECT * FROM auto_parts;";
	else
		sql = "SELECT * FROM auto_parts WHERE type = " + std::to_string(type) + ";";

     PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}

//	_ParseGPResult(res, autoParts);
}

bool AutoPartManager::SetAutoPartResponseAvatar(int autoPartId, const std::string& avatarPath)
{
    std::string sql = "UPDATE auto_part_responses SET avatar = '" + avatarPath + "' WHERE id = " + std::to_string(autoPartId) + ";";
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Add auto part image failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        return false;
    }

    return true;
}

bool AutoPartManager::SetAutoPartAvatar(int autoPartId, const std::string& avatarPath)
{
    std::string sql = "UPDATE auto_part_requests SET avatar = '" + avatarPath + "' WHERE id = " + std::to_string(autoPartId) + ";";
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Add auto part image failed: %s", PQerrorMessage(pConn));
		PQclear(res);
		return false;
	}

	return true;
}

bool AutoPartManager::AddAutoPartImage(int autoPartId, const std::string& imagePath)
{
	std::string sql = "INSERT INTO auto_part_images(auto_part_id, image_path) VALUES ("
		+ std::to_string(autoPartId) + ", '"
		+ imagePath + "'"
		+ ");";

      PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Add auto part image failed: %s", PQerrorMessage(pConn));
		PQclear(res);
		return false;
	}

	return true;
}

bool AutoPartManager::_ParseGPResult(PGresult* res, std::vector<DBAutoPartRequest*>& autoParts)
{
	char* temp = (char*)calloc(256, sizeof(char));

    PGconn* pConn = ConnectionPool::Get()->getConnection();
	int rec_count = PQntuples(res);
	for (int i = 0; i < rec_count; i++)
	{

        DBAutoPartRequest* pAutoPart = new DBAutoPartRequest();
		strcpy(temp, PQgetvalue(res, i, 0));
		pAutoPart->Id = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 1));
        pAutoPart->Make = temp;

        strcpy(temp, PQgetvalue(res, i, 2));
        pAutoPart->Serie = temp;

        strcpy(temp, PQgetvalue(res, i, 3));
        pAutoPart->Model = temp;

        strcpy(temp, PQgetvalue(res, i, 4));
        pAutoPart->Description = temp;

        strcpy(temp, PQgetvalue(res, i, 5));
        pAutoPart->UserId = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 6));
        pAutoPart->Category = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 7));
        pAutoPart->Avatar = temp;

        std::string sql = "SELECT COUNT(*) FROM auto_part_responses WHERE req_id = "
                        + std::to_string(pAutoPart->Id) + ";";
        PGresult* r = PQexec(pConn, sql.c_str());
        char* tmp = (char*)calloc(256, sizeof(char));
        strcpy(tmp, PQgetvalue(r, 0, 0));
        pAutoPart->ResponseCount = atoi(tmp);
        PQclear(r);
        free(tmp);
//		{
//			std::string sql = "SELECT image_path FROM auto_part_images WHERE auto_part_id = "
//				+ std::to_string(pAutoPart->Id) + ";";

//            PGresult* res = PQexec(pConn, sql.c_str());
//			if (PQresultStatus(res) != PGRES_TUPLES_OK)
//			{
//                char* err = PQerrorMessage(pConn);
//                fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
//				PQclear(res);
//				//exit_nicely(conn);
//			}
//			else
//			{
//				int imageCount = PQntuples(res);
//				char* tmp = (char*)calloc(256, sizeof(char));
//				for (int img = 0; img < imageCount; img++)
//				{
//					strcpy(tmp, PQgetvalue(res, img, 0));
//					pAutoPart->Images.push_back(std::string(tmp));
//				}
//				free(tmp);
//			}
//			PQclear(res);
//		}

		autoParts.push_back(pAutoPart);
	}
	free(temp);
	PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
	return true;
}
