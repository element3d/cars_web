#include "UserManager.h"
#include <iostream>
#include "PQManager.h"

UserManager* UserManager::sInstance = nullptr;

UserManager* UserManager::Get()
{
    if (!sInstance) sInstance = new UserManager();

    return sInstance;
}



/*void UserManager::SetPsql(pqxx::connection* pPsql)
{
    mPsql = pPsql;
}*/

int UserManager::CreateUser(const std::string& email, const std::string& name)
{
    std::string sql = "INSERT INTO users(email,first_name, num_golds) VALUES ('"
        + email + "', '" 
		+ name + "', " 
        + std::to_string(20) +
    ");";

    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to create user: %s", PQerrorMessage(pg));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return -1;
    }

    sql = "SELECT currval('users_id_seq');";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get new user id: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return -1;
    }
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    free(temp);
    ConnectionPool::Get()->releaseConnection(pg);
    return id;
}

int UserManager::CreateUser(const std::string& phone, const std::string& password, int type, const std::string& firstName)
{
    std::string sql = "INSERT INTO users(phone, password, type, first_name, num_golds) VALUES ('"
            + phone + "', '" 
			      + password + "', " 
			      + std::to_string(type) + ", '"
			      + firstName + "', '"
            + std::to_string(20) +
			      "');";


    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
	  if (PQresultStatus(res) != PGRES_COMMAND_OK)
	  {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to create user: %s", PQerrorMessage(pg));
		    PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return -1;
    }

    sql = "SELECT currval('users_id_seq');";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get new user id: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return -1;
    }
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    free(temp);
    ConnectionPool::Get()->releaseConnection(pg);
    return id;
}
#include <cstdlib>

std::vector<std::string> UserManager::UserGetAutoPartMakes(int id)
{
    std::vector<std::string> makes;
    std::string sql = "SELECT make FROM user_auto_part_makes WHERE user_id=" + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return makes;
    }


    int nt = PQntuples(res);
    if (!nt)
    {
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return makes;
    }

    char* temp = (char*)calloc(256, sizeof(char));
    for (int i = 0; i < nt; ++i)
    {
        strcpy(temp, PQgetvalue(res, i, 0));
        makes.push_back(temp);
    }
    free(temp);
    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return makes;
}

std::vector<int> UserManager::UserGetAutoPartCategories(int id)
{
    std::vector<int> makes;
    std::string sql = "SELECT category FROM user_auto_part_categories WHERE user_id=" + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return makes;
    }


    int nt = PQntuples(res);
    if (!nt)
    {
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return makes;
    }

    char* temp = (char*)calloc(256, sizeof(char));
    for (int i = 0; i < nt; ++i)
    {
        strcpy(temp, PQgetvalue(res, i, 0));
        makes.push_back(atoi(temp));
    }
    free(temp);
    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return makes;
}

void UserManager::UserSetAutoPartCategories(int id, const std::vector<int>& categories)
{
    std::string sql = "DELETE FROM user_auto_part_categories WHERE user_id=" + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        PQfinish(pg);
        return;
    }

    PQclear(res);
    for (auto& c: categories)
    {
        sql = "INSERT INTO user_auto_part_categories(user_id, category) VALUES("
            + std::to_string(id) + ", "
            + std::to_string(c)
            + ");";

        res = PQexec(pg, sql.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
            PQclear(res);
            PQfinish(pg);
            return;
        }
        PQclear(res);
    }

    PQfinish(pg);
}

void UserManager::UserSetAutoPartMakes(int id, const std::vector<std::string>& makes)
{
    std::string sql = "DELETE FROM user_auto_part_makes WHERE user_id=" + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        PQfinish(pg);
        return;
    }

    PQclear(res);
    for (auto& make: makes)
    {
        sql = "INSERT INTO user_auto_part_makes(user_id, make) VALUES("
            + std::to_string(id) + ", '"
            + make + "'"
            + ");";

        res = PQexec(pg, sql.c_str());
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
            PQclear(res);
            PQfinish(pg);
            return;
        }
        PQclear(res);
    }

    PQfinish(pg);
}

