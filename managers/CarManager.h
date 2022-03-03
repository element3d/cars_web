#ifndef __CAR_MANAGER__
#define __CAR_MANAGER__

#include <libpq-fe.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <vector>
#include <string>

struct DBCar
{
	int Id;
	int UserId;
	int Make;
	int Class;
	int Model;
	int Submodel;
	int Country;
	int Province;
	int SubProvince;
	int Price;
	int BodyType;
	int Year;
	int EngineType;
	float EngineSize;
	int EnginePower;
	float Acc0To100;
	int Transmission;
	int StearingWheel;
	bool Exchange;
	bool CustomsCleared;
	int Color;
	std::string Avatar;
	std::vector<std::string> Images;
	bool OnSale;
	bool OnTop;
};

struct CarFilter
{
	int Make;
	int Class;
	int Model;
	/*int Country;
	int Province;
	int SubProvince;
	int PriceFrom;
	int PriceTo;
	std::vector<int> BodyTypes;*/
	// int Color = ECarColor::All;
	// bool OnSale;
	// bool OnTop;

	/*void SetDefault()
	{
		Country = 0;
		Province = (int)EProvince::All;
		SubProvince = (int)ESubProvince::All;
		PriceFrom = -1;
		PriceTo = -1;
		Color = ECarColor::All;
		OnSale = 0;
		OnTop = 0;
	}*/
};

class CarManager
{
public:
	static CarManager* Get();

	void SetPG(PGconn* pPG);
	int CreateCar(const std::string& carJson);
	void GetCars(int userId, std::vector<DBCar*>& cars);
	void GetCars(const CarFilter& filter, std::vector<DBCar*>& cars);
	void GetTopCarsByMake(int make, std::vector<DBCar*>& cars);
	void GetTopCarsBySerie(int serie, std::vector<DBCar*>& cars);
	bool SetCarAvatar(int carId, const std::string& avatarPath);
	bool AddCarImage(int carId, const std::string& imagePath);

private:
	bool _ParseGPResult(PGresult* res, std::vector<DBCar*>& cars);

private:
	static CarManager* sInstance;
	PGconn* mPG = nullptr;
};

#endif // __USER_MANAGER__
