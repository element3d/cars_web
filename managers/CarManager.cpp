#include "CarManager.h"
#include <iostream>
#include "PQManager.h"
#include "EMake.h"
#include <chrono>
#include "EventsManager.h"

CarManager* CarManager::sInstance = nullptr;

CarManager* CarManager::Get()
{
	if (!sInstance) sInstance = new CarManager();

	return sInstance;
}



int CarManager::CreateCar(int userId, const std::string& carJson)
{
	  rapidjson::Document d;
	  d.Parse(carJson.c_str());
  
    if (!d.HasMember("model")) return -1;

    PGconn* pg = ConnectionPool::Get()->getConnection();
    std::string  sql = "SELECT num_golds FROM USERS WHERE id=" + std::to_string(userId) + ";";
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get num_golds: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return -1;
    }

    int numGolds = 0;
    {
        char* temp = (char*)calloc(256, sizeof(char));
        int rec_count = PQntuples(res);
        strcpy(temp, PQgetvalue(res, 0, 0));
        numGolds = atoi(temp);
        free(temp);
    }

    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    auto now = std::chrono::system_clock::now();

    // Convert the timepoint to milliseconds since January 1, 1970 (Unix Epoch)
    auto duration = now.time_since_epoch();
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


    uint64_t onTopTs = ms;

    int onTop = d["on_top"].GetInt();
    int dg = 0;
    if (onTop == 1) dg = 10;
    else if (onTop == 2) dg = 15;
    else if (onTop == 3) dg = 20;
    if (numGolds < dg || onTop == 0)
    {
        onTop = 0;
        dg = 0;
        onTopTs = 0;
    }

    sql = "INSERT INTO cars(user_id , make, class, model, submodel, country, province, sub_province, price, body_type, year, engine_type, engine_size, engine_power, drive_type, transmission, stearing_wheel, exchange, customs_cleared, color, mileage, description, on_sale, on_top, on_top_ts, refresh_ts) VALUES ("
        + std::to_string(userId) + ", '"
        + d["make"].GetString() + "', '"
        + d["class"].GetString() + "', '"
        + d["model"].GetString() + "', '"
		    + d["submodel"].GetString() + "', "
		+ std::to_string(d["country"].GetInt()) + ", "
		+ std::to_string(d["province"].GetInt()) + ", "
		+ std::to_string(d["sub_province"].GetInt()) + ", "
		+ std::to_string(d["price"].GetInt()) + ", "
		+ std::to_string(d["body_type"].GetInt()) + ", "
		+ std::to_string(d["year"].GetInt()) + ", "
		+ std::to_string(d["engine_type"].GetInt()) + ", "
		+ std::to_string(d["engine_size"].GetFloat()) + ", "
		+ std::to_string(d["engine_power"].GetInt()) + ", "
    + std::to_string(d["drive_type"].GetInt()) + ", "
		+ std::to_string(d["transmission"].GetInt()) + ", "
		+ std::to_string(d["stearing_wheel"].GetInt()) + ", "
		+ std::to_string(d["exchange"].GetInt()) + ", "
		+ std::to_string(d["customs_cleared"].GetInt()) + ", "
		+ std::to_string(d["color"].GetInt()) + ", "
        + std::to_string(d["mileage"].GetInt()) + ", '"
        + d["description"].GetString() + "', "
		//+ std::string(d["avatar"].GetString()) + "', '"
		//+ std::string(d["images"].GetString()) + "', "
		+ std::to_string(d["on_sale"].GetInt()) + ", "
        + std::to_string(onTop) + ", "
        + std::to_string(onTopTs) + ", "
        + std::to_string(ms) + ""
		");";


    res = PQexec(pg, sql.c_str());
	  if (PQresultStatus(res) != PGRES_COMMAND_OK)
	  {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to create car: %s", PQerrorMessage(pg));
		    PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
		    return -1;
	  }

	  sql = "SELECT currval('cars_id_seq');";
    res = PQexec(pg, sql.c_str());
	  if (PQresultStatus(res) != PGRES_TUPLES_OK)
	  {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get new car id: %s", PQerrorMessage(pg));
		    PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
	  	  return -1;
	  }

	  char* temp = (char*)calloc(256, sizeof(char));
	  int rec_count = PQntuples(res);
	  strcpy(temp, PQgetvalue(res, 0, 0));
	  int id = atoi(temp);
	  free(temp);

    if (dg)
    {
        sql = "UPDATE USERS SET num_golds=num_golds-" + std::to_string(dg) + " WHERE id=" + std::to_string(userId) + ";";
        res = PQexec(pg, sql.c_str());
        PQclear(res);
    }

    DBInception* pI = EventsManager::Get()->GetInception();
    bool found = false;
    if (pI->Status == EEventStatus::Started) 
    {
      for (auto u : pI->Users) 
      { 
        if (u.Id == userId) 
        {
          found = true;
          break;
        }
      }
    }

    if (!found && pI->Status == EEventStatus::Started) 
    {
        EventsManager::Get()->InceptionAddUser(userId);
    }
    delete pI;

    ConnectionPool::Get()->releaseConnection(pg);
	  return id;
}

#include "../stlplus/file_system.hpp"

bool CarManager::DeleteCar(int userId, int id)
{
    std::string sql = "DELETE from cars WHERE id=" + std::to_string(id) + ";";
    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to delete car: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return 0;
    }

    PQclear(res);
    sql = "DELETE from car_images WHERE car_id=" + std::to_string(id) + ";";
    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to delete car images: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return 0;
    }

    PQclear(res);
    std::string dir = "data/cars/" + std::to_string(userId) + "/" + std::to_string(id) + "/";
    auto all = stlplus::folder_all(dir);
    for (auto f : all)
    {
        stlplus::file_delete(dir + f);
    }
    stlplus::folder_delete(dir);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

bool CarManager::EditCar(int userId, int id, const std::string& carJson)
{
    rapidjson::Document d;
    d.Parse(carJson.c_str());

    std::string sql = "SELECT on_top FROM cars WHERE id=" + std::to_string(id) + ";";

    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get on_top: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return 0;
    }

    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    int onTopOld = atoi(temp);
    int onTopNew = d["on_top"].GetInt();
    PQclear(res);

    int dg = 0;
    int onTop = onTopOld;
    long int onTopTs = 0;
    if (onTopNew > onTopOld)
    {
        if (onTopNew == 1) dg = 10;
        else if (onTopNew == 2) dg = 15;
        else if (onTopNew == 3) dg = 20;
        if (dg)
        {
            sql = "SELECT num_golds FROM users WHERE id=" + std::to_string(userId) + ";";
            res = PQexec(pg, sql.c_str());
            if (PQresultStatus(res) != PGRES_TUPLES_OK)
            {
                fprintf(stderr, "Error: Failed to get num_golds: %s", PQerrorMessage(pg));
                PQclear(res);
            }
            else
            {
                char* temp = (char*)calloc(256, sizeof(char));
                strcpy(temp, PQgetvalue(res, 0, 0));
                int numGolds = atoi(temp);
                if (numGolds - dg >= 0)
                {
                    onTop = onTopNew;
                    sql = "UPDATE USERS SET num_golds=num_golds-" + std::to_string(dg) + " WHERE id=" + std::to_string(userId) + ";";
                    res = PQexec(pg, sql.c_str());
                    ExecStatusType r =  PQresultStatus(res);
                    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK)
                    {
                        char* err = PQerrorMessage(pg);
                        fprintf(stderr, "Error: Failed to set num_golds: %s", PQerrorMessage(pg));
                        PQclear(res);
                        ConnectionPool::Get()->releaseConnection(pg);
                        return 0;
                    }

                    using namespace std::chrono;
                    onTopTs = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
                    PQclear(res);
                }
            }
        }
    }

    sql = std::string("UPDATE cars SET")
            + " make='" + d["make"].GetString()
            + "' ,class='" + d["class"].GetString()
            + "' ,model='" + d["model"].GetString()
            + "' ,submodel='" + d["submodel"].GetString()
            + "' ,country=" + std::to_string(d["country"].GetInt())
            + " ,province=" + std::to_string(d["province"].GetInt())
            + " ,sub_province=" + std::to_string(d["sub_province"].GetInt())
            + " ,price=" + std::to_string(d["price"].GetInt())
            + " ,body_type=" + std::to_string(d["body_type"].GetInt())
            + " ,drive_type=" + std::to_string(d["drive_type"].GetInt())
            + " ,year=" + std::to_string(d["year"].GetInt())
            + " ,engine_type=" + std::to_string(d["engine_type"].GetInt())
            + " ,engine_size=" + std::to_string(d["engine_size"].GetFloat())
            + " ,engine_power=" + std::to_string(d["engine_power"].GetInt())
            + " ,transmission=" + std::to_string(d["transmission"].GetInt())
            + " ,stearing_wheel=" + std::to_string(d["stearing_wheel"].GetInt())
            + " ,exchange=" + std::to_string(d["exchange"].GetInt())
            + " ,customs_cleared=" + std::to_string(d["customs_cleared"].GetInt())
            + " ,color=" + std::to_string(d["color"].GetInt())
            + " ,on_sale=" + std::to_string(d["on_sale"].GetInt())
            + " ,mileage=" + std::to_string(d["mileage"].GetInt())
            + " ,description='" + d["description"].GetString()
            + " ',on_top=" + std::to_string(onTop);

            if (onTopTs > 0) sql += " ,on_top_ts=" + std::to_string(onTopTs);
            sql += " WHERE id=" + std::to_string(id)
            + ";";


    /*std::string sql = "UPDATE cars SET (user_id , make, class, model, submodel, country, province, sub_province, price, body_type, year, engine_type, engine_size, engine_power, transmission, stearing_wheel, exchange, customs_cleared, color, mileage, description, on_sale, on_top) VALUES ("
        + std::to_string(d["user_id"].GetInt()) + ", "
        + std::to_string(d["make"].GetInt()) + ", "
        + std::to_string(d["class"].GetInt()) + ", "
        + std::to_string(d["model"].GetInt()) + ", "
        + std::to_string(d["submodel"].GetInt()) + ", "
        + std::to_string(d["country"].GetInt()) + ", "
        + std::to_string(d["province"].GetInt()) + ", "
        + std::to_string(d["sub_province"].GetInt()) + ", "
        + std::to_string(d["price"].GetInt()) + ", "
        + std::to_string(d["body_type"].GetInt()) + ", "
        + std::to_string(d["year"].GetInt()) + ", "
        + std::to_string(d["engine_type"].GetInt()) + ", "
        + std::to_string(d["engine_size"].GetFloat()) + ", "
        + std::to_string(d["engine_power"].GetInt()) + ", "

        + std::to_string(d["transmission"].GetInt()) + ", "
        + std::to_string(d["stearing_wheel"].GetInt()) + ", "
        + std::to_string(d["exchange"].GetInt()) + ", "
        + std::to_string(d["customs_cleared"].GetInt()) + ", "
        + std::to_string(d["color"].GetInt()) + ", "
        + std::to_string(d["mileage"].GetInt()) + ", '"
        + d["description"].GetString() + "', "
        //+ std::string(d["avatar"].GetString()) + "', '"
        //+ std::string(d["images"].GetString()) + "', "
        + std::to_string(d["on_sale"].GetInt()) + ", "
        + std::to_string(d["on_top"].GetInt()) + ""
        ");";*/

    res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to update car: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }

    /*sql = "SELECT currval('cars_id_seq');";
    res = PQexec(mPG, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(mPG);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
        PQclear(res);
        return -1;
    }*/
    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    /*char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    free(temp);*/
    return true;
}

void CarManager::GetTopCarsBySerie(int serie, std::vector<DBCar*>& cars)
{
	  std::string sql;
	  if (serie == -1)
          sql = "SELECT * FROM cars WHERE on_sale = 1  ORDER BY random()  limit 10;";
	  else
          sql = "SELECT * FROM cars WHERE on_sale = 1 AND class = " + std::to_string(serie) + "  ORDER BY random() limit 10;";

    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return;
    }
    ConnectionPool::Get()->releaseConnection(pg);

    _ParseGPResult(res, cars);
}

void CarManager::GetTopCarsByMake(int make, std::vector<DBCar*>& cars)
{
	std::string sql;
	if (make == -1)
		sql = "SELECT * FROM cars WHERE on_sale = 1;";
	else
		sql = "SELECT * FROM cars WHERE on_sale = 1 AND make = " + std::to_string(make) + ";";

    sql = "SELECT * FROM cars WHERE on_top>0 ORDER BY random() LIMIT 10;";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
      fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
		  PQclear(res);
      ConnectionPool::Get()->releaseConnection(pConn);
		//exit_nicely(conn);
		return;
	}

   ConnectionPool::Get()->releaseConnection(pConn);
	_ParseGPResult(res, cars);
}

int CarManager::GetTotalNumCars(const CarFilter& filter)
{
    std::string sql;
    sql = std::string("SELECT COUNT(*) FROM cars");

    if (filter.Model != "all")
    {
        sql += " WHERE model='" + filter.Model + "'";
    }
    else if (filter.Class != "all")
    {
        sql += " WHERE class='" + filter.Class + "'";
    }
    else if (filter.Make != "all")
    {
        sql += " WHERE make='" + filter.Make + "'";
    }
    sql += ";";

    PGconn* pg = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get num cars: %s", PQerrorMessage(pg));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pg);
        return 0;
    }
    
    ConnectionPool::Get()->releaseConnection(pg);
    char* temp = (char*)calloc(256, sizeof(char));
    strcpy(temp, PQgetvalue(res, 0, 0));
    int totalNumCars = atoi(temp);
    free(temp);
    PQclear(res);
    return totalNumCars;
}

DBCar* CarManager::GetCar(int carId)
{
    std::string sql;
    sql = std::string("SELECT * FROM cars WHERE id = " + std::to_string(carId));
    ConnectionPool* pPool = ConnectionPool::Get();
    PGconn* pg = pPool->getConnection();

    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get car: %s", PQerrorMessage(pg));
        PQclear(res);
        pPool->releaseConnection(pg);
        return nullptr;
    }

    pPool->releaseConnection(pg);
    std::vector<DBCar*> cars;
    _ParseGPResult(res, cars);
    if (!cars.size()) return nullptr;
    return cars[0];
}