int UserManager::GetUserNumGolds(int id)
{
    std::string sql = "SELECT num_golds FROM users WHERE id = " + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Error: Failed to get num_golds: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return 0;
    }

    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    int numGolds = atoi(temp);
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pg);
    free(temp);
    return numGolds;
}

bool UserManager::UserEarnGold(int id)
{
    std::string sql = "UPDATE users set num_golds=num_golds+1 WHERE id = " + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }

    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

bool UserManager::UserAddDevice(int userId,const std::string& device, const std::string& os)
{
    PGconn* pg = ConnectionPool::Get()->getConnection();

    std::string sql = "SELECT COUNT(*) FROM devices where user_id = " + std::to_string(userId)
        + " AND device = '" + device + "';";

    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }
    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    bool exists = atoi(temp) > 0;
    PQclear(res);
    free(temp);
    if (exists) 
    {
        ConnectionPool::Get()->releaseConnection(pg);
        return true;
    }

    sql = "INSERT INTO devices (user_id, device, os) VALUES (" + std::to_string(userId) 
        + ",'" + device 
        + "','" + os 
        + "');";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }

    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

bool UserManager::UserReceiveGift(int giftId)
{
    std::string sql = "DELETE FROM gifts WHERE id = " + std::to_string(giftId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }

    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

DBGift* UserManager::GetUserGift(int id)
{
    std::string sql = "SELECT * FROM gifts WHERE user_id = " + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }

    int nt = PQntuples(res);
    if (!nt)
    {
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return nullptr;
    }

    DBGift* pGift = new DBGift();
    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    pGift->Id = atoi(temp);

    strcpy(temp, PQgetvalue(res, 0, 2));
    pGift->Type = atoi(temp);

    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return pGift;
}

bool UserManager::ChangePassword(const std::string& phone, const std::string& password)
{
   std::string sql = "UPDATE USERS SET password = '"
            + password + "' WHERE phone = '" + phone + "';";

    PGconn* pg = ConnectionPool::Get()->getConnection();

    PGresult* res = PQexec(pg, sql.c_str());
	  if (PQresultStatus(res) != PGRES_COMMAND_OK)
	  {
      char* err = PQerrorMessage(pg);
      fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
		  PQclear(res);
      //PQfinish(pg);
      ConnectionPool::Get()->releaseConnection(pg);
		  //exit_nicely(conn);
		  return false;
	  }
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

DBUser* UserManager::GetUser(const std::string& username)
{
    std::string sql = "SELECT * FROM users WHERE phone = '"
        + username 
        + "' AND (email='' or email is null);";
      
   PGconn* pg = ConnectionPool::Get()->getConnection(); //ConnectionPool::Get()->getConnection();

    PGresult* res = PQexec(pg, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
		    PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
		//exit_nicely(conn);
		return nullptr;
	}

	/*int field_count = PQnfields(res);
	for (int i = 0; i< field_count; i++)
	{
		fprintf(stdout, "%-40s", PQfname(res, i));
	}
	fprintf(stdout, "\n");*/

	int nt = PQntuples(res);
	if (!nt)  
  {
    ConnectionPool::Get()->releaseConnection(pg);
    return nullptr;
  }
	int nf = PQnfields(res);

	char* temp = (char*)calloc(256, sizeof(char));
	DBUser* pUser = new DBUser();
	strcpy(temp, PQgetvalue(res, 0, 0));
	pUser->Id = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 1));
	pUser->Phone = (temp);

	strcpy(temp, PQgetvalue(res, 0, 2));
	pUser->Password = (temp);

	strcpy(temp, PQgetvalue(res, 0, 3));
	pUser->Type = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 4));
	pUser->FirstName = (temp);

  strcpy(temp, PQgetvalue(res, 0, 5));
  pUser->Avatar = (temp);

  strcpy(temp, PQgetvalue(res, 0, 6));
  pUser->NumGolds = atoi(temp);

  strcpy(temp, PQgetvalue(res, 0, 7));
  pUser->Cover = (temp);
    /*strcpy(temp, PQgetvalue(res, 0, 6));
	pUser->NumGolds = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 7));
    pUser->Reputation = atof(temp);*/

