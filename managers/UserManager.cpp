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

int UserManager::CreateUser(const std::string& username, const std::string& phone, const std::string& password, int type, const std::string& firstName, const std::string& secondName)
{
    std::string sql = "INSERT INTO users(username, phone, password, type, first_name, second_name, num_golds) VALUES ('"
            + username + "', '"
            + phone + "', '" 
			+ password + "', " 
			+ std::to_string(type) + ", '"
			+ firstName + "', '"
            + secondName + "', '"
            + std::to_string(20) +
			"');";


    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
      char* err = PQerrorMessage(pg);
      fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
		  PQclear(res);
      ConnectionPool::Get()->releaseConnection(pg);
	  	//exit_nicely(conn);
      return -1;
    }

    /*pqxx::work W(*mPsql);
    try
    {
        pqxx::result res = W.exec( sql );
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    W.commit();*/
//    return true;
    sql = "SELECT currval('users_id_seq');";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
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
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        //PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return 0;
    }

    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    int numGolds = atoi(temp);
    PQclear(res);
    //PQfinish(pg);
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
            + password + "' WHERE username = '" + phone + "';";

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
    ConnectionPool::Get()->releaseConnection(pg);
}

DBUser* UserManager::GetUser(const std::string& username)
{
    std::string sql = "SELECT * FROM users WHERE username = '"
            + username + "';";
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
	if (!nt) return nullptr;
	int nf = PQnfields(res);

	char* temp = (char*)calloc(256, sizeof(char));
	DBUser* pUser = new DBUser();
	strcpy(temp, PQgetvalue(res, 0, 0));
	pUser->Id = atoi(temp);

  strcpy(temp, PQgetvalue(res, 0, 1));
  pUser->Username = (temp);

	strcpy(temp, PQgetvalue(res, 0, 2));
	pUser->Phone = (temp);

	strcpy(temp, PQgetvalue(res, 0, 3));
	pUser->Password = (temp);

	strcpy(temp, PQgetvalue(res, 0, 4));
	pUser->Type = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 5));
	pUser->FirstName = (temp);

	strcpy(temp, PQgetvalue(res, 0, 6));
	pUser->SecondName = (temp);

  strcpy(temp, PQgetvalue(res, 0, 7));
  pUser->Avatar = (temp);

  strcpy(temp, PQgetvalue(res, 0, 8));
  pUser->NumGolds = atoi(temp);


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

bool UserManager::EditUser(int id, const std::string& firstName, const std::string& secondName, const std::string& phone)
{
    std::string sql = "UPDATE users SET first_name='" + firstName + "', second_name = '" + secondName + "', phone = '" + phone + "' WHERE id = "
            + std::to_string(id) + ";";

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
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
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
      fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
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
  pUser->Username = (temp);

	strcpy(temp, PQgetvalue(res, 0, 2));
	pUser->Phone = (temp);

	strcpy(temp, PQgetvalue(res, 0, 3));
	pUser->Password = (temp);

	strcpy(temp, PQgetvalue(res, 0, 4));
	pUser->Type = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 5));
	pUser->FirstName = (temp);

	strcpy(temp, PQgetvalue(res, 0, 6));
	pUser->SecondName = (temp);

  strcpy(temp, PQgetvalue(res, 0, 7));
  pUser->Avatar = (temp);

  strcpy(temp, PQgetvalue(res, 0, 8));
  pUser->NumGolds = atoi(temp);

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