void CarManager::GetCars(const CarFilter& filter, int page, std::vector<DBCar*>& cars)
{
	  std::string sql;
    sql = std::string("SELECT * FROM cars");

    if (filter.SubModel != "all")
    {
        sql += " WHERE submodel='" + filter.SubModel + "'";
    }
    else if (filter.Model != "all")
    {
        sql += " WHERE model='" + filter.Model + "'";
    }
    else if (filter.Class != "all")
    {
        sql += " WHERE class='" + filter.Class + "'";
    }
    else if (filter.Make != "all")
    {
        sql += " WHERE make='" + filter.Make + "'";
    }
    else
    {
        sql += " WHERE make!=' '";
    }


    if (filter.Province >= 0)
    {
        sql += " AND province=" + std::to_string(filter.Province);
    }
    if (filter.PriceFrom >= 0)
    {
        sql += " AND price>=" + std::to_string(filter.PriceFrom);
    }
    if (filter.PriceTo >= 0)
    {
        sql += " AND price<=" + std::to_string(filter.PriceTo);
    }

    if (filter.YearFrom >= 0)
    {
        sql += " AND year>=" + std::to_string(filter.YearFrom);
    }
    if (filter.YearTo >= 0)
    {
        sql += " AND year<=" + std::to_string(filter.YearTo);
    }

    if (filter.BodyType >= 0)
    {
        sql += " AND body_type=" + std::to_string(filter.BodyType);
    }
    
    if (filter.DriveType >= 0)
    {
        sql += " AND drive_type=" + std::to_string(filter.DriveType);
    }
    if (filter.EngineType >= 0)
    {
        sql += " AND engine_type=" + std::to_string(filter.EngineType);
    }

    if (filter.EngineSizeFrom >= 0)
    {
        sql += " AND engine_size>=" + std::to_string(filter.EngineSizeFrom);
    }
    if (filter.EngineSizeTo >= 0)
    {
        sql += " AND engine_size<=" + std::to_string(filter.EngineSizeTo);
    }

    if (filter.Transmission >= 0)
    {
        sql += " AND transmission=" + std::to_string(filter.Transmission);
    }
    if (filter.SteeringWheel >= 0)
    {
        sql += " AND stearing_wheel=" + std::to_string(filter.SteeringWheel);
    }
    if (filter.Color >= 0)
    {
        sql += " AND color=" + std::to_string(filter.Color);
    }
    if (filter.CustomsCleared >= 0)
    {
        sql += " AND customs_cleared=" + std::to_string(filter.CustomsCleared);
    }
    if (filter.Exchange > 0)
    {
        sql += " AND exchange=" + std::to_string(filter.Exchange);
    }

    if (filter.OnSale > 0)
    {
        sql += " AND on_sale=" + std::to_string(filter.OnSale);
    }
    sql += " order by refresh_ts desc limit " +  std::to_string(filter.Limit) + " offset " + std::to_string(filter.Limit * std::max((page - 1), 0)) + ";";

    /*if (filter.Make == -1)
	{
		 sql = "SELECT * FROM cars;";
	}
	else if (filter.Class == -1)
	{
		sql = "SELECT * FROM cars WHERE make = " + std::to_string(filter.Make) + ";";
	}
	else if (filter.Model == -1)
	{
		sql = "SELECT * FROM cars WHERE class = " + std::to_string(filter.Class) + ";";
	}
	else 
	{
		sql = "SELECT * FROM cars WHERE model = " + std::to_string(filter.Model) + ";";
    }*/
   // sql = std::string("SELECT * FROM cars limit 20 ") + "offset " + std::to_string(20 * (page - 1)) + ";";

    // PGconn* pg = ConnectionPool::Get()->getConnection();
    ConnectionPool* pPool = ConnectionPool::Get();
    PGconn* pg = pPool->getConnection();

    PGresult* res = PQexec(pg, sql.c_str());
	  if (PQresultStatus(res) != PGRES_TUPLES_OK)
	  {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Error: Failed to get cars: %s", PQerrorMessage(pg));
	      PQclear(res);
        pPool->releaseConnection(pg);
		    return;
	  }
    //PQfinish(pg);
    pPool->releaseConnection(pg);
    
	  _ParseGPResult(res, cars);
}

void CarManager::GetNumCarsBySerie(const std::string& serie, std::vector<int>& outCounts)
{
    auto models = ESerie::ModelsToArray(serie);

    PGconn* mPG = ConnectionPool::Get()->getConnection();
    for (auto m : models)
    {
        std::string sql = "SELECT COUNT(*) FROM cars WHERE model='"
            + m + "';";

        PGresult* res = PQexec(mPG, sql.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            char* err = PQerrorMessage(mPG);
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
            PQclear(res);
            ConnectionPool::Get()->releaseConnection(mPG);
            //exit_nicely(conn);
            return;
        }
        char* temp = (char*)calloc(256, sizeof(char));
        strcpy(temp, PQgetvalue(res, 0, 0));
        outCounts.push_back(atoi(temp));
        free(temp);
        PQclear(res);
    }
    ConnectionPool::Get()->releaseConnection(mPG);
}

void CarManager::GetNumCars(const std::string& make, std::vector<int>& outCounts)
{
    int start = 0;
    int end = 0;
    auto series = EMake::SeriesToArray(make);

    PGconn* mPG = ConnectionPool::Get()->getConnection();
    for (auto m : series)
    {
        std::string sql = "SELECT COUNT(*) FROM cars WHERE class='"
            + m + "';";

        PGresult* res = PQexec(mPG, sql.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            char* err = PQerrorMessage(mPG);
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
            PQclear(res);
            //exit_nicely(conn);
            ConnectionPool::Get()->releaseConnection(mPG);
            return;
        }
        char* temp = (char*)calloc(256, sizeof(char));
        strcpy(temp, PQgetvalue(res, 0, 0));
        outCounts.push_back(atoi(temp));
        free(temp);
        PQclear(res);
    }
    ConnectionPool::Get()->releaseConnection(mPG);
}

void CarManager::GetNumCars(std::vector<int>& outCounts)
{

    PGconn* mPG = ConnectionPool::Get()->getConnection();
    std::vector<std::string> makes = EMake::ToArray();
    for (std::string& m : makes) {
        std::string sql = "SELECT COUNT(*) FROM cars WHERE make='"
            + m + "';";

        PGresult* res = PQexec(mPG, sql.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            char* err = PQerrorMessage(mPG);
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
            PQclear(res);
            //exit_nicely(conn);
            ConnectionPool::Get()->releaseConnection(mPG);
            return;
        }
        char* temp = (char*)calloc(256, sizeof(char));
        strcpy(temp, PQgetvalue(res, 0, 0));
        outCounts.push_back(atoi(temp));
        free(temp);
        PQclear(res);
    }
    ConnectionPool::Get()->releaseConnection(mPG);
}

void CarManager::GetCars(int userId, std::vector<DBCar*>& cars)
{
	std::string sql = "SELECT * FROM cars WHERE user_id = "
        + std::to_string(userId) + " order by refresh_ts desc;";

  PGconn* mPG = ConnectionPool::Get()->getConnection();//ConnectionPool::Get()->getConnection();
	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
    ConnectionPool::Get()->releaseConnection(mPG);
		//exit_nicely(conn);
		return;
	}
	_ParseGPResult(res, cars);
  ConnectionPool::Get()->releaseConnection(mPG);

	/*char* temp = (char*)calloc(256, sizeof(char));

	int rec_count = PQntuples(res);
	for (int i = 0; i < rec_count; i++)
	{
		DBCar* pCar = new DBCar();
		strcpy(temp, PQgetvalue(res, i, 0));
		pCar->Id = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 1));
		pCar->UserId = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 2));
		pCar->Make = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 3));
		pCar->Class = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 4));
		pCar->Model = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 5));
		pCar->Submodel = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 6));
		pCar->Country = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 7));
		pCar->Province = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 8));
		pCar->SubProvince = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 9));
		pCar->Price = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 10));
		pCar->BodyType = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 11));
		pCar->Year = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 12));
		pCar->EngineType = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 13));
		pCar->EngineSize = atof(temp);

		strcpy(temp, PQgetvalue(res, i, 14));
		pCar->EnginePower = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 15));
		pCar->Acc0To100 = atof(temp);

		strcpy(temp, PQgetvalue(res, i, 16));
		pCar->Transmission = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 17));
		pCar->StearingWheel = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 18));
		pCar->Exchange = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 19));
		pCar->CustomsCleared = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 20));
		pCar->Color = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 21));
		pCar->Avatar = (temp);

		strcpy(temp, PQgetvalue(res, i, 23));
		pCar->OnSale = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 24));
		pCar->OnTop = atoi(temp);
		
		{
			std::string sql = "SELECT image_path FROM car_images WHERE car_id = "
				+ std::to_string(pCar->Id) + ";";

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
					pCar->Images.push_back(std::string(tmp));
				}
				free(tmp);
			}
			PQclear(res);
		}

		cars.push_back(pCar);
	}
	free(temp);
	PQclear(res);*/
}

bool CarManager::_ParseGPResult(PGresult* res, std::vector<DBCar*>& cars)
{
	char* temp = (char*)calloc(4096, sizeof(char));

  ConnectionPool* pPool = ConnectionPool::Get();
  PGconn* pConn = pPool->getConnection();
	int rec_count = PQntuples(res);
	for (int i = 0; i < rec_count; i++)
	{
		DBCar* pCar = new DBCar();
		strcpy(temp, PQgetvalue(res, i, 0));
		pCar->Id = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 1));
		pCar->UserId = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 2));
    pCar->Make = temp;

		strcpy(temp, PQgetvalue(res, i, 3));
    pCar->Class = temp;

		strcpy(temp, PQgetvalue(res, i, 4));
    pCar->Model = temp;

		strcpy(temp, PQgetvalue(res, i, 5));
		pCar->Submodel = temp;

		strcpy(temp, PQgetvalue(res, i, 6));
		pCar->Country = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 7));
		pCar->Province = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 8));
		pCar->SubProvince = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 9));
		pCar->Price = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 10));
		pCar->BodyType = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 11));
		pCar->Year = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 12));
		pCar->EngineType = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 13));
		pCar->EngineSize = atof(temp);

		strcpy(temp, PQgetvalue(res, i, 14));
		pCar->EnginePower = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 15));
		pCar->DriveType = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 16));
		pCar->Transmission = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 17));
		pCar->StearingWheel = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 18));
		pCar->Exchange = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 19));
		pCar->CustomsCleared = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 20));
		pCar->Color = atoi(temp);

    /*strcpy(temp, PQgetvalue(res, i, 21));
    pCar->Avatar = (temp);*/

    strcpy(temp, PQgetvalue(res, i, 21));
		pCar->OnSale = atoi(temp);

    strcpy(temp, PQgetvalue(res, i, 22));
		pCar->OnTop = atoi(temp);

    // strcpy(temp, PQgetvalue(res, i, 23));
    // pCar->Avatar = (temp);

    strcpy(temp, PQgetvalue(res, i, 24));
    pCar->Mileage = atoi(temp);

    strcpy(temp, PQgetvalue(res, i, 25));
    pCar->Description = temp;

    strcpy(temp, PQgetvalue(res, i, 26));
    pCar->AvatarImageId = atoi(temp);

    strcpy(temp, PQgetvalue(res, i, 27));
    pCar->RefreshTs = std::strtoull(temp, nullptr, 10);

    pCar->Rank = GetCarStars(pCar->Id);

		{
        std::string sql = "SELECT image_path, id FROM car_images WHERE car_id = " + std::to_string(pCar->Id) + ";";

        PGresult* res = PQexec(pConn, sql.c_str());
			  if (PQresultStatus(res) != PGRES_TUPLES_OK)
			  {
            char* err = PQerrorMessage(pConn);
            fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
				    PQclear(res);
  				  //exit_nicely(conn);
			  }
			  else
			  {
          int imageCount = PQntuples(res);
				  char* tmp = (char*)calloc(256, sizeof(char));
				  for (int img = 0; img < imageCount; img++)
				  {
            DBCarImage im;
            strcpy(tmp, PQgetvalue(res, img, 0));
            im.ImagePath = tmp;
            strcpy(tmp, PQgetvalue(res, img, 1));
            im.Id = atoi(tmp);
            pCar->Images.push_back(im);
				  }
				  free(tmp);
			  }
			  PQclear(res);
		}
    cars.push_back(pCar);
	}

	free(temp);
	// PQclear(res);
  //ConnectionPool::Get()->releaseConnection(pConn);
  pPool->releaseConnection(pConn);
	return true;
}

