#include "UserManager.h"
#include <iostream>


UserManager* UserManager::sInstance = nullptr;

UserManager* UserManager::Get()
{
    if (!sInstance) sInstance = new UserManager();

    return sInstance;
}

void UserManager::SetPG(PGconn* pPG)
{
	mPG = pPG;
}

void UserManager::SetPsql(pqxx::connection* pPsql)
{
    mPsql = pPsql;
}

bool UserManager::CreateUser(const std::string& phone, const std::string& password, int type, const std::string& firstName, const std::string& secondName)
{
    std::string sql = "INSERT INTO users(phone, password, type, first_name, second_name) VALUES ('"
            + phone + "', '" 
			+ password + "', " 
			+ std::to_string(type) + ", '"
			+ firstName + "', '"
			+ secondName +
			"');";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		//exit_nicely(conn);
		return false;
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
    return true;
}
#include <cstdlib>

DBUser* UserManager::GetUser(const std::string& phone)
{
    std::string sql = "SELECT * FROM users WHERE phone = '"
            + phone + "';";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
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
	pUser->Phone = (temp);

	strcpy(temp, PQgetvalue(res, 0, 2));
	pUser->Password = (temp);

	strcpy(temp, PQgetvalue(res, 0, 3));
	pUser->Type = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 4));
	pUser->FirstName = (temp);

	strcpy(temp, PQgetvalue(res, 0, 5));
	pUser->SecondName = (temp);

	strcpy(temp, PQgetvalue(res, 0, 6));
	pUser->NumGolds = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 7));
	pUser->Reputation = atof(temp);

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
    return pUser;
}

DBUser* UserManager::GetUser(int id)
{
    std::string sql = "SELECT * FROM users WHERE id = "
            + std::to_string(id) + ";";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
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
	pUser->SecondName = (temp);

	strcpy(temp, PQgetvalue(res, 0, 6));
	pUser->NumGolds = atoi(temp);

	strcpy(temp, PQgetvalue(res, 0, 7));
	pUser->Reputation = atof(temp);

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
	free(temp);
    return pUser;
}
