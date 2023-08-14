#ifndef __CAR_MANAGER__
#define __CAR_MANAGER__


#include <libpq-fe.h>
//#include <postgresql/libpq-fe.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <vector>
#include <string>
#include "EMake.h"

struct DBCarImage
{
    int Id;
    std::string ImagePath;
};

struct DBCar
{
	int Id;
	int UserId;
    std::string Make;
    std::string Class;
    std::string Model;
	  std::string Submodel;
	int Country;
	int Province;
	int SubProvince;
	int Price;
	int BodyType;
	int Year;
	int EngineType;
	float EngineSize;
	int EnginePower;
	int DriveType;
	int Transmission;
	int StearingWheel;
	bool Exchange;
	bool CustomsCleared;
	int Color;
    int Mileage;
    std::string Description;
    //std::string Avatar;
    int AvatarImageId;
    std::vector<DBCarImage> Images;
	bool OnSale;
    int OnTop;
    uint64_t  RefreshTs;
    int Rank;
};

struct CarFilter
{
    int Province;
    std::string Make;
    std::string Class;
    std::string Model;
    std::string SubModel;
    int PriceFrom;
    int PriceTo;
    int YearFrom;
    int YearTo;
    int BodyType;
    int DriveType;
    int EngineType;
    int EngineSizeFrom;
    int EngineSizeTo;
    int Transmission;
    int SteeringWheel;
    int Color;
    int CustomsCleared;
    int Exchange;
    int OnTop;
    int Page;
    int View;
    int OnSale;
    int Limit;
	/*int Country;
	int Province;
	int SubProvince;

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

//	void SetPG(PGconn* pPG);
    int CreateCar(int userId, const std::string& carJson);
    bool EditCar(int userId, int id, const std::string& carJson);
    bool DeleteCar(int userId, int id);
	  void GetCars(int userId, std::vector<DBCar*>& cars);
    DBCar* GetCar(int carId);
    void GetCars(const CarFilter& filter, int page, std::vector<DBCar*>& cars);
    int GetTotalNumCars(const CarFilter& filter);
    void GetNumCars(std::vector<int>& outCounts);
    void GetNumCars(const std::string& make, std::vector<int>& outCounts);
    void GetNumCarsBySerie(const std::string& serie, std::vector<int>& outCounts);

	  void GetTopCarsByMake(int make, std::vector<DBCar*>& cars);
	  void GetTopCarsBySerie(int serie, std::vector<DBCar*>& cars);
	  bool SetCarAvatar(int carId, const std::string& avatarPath);
    bool SetCarAvatar(int carId, int imageId);
    int AddCarImage(int carId, const std::string& imagePath);
    bool DeleteCarImage(int carId, int imageId);
    bool UpdateCarStars(int userId, int carId, int numStars);
    int GetCarStars(int carId);
    int GetCarUserVoteStars(int carId, int userId);
    void Refresh(int carId);
    void RequestModel(int userId, const std::string& msg);
    void ToJson(int totalNumCars, const std::vector<DBCar*> cars, std::string& json);
private:
	bool _ParseGPResult(PGresult* res, std::vector<DBCar*>& cars);

private:
	static CarManager* sInstance;
//	PGconn* mPG = nullptr;
};

#endif // __USER_MANAGER__