bool CarManager::SetCarAvatar(int carId, int imageId)
{
    std::string sql = "UPDATE cars SET avatar_image_id = '" + std::to_string(imageId) + "' WHERE id = " + std::to_string(carId) + ";";
    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Add car image failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return false;
    }

    ConnectionPool::Get()->releaseConnection(pConn);
    return true;
}

bool CarManager::SetCarAvatar(int carId, const std::string& avatarPath)
{
    PGconn* mPG = ConnectionPool::Get()->getConnection();
	std::string sql = "UPDATE cars SET avatar = '" + avatarPath + "' WHERE id = " + std::to_string(carId) + ";";
	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "Add car image failed: %s", PQerrorMessage(mPG));
		PQclear(res);
    ConnectionPool::Get()->releaseConnection(mPG);
		return false;
	}

  ConnectionPool::Get()->releaseConnection(mPG);
	return true;
}

#include "../stlplus/file_system.hpp"
bool CarManager::DeleteCarImage(int carId, int imageId)
{
    PGconn* pg = ConnectionPool::Get()->getConnection();


    std::string sql = "SELECT image_path FROM car_images WHERE id="
        //+ std::to_string(carId) + " AND id="
        + std::to_string(imageId) + ""
        + ";";

    {
        PGresult* res = PQexec(pg, sql.c_str());
        int rec_count = PQntuples(res);
        if (rec_count)
        {
            char* temp = (char*)calloc(256, sizeof(char));
            strcpy(temp, PQgetvalue(res, 0, 0));
            bool b = stlplus::file_delete(temp);
            std::string op = stlplus::folder_part(temp) +"/" + stlplus::basename_part(temp) + "_orig.jpg";
            bool b1 = stlplus::file_delete(op);
            free(temp);
        }
    }
    sql = "DELETE FROM car_images WHERE car_id="
        + std::to_string(carId) + " AND id="
        + std::to_string(imageId) + ""
        + ";";
    PGresult* res = PQexec(pg, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        char* err = PQerrorMessage(pg);
        fprintf(stderr, "Delete car image failed: %s", PQerrorMessage(pg));
        PQclear(res);
         // PQfinish(pg);
        ConnectionPool::Get()->releaseConnection(pg);
        return false;
    }
    PQclear(res);
    //PQfinish(pg);
    ConnectionPool::Get()->releaseConnection(pg);
    return true;
}

int CarManager::AddCarImage(int carId, const std::string& imagePath)
{

	std::string prefix = "/var/www/";
	size_t pos = imagePath.find(prefix);
	std::string url = imagePath;
        if (pos != std::string::npos) 
	{
	    url = url.substr(pos + prefix.length());
	}
	std::string sql = "INSERT INTO car_images(car_id, image_path) VALUES ("
		+ std::to_string(carId) + ", '"
		+ url + "'"
		+ ");";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
        fprintf(stderr, "Add car image failed: %s", PQerrorMessage(pConn));
		PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return -1;
	}

    sql = "SELECT currval('car_images_id_seq');";
    res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(pConn);
        return -1;
    }
    char* temp = (char*)calloc(256, sizeof(char));
    int rec_count = PQntuples(res);
    strcpy(temp, PQgetvalue(res, 0, 0));
    int id = atoi(temp);
    free(temp);
    ConnectionPool::Get()->releaseConnection(pConn);
    return id;
}

