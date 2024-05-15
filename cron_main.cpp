#include "managers/PQManager.h"
#include <string>
#include <vector>
#include <memory.h>
#include <chrono>

struct DBCar
{
    int Id;
    long int OnTopTs;
    long int IsUrgentTs;
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
    // On TOP
    // std::string sql = "SELECT id, on_top_ts FROM cars WHERE on_top > 0 AND on_top_ts > 0;";
    // PGconn* pConn = ConnectionPool::Get()->getConnection();
    // PGresult* res = PQexec(pConn, sql.c_str());
    // if (PQresultStatus(res) != PGRES_TUPLES_OK)
    // {
    //     fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
    //     PQclear(res);
    //     return -1;
    // }

    using namespace std::chrono;
    uint64_t current_time_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    PGconn* pConn = ConnectionPool::Get()->getConnection();

    // Is urgent
    {
        std::string sql = "SELECT id, is_urgent_ts FROM cars WHERE is_urgent > 0 AND is_urgent_ts > 0;";
        PGresult* res = PQexec(pConn, sql.c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK) 
        {
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
            PQclear(res);
            return -1;
        }

        uint64_t one_week_ms = 7 * 24 * 60 * 60 * 1000;

        char* temp = (char*)calloc(256, sizeof(char));
        int rec_count = PQntuples(res);

        printf("Urgent car count: %i\n", rec_count);

        for (int i = 0; i < rec_count; i++) 
        {
            DBCar* pCar = new DBCar();
            strcpy(temp, PQgetvalue(res, i, 0));
            pCar->Id = atoi(temp);

            strcpy(temp, PQgetvalue(res, i, 1));
            pCar->IsUrgentTs = atol(temp);

            // Check if the difference is greater than a week
            if (current_time_ms - pCar->IsUrgentTs > one_week_ms) 
            {
                // Update the is_urgent field to 0
                std::string update_sql = "UPDATE cars SET is_urgent = 0 WHERE id = " + std::to_string(pCar->Id) + ";";
                PGresult* update_res = PQexec(pConn, update_sql.c_str());

                if (PQresultStatus(update_res) != PGRES_COMMAND_OK) 
                {
                    fprintf(stderr, "UPDATE failed: %s", PQerrorMessage(pConn));
                }

                printf("Urgent car updated.\n");
                PQclear(update_res);
            }
        
            delete pCar;
        }

        // Clean up
        free(temp);
        PQclear(res);
    }


    // Refresh Top Cars
    {
        std::string sql = "SELECT id, on_top_ts FROM cars WHERE on_top > 0 AND on_top_ts > 0;";
        PGresult* res = PQexec(pConn, sql.c_str());

        if (PQresultStatus(res) != PGRES_TUPLES_OK) 
        {
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
            PQclear(res);
            return -1;
        }

        char* temp = (char*)calloc(256, sizeof(char));
        int rec_count = PQntuples(res);
        printf("Top car count: %i\n", rec_count);

        for (int i = 0; i < rec_count; i++) 
        {
            DBCar* pCar = new DBCar();
            strcpy(temp, PQgetvalue(res, i, 0));
            pCar->Id = atoi(temp);

            strcpy(temp, PQgetvalue(res, i, 1));
            pCar->OnTopTs = atol(temp);

            // Update the refresh_ts field to current_time_ms
            std::string update_sql = "UPDATE cars SET refresh_ts = " + std::to_string(current_time_ms) + " WHERE id = " + std::to_string(pCar->Id) + ";";
            PGresult* update_res = PQexec(pConn, update_sql.c_str());

            if (PQresultStatus(update_res) != PGRES_COMMAND_OK) 
            {
                fprintf(stderr, "UPDATE failed: %s", PQerrorMessage(pConn));
            }
            printf("Top car refreshed.\n");
            PQclear(update_res);          
            delete pCar;
        }

         // Clean up
        free(temp);
        PQclear(res);
    }
    ConnectionPool::Get()->releaseConnection(pConn);
   /* _ParseCars(res, cars);



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
    }*/


    return 0;
}
