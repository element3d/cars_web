#include "CarManager.h"
#include <iostream>


CarManager* CarManager::sInstance = nullptr;

CarManager* CarManager::Get()
{
	if (!sInstance) sInstance = new CarManager();

	return sInstance;
}

void CarManager::SetPG(PGconn* pPG)
{
	mPG = pPG;
}

int CarManager::CreateCar(const std::string& carJson) 
{
	rapidjson::Document d;
	d.Parse(carJson.c_str());

	std::string sql = "INSERT INTO cars(user_id , make, class, model, submodel, country, province, sub_province, price, body_type, year, engine_type, engine_size, engine_power, acc_0_100, transmission, stearing_wheel, exchange, customs_cleared, color, on_sale, on_top) VALUES ("
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
		+ std::to_string(d["acc_0_100"].GetFloat()) + ", "
		+ std::to_string(d["transmission"].GetInt()) + ", "
		+ std::to_string(d["stearing_wheel"].GetInt()) + ", "
		+ std::to_string(d["exchange"].GetInt()) + ", "
		+ std::to_string(d["customs_cleared"].GetInt()) + ", "
		+ std::to_string(d["color"].GetInt()) + ", "
		//+ std::string(d["avatar"].GetString()) + "', '"
		//+ std::string(d["images"].GetString()) + "', "
		+ std::to_string(d["on_sale"].GetInt()) + ", "
		+ std::to_string(d["on_top"].GetInt()) + ""
		");";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return -1;
	}

	sql = "SELECT currval('cars_id_seq');";
	res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return -1;
	}
	char* temp = (char*)calloc(256, sizeof(char));
	int rec_count = PQntuples(res);
	strcpy(temp, PQgetvalue(res, 0, 0));
	int id = atoi(temp);
	free(temp);
	return id;
}

void CarManager::GetTopCarsBySerie(int serie, std::vector<DBCar*>& cars)
{
	std::string sql;
	if (serie == -1)
		sql = "SELECT * FROM cars WHERE on_sale = 1;";
	else
		sql = "SELECT * FROM cars WHERE on_sale = 1 AND class = " + std::to_string(serie) + ";";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}

	_ParseGPResult(res, cars);
}

void CarManager::GetTopCarsByMake(int make, std::vector<DBCar*>& cars)
{
	std::string sql;
	if (make == -1)
		sql = "SELECT * FROM cars WHERE on_sale = 1;";
	else
		sql = "SELECT * FROM cars WHERE on_sale = 1 AND make = " + std::to_string(make) + ";";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}

	_ParseGPResult(res, cars);
}

void CarManager::GetCars(const CarFilter& filter, std::vector<DBCar*>& cars)
{
	std::string sql;
	if (filter.Make == -1)
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
	}

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}
	_ParseGPResult(res, cars);
}

void CarManager::GetCars(int userId, std::vector<DBCar*>& cars)
{
	std::string sql = "SELECT * FROM cars WHERE user_id = "
		+ std::to_string(userId) + ";";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}
	_ParseGPResult(res, cars);

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
	char* temp = (char*)calloc(256, sizeof(char));

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
	PQclear(res);
	return true;
}

bool CarManager::SetCarAvatar(int carId, const std::string& avatarPath)
{
	std::string sql = "UPDATE cars SET avatar = '" + avatarPath + "' WHERE id = " + std::to_string(carId) + ";";
	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "Add car image failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return false;
	}

	return true;
}

bool CarManager::AddCarImage(int carId, const std::string& imagePath) 
{
	std::string sql = "INSERT INTO car_images(car_id, image_path) VALUES ("
		+ std::to_string(carId) + ", '"
		+ imagePath + "'"
		+ ");";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "Add car image failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return false;
	}

	return true;
}