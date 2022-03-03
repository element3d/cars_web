#ifndef __CARS_ROUTE__
#define __CARS_ROUTE__

#include "../httplib.h"
#include "../managers/CarManager.h"

class CarsRoute
{
public:
	static CarsRoute* Get();

	std::function<void(const httplib::Request &, httplib::Response &)> MyCars();
	std::function<void(const httplib::Request &, httplib::Response &)> TopCars();
	std::function<void(const httplib::Request &, httplib::Response &)> CarsGet();
	std::function<void(const httplib::Request &, httplib::Response &)> CarsPost();
	std::function<void(const httplib::Request &, httplib::Response &)> CarsUploadAvatar();
	std::function<void(const httplib::Request &, httplib::Response &)> CarsUploadImage();

private:
	void ToJson(const std::vector<DBCar*> cars, std::string& json);

private:
	static CarsRoute* sInstance;
};

#endif // __CARS_ROUTE__