int CarManager::GetCarUserVoteStars(int carId, int userId)
{
    PGconn* mPG = ConnectionPool::Get()->getConnection();
    std::string sql = "SELECT num_stars FROM car_stars WHERE car_id = " + std::to_string(carId) + " AND user_id=" + std::to_string(userId) +  ";";
    PGresult* res = PQexec(mPG, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(mPG);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(mPG);
        return 0;
        //exit_nicely(conn);
    }
    int selectCount = PQntuples(res);
    if (!selectCount) 
    {
      ConnectionPool::Get()->releaseConnection(mPG);
      return 0;
    }

    int sumStars = 0;
    char* tmp = (char*)calloc(256, sizeof(char));
    strcpy(tmp, PQgetvalue(res, 0, 0));
    sumStars = atoi(tmp);
    free(tmp);
    ConnectionPool::Get()->releaseConnection(mPG);
    PQclear(res);
    return sumStars;
}

int CarManager::GetCarStars(int carId)
{
    PGconn* mPG = ConnectionPool::Get()->getConnection();
    std::string sql = "SELECT num_stars FROM car_stars WHERE car_id = " + std::to_string(carId) + ";";
    PGresult* res = PQexec(mPG, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        char* err = PQerrorMessage(mPG);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
        PQclear(res);
        ConnectionPool::Get()->releaseConnection(mPG);
        return 0;
        //exit_nicely(conn);
    }
    int imageCount = PQntuples(res);
    if (!imageCount)
    {
       PQclear(res);
       ConnectionPool::Get()->releaseConnection(mPG);
       return 0;
    }
    int sumStars = 0;
    char* tmp = (char*)calloc(256, sizeof(char));
    for (int img = 0; img < imageCount; img++)
    {
        strcpy(tmp, PQgetvalue(res, img, 0));
        sumStars += atoi(tmp);
    }
    free(tmp);

    PQclear(res);
    ConnectionPool::Get()->releaseConnection(mPG);
    return sumStars;
}

void CarManager::Refresh(int carId)
{
    using namespace std::chrono;

    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//    std::cout << ms << " milliseconds since the Epoch\n";

    std::string sql = "UPDATE CARS SET refresh_ts="
        + std::to_string(ms)
        + " WHERE id="
        + std::to_string(carId)
        + ";";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
//        char* err = PQerrorMessage(mPG);
        fprintf(stderr, "SELECT failed: %s", PQerrorMessage(pConn));
//        PQclear(res);
//        return;
        //exit_nicely(conn);
    }
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
}

void CarManager::RequestModel(int userId, const std::string& msg)
{
    std::string sql = "INSERT INTO CAR_MODEL_REQS(user_id, msg) VALUES("
        + std::to_string(userId)
        + ", '"
        + msg
        + "');";

    PGconn* pConn = ConnectionPool::Get()->getConnection();
    PGresult* res = PQexec(pConn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error: Failed to create model request: %s", PQerrorMessage(pConn));
    }
    PQclear(res);
    ConnectionPool::Get()->releaseConnection(pConn);
}

