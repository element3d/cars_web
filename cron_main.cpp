#include "managers/PQManager.h"
#include <string>
#include <vector>
#include <memory.h>
#include <chrono>

struct DBCar
{
    int Id;
    long int OnTopTs;
};

struct DBUser
{
    int Id;
    long int LastGoldGift;
};

void _ParseCars(PGresult* res, std::vector<DBCar*>& cars)
{
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    for (int i = 0; i < rec_count; i++)
    {
        DBCar* pCar = new DBCar();
        strcpy(temp, PQgetvalue(res, i, 0));
        pCar->Id = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 1));
        pCar->OnTopTs = atol(temp);

        cars.push_back(pCar);
    }

    free(temp);
    PQclear(res);
}

void _ParseUsers(PGresult* res, std::vector<DBUser*>& users)
{
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    for (int i = 0; i < rec_count; i++)
    {
        DBUser* pUser = new DBUser();
        strcpy(temp, PQgetvalue(res, i, 0));
        pUser->Id = atoi(temp);

        strcpy(temp, PQgetvalue(res, i, 1));
        pUser->LastGoldGift = atol(temp);

        users.push_back(pUser);
    }

    free(temp);
    PQclear(res);
}

int main()
{
    std::string sql = "SELECT id, on_top_ts FROM cars WHERE on_top > 0 AND on_top_ts > 0;";
    PGconn* pConn = GetPQConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        return -1;
    }

    std::vector<DBCar*> cars;
    _ParseCars(res, cars);

    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    std::vector<DBCar*> tmpCars;

    for (auto pCar : cars)
    {
        long int d = ms - pCar->OnTopTs;
        long int days = d / (1000 * 60 * 60 * 24);
        if (days > 7) tmpCars.push_back(pCar);
    }

    for (auto pCar : tmpCars)
    {
        sql = "UPDATE cars SET on_top = 0 WHERE id = " + std::to_string(pCar->Id) + ";";
        res = PQexec(pConn, sql.c_str());
    }

    sql = "SELECT id, last_gold_gift FROM users where num_golds < 10;";
    res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        return -1;
    }

    std::vector<DBUser*> users;
    _ParseUsers(res, users);
    std::vector<DBUser*> tmpUsers;
    for (auto pUser : users)
    {
        long int d = ms - pUser->LastGoldGift;
        long int days = d / (1000 * 60 * 60 * 24);
        if (days > 7)
        {
            tmpUsers.push_back(pUser);
        }
    }

    for (auto pUser : tmpUsers)
    {
        sql = "UPDATE users SET num_golds = 10, last_gold_gift = " + std::to_string(ms) +  " WHERE id = " + std::to_string(pUser->Id) + ";";
        res = PQexec(pConn, sql.c_str());
    }


    return 0;
}