/*	int rec_count = PQntuples(res);
	for (int i = 0; i< rec_count; i++)
	{
		for (int j = 0; j< field_count; j++)
		{
			strcpy(temp, PQgetvalue(res, i, j));
			temp[strlen(temp) - 1] = 0;
			fprintf(stdout, "%-40s", temp);
		}
		fprintf(stdout, "\n");
	}*/

 /*   pqxx::nontransaction N(*mPsql);
    pqxx::result R;
    try
    {
        R = N.exec(sql);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }

    int id;
    std::string pwd;
    std::string ph;
    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c)
    {

          id = c[0].as<int>();
          std::cout << "phone = " << c[1].as<std::string>() << std::endl;
          pwd = c[2].as<std::string>();
    }*/

   // DBUser* pUser = new DBUser();
    /*pUser->Id = id;
    pUser->Phone = phone;
    pUser->Password = pwd;*/
	free(temp);
    //PQfinish(pg);
   ConnectionPool::Get()->releaseConnection(pg);
    return pUser;
}

bool UserManager::EditUser(int id, const std::string& firstName, const std::string& phone, const std::string& whatsapp, const std::string& viber)
{
    std::string sql = "UPDATE users SET first_name='" + firstName 
        + "', phone = '" + phone 
        + "', whatsapp = '" + whatsapp
        + "', viber = '" + viber
        + "' WHERE id = "
            + std::to_string(id) + ";";

    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to edit user: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

bool UserManager::MeDelete(int id)
{
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    std::string sql = "DELETE FROM cars WHERE user_id = " + std::to_string(id) + ";";
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Error: Failed to delete user cars: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
		return false;
	}
    PQclear(res);

    sql = "DELETE FROM users WHERE id = " + std::to_string(id) + ";";
    res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Error: Failed to delete user: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
		return false;
	}
    PQclear(res);

    ConnectionPool::Get()->releaseConnection(pConn);
    return true;   
}

bool UserManager::MeHandshake(int id)
{
    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    const std::string sql = "UPDATE users SET handshake_ts = " + std::to_string(ms) + " WHERE id = " + std::to_string(id) + ";";
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Error: Failed to get user: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
		return false;
	}
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
    return true;
}

uint64_t UserManager::UserHandshake(int id)
{
    const std::string sql = "SELECT handshake_ts FROM users WHERE id = " + std::to_string(id) + ";";
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Error: Failed to get user: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
		return 0;
	}
    bool userFound = PQntuples(res) > 0;
    if (!userFound) return -1;

    char* temp = (char*)calloc(256, sizeof(char));
	strcpy(temp, PQgetvalue(res, 0, 0));
	uint64_t ts = atoll(temp);
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
    free(temp);
    return ts;
}

DBUser* UserManager::GetUserByEmail(const std::string& email)
{
    if (email.size() <= 0) return nullptr;
    std::string sql = "SELECT * FROM users WHERE email = '"
                + email + "';";

    PGconn* pConn = ConnectionPool::Get()->getConnection();

    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0)
	{
        char* err = PQerrorMessage(pConn);
        fprintf(stderr, "Error: Failed to get user: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
	    //exit_nicely(conn);
		return nullptr;
	}

	char* temp = (char*)calloc(256, sizeof(char));
	DBUser* pUser = new DBUser();
	strcpy(temp, PQgetvalue(res, 0, 0));
	pUser->Id = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 1));
	pUser->Phone = (temp);

	strcpy(temp, PQgetvalue(res, 0, 2));
	pUser->Password = (temp);

	strcpy(temp, PQgetvalue(res, 0, 3));
	pUser->Type = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 4));
	pUser->FirstName = (temp);

    strcpy(temp, PQgetvalue(res, 0, 5));
    pUser->Avatar = (temp);

    strcpy(temp, PQgetvalue(res, 0, 6));
    pUser->NumGolds = atoi(temp);

    strcpy(temp, PQgetvalue(res, 0, 7));
    pUser->Cover = temp;

    ConnectionPool::Get()->releaseConnection(pConn);
	free(temp);
    return pUser;
}