void CarManager::ToJson(int totalNumCars, const std::vector<DBCar*> cars, std::string& json)
{
	rapidjson::Document d;
    d.SetObject();
    rapidjson::Value tnc;
    tnc.SetInt(totalNumCars);
    d.AddMember("totalNumCars", tnc, d.GetAllocator());

    rapidjson::Value vcars;
    vcars.SetArray();
	for (auto pCar : cars)
	{
		rapidjson::Value o;
		o.SetObject();
		rapidjson::Value v;

		v.SetInt(pCar->Id);
		o.AddMember("id", v, d.GetAllocator());

		v.SetInt(pCar->UserId);
		o.AddMember("user_id", v, d.GetAllocator());

        v.SetString(pCar->Make.c_str(), d.GetAllocator());
		o.AddMember("make", v, d.GetAllocator());

        v.SetString(pCar->Class.c_str(), d.GetAllocator());
		o.AddMember("class", v, d.GetAllocator());

        v.SetString(pCar->Model.c_str(), d.GetAllocator());
		o.AddMember("model", v, d.GetAllocator());

		v.SetString(pCar->Submodel.c_str(), d.GetAllocator());
		o.AddMember("submodel", v, d.GetAllocator());

		v.SetInt(pCar->Country);
		o.AddMember("country", v, d.GetAllocator());

		v.SetInt(pCar->Province);
		o.AddMember("province", v, d.GetAllocator());

		v.SetInt(pCar->SubProvince);
		o.AddMember("sub_province", v, d.GetAllocator());

//		v.SetInt(pCar->Class);
//		o.AddMember("class", v, d.GetAllocator());

//		v.SetInt(pCar->Model);
//		o.AddMember("model", v, d.GetAllocator());

		v.SetInt(pCar->Price);
		o.AddMember("price", v, d.GetAllocator());

		v.SetInt(pCar->BodyType);
		o.AddMember("body_type", v, d.GetAllocator());

		v.SetInt(pCar->Year);
		o.AddMember("year", v, d.GetAllocator());

		v.SetInt(pCar->EngineType);
		o.AddMember("engine_type", v, d.GetAllocator());

        v.SetFloat(pCar->EngineSize);
		o.AddMember("engine_size", v, d.GetAllocator());

		v.SetInt(pCar->EnginePower);
		o.AddMember("engine_power", v, d.GetAllocator());

		v.SetInt(pCar->DriveType);
		o.AddMember("drive_type", v, d.GetAllocator());

		v.SetInt(pCar->Transmission);
		o.AddMember("transmission", v, d.GetAllocator());

		v.SetInt(pCar->StearingWheel);
		o.AddMember("stearing_wheel", v, d.GetAllocator());

		v.SetInt(pCar->Exchange);
		o.AddMember("exchange", v, d.GetAllocator());

		v.SetInt(pCar->CustomsCleared);
		o.AddMember("customs_cleared", v, d.GetAllocator());

		v.SetInt(pCar->Color);
		o.AddMember("color", v, d.GetAllocator());

        v.SetInt(pCar->Mileage);
        o.AddMember("mileage", v, d.GetAllocator());

        v.SetString(pCar->Description.c_str(), d.GetAllocator());
        o.AddMember("description", v, d.GetAllocator());

//		v.SetString(pCar->Avatar.c_str(), d.GetAllocator());
//		o.AddMember("avatar", v, d.GetAllocator());

        v.SetInt(pCar->AvatarImageId);
        o.AddMember("avatar_image_id", v, d.GetAllocator());

		v.SetInt(pCar->OnSale);
		o.AddMember("on_sale", v, d.GetAllocator());

		v.SetInt(pCar->OnTop);
		o.AddMember("on_top", v, d.GetAllocator());

        v.SetDouble(pCar->RefreshTs);
        o.AddMember("refresh_ts", v, d.GetAllocator());

        v.SetInt(pCar->Rank);
        o.AddMember("rank", v, d.GetAllocator());

		v.SetArray();
		for (auto& i : pCar->Images)
		{
            rapidjson::Value vcar;
            vcar.SetObject();
            rapidjson::Value va;
            va.SetString(i.ImagePath.c_str(), d.GetAllocator());
            vcar.AddMember("uri", va, d.GetAllocator());
            va.SetInt(i.Id);
            vcar.AddMember("id", va, d.GetAllocator());

            v.PushBack(vcar, d.GetAllocator());
		}
		o.AddMember("images", v,d.GetAllocator());

        vcars.PushBack(o, d.GetAllocator());
	}
    d.AddMember("cars", vcars, d.GetAllocator());
	
	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	json = buffer.GetString();
}


bool CarManager::UpdateCarStars(int userId, int carId, int numStars)
{
    {
        std::string sql = "SELECT num_stars FROM car_stars WHERE user_id = " + std::to_string(userId) + " AND car_id=" + std::to_string(carId) + ";";
        PGconn* mPG = ConnectionPool::Get()->getConnection();
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
            if (!imageCount)
            {
                std::string sql = "INSERT INTO car_stars(user_id, car_id, num_stars) VALUES ("
                    + std::to_string(userId) + ", "
                    + std::to_string(carId) + ", "
                    + std::to_string(numStars)
                    + ");";

                PGresult* res = PQexec(mPG, sql.c_str());
                if (PQresultStatus(res) != PGRES_COMMAND_OK)
                {
                    char* err = PQerrorMessage(mPG);
                    fprintf(stderr, "Update car stars failed: %s", PQerrorMessage(mPG));
                    PQclear(res);
                    ConnectionPool::Get()->releaseConnection(mPG);
                    return false;
                }
                ConnectionPool::Get()->releaseConnection(mPG);
                return true;
            }

            std::string sql = "UPDATE car_stars SET num_stars=" + std::to_string(numStars) + " WHERE user_id=" + std::to_string(userId) + " AND car_id=" + std::to_string(carId) + ";";
            PGresult* res = PQexec(mPG, sql.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
                char* err = PQerrorMessage(mPG);
                fprintf(stderr, "Update car stars failed: %s", PQerrorMessage(mPG));
                PQclear(res);
                ConnectionPool::Get()->releaseConnection(mPG);
                return false;
            }

//            char* tmp = (char*)calloc(256, sizeof(char));
//            for (int img = 0; img < imageCount; img++)
//            {
//                strcpy(tmp, PQgetvalue(res, img, 0));
//                pCar->Images.push_back(std::string(tmp));
//            }
//            free(tmp);
            PQclear(res);
            ConnectionPool::Get()->releaseConnection(mPG);
            return true;
        }
        ConnectionPool::Get()->releaseConnection(mPG);
    }

//    std::string sql = "INSERT INTO car_stars(user_id, car_id, num_stars) VALUES ("
//        + std::to_string(userId) + ", "
//        + std::to_string(carId) + ", "
//        + std::to_string(numStars)
//        + ");";

//    PGresult* res = PQexec(mPG, sql.c_str());
//    if (PQresultStatus(res) != PGRES_COMMAND_OK)
//    {
//        char* err = PQerrorMessage(mPG);
//        fprintf(stderr, "Update car stars failed: %s", PQerrorMessage(mPG));
//        PQclear(res);
//        return false;
//    }
   
    return true;
}