DBUser* UserManager::GetUser(int id)
{
    if (id <= 0) return nullptr;
    std::string sql = "SELECT * FROM users WHERE id = "
                + std::to_string(id) + ";";

    PGconn* pConn = ConnectionPool::Get()->getConnection();//ConnectionPool::Get()->getConnection();

    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0)
	{
      char* err = PQerrorMessage(pConn);
      fprintf(stderr, "Error: Failed to get user: %s", PQerrorMessage(pConn));
		  PQclear(res);
      // ConnectionPool::Get()->releaseConnection(pConn);
      ConnectionPool::Get()->releaseConnection(pConn);
		  //exit_nicely(conn);
		  return nullptr;
	}

	char* temp = (char*)calloc(256, sizeof(char));
	DBUser* pUser = new DBUser();
	strcpy(temp, PQgetvalue(res, 0, 0));
	pUser->Id = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 1));
	pUser->Phone = (temp);

	strcpy(temp, PQgetvalue(res, 0, 2));
	pUser->Password = (temp);

	strcpy(temp, PQgetvalue(res, 0, 3));
	pUser->Type = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 4));
	pUser->FirstName = (temp);

    strcpy(temp, PQgetvalue(res, 0, 5));
    pUser->Avatar = (temp);

    strcpy(temp, PQgetvalue(res, 0, 6));
    pUser->NumGolds = atoi(temp);

    strcpy(temp, PQgetvalue(res, 0, 7));
    pUser->Cover = temp;

    strcpy(temp, PQgetvalue(res, 0, 9));
    pUser->Email = temp;

    strcpy(temp, PQgetvalue(res, 0, 10));
    pUser->WhatsApp = temp;

    strcpy(temp, PQgetvalue(res, 0, 11));
    pUser->Viber = temp;

/*	strcpy(temp, PQgetvalue(res, 0, 6));
	pUser->NumGolds = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 7));
    pUser->Reputation = atof(temp);*/

  /*  pqxx::nontransaction N(*mPsql);
    pqxx::result R;
    try
    {
        R = N.exec(sql);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }

    int userId;
    std::string ph;
    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c)
    {
        id = c[0].as<int>();
        ph = c[1].as<std::string>();
          std::cout << "ID = " << c[0].as<int>() << std::endl;
          std::cout << "phone = " << c[1].as<std::string>() << std::endl;
          std::cout << "password = " << c[2].as<std::string>() << std::endl;
    }

    DBUser* pUser = new DBUser();
    pUser->Id = id;
    pUser->Phone = ph;*/
    //ConnectionPool::Get()->releaseConnection(pConn);
  ConnectionPool::Get()->releaseConnection(pConn);
	free(temp);
  return pUser;
}

bool UserManager::SetUserCover(int userId, const std::string& coverPath)
{
    std::string sql = "UPDATE users SET cover = '" + coverPath + "' WHERE id = " + std::to_string(userId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to set user cover: %s", PQerrorMessage(pg));
        PQclear(res);
  
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }
    PQclear(res);

    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

bool UserManager::SetUserAvatar(int userId, const std::string& avatarPath)
{
    std::string sql = "UPDATE users SET avatar = '" + avatarPath + "' WHERE id = " + std::to_string(userId) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Add car image failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }
    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}
