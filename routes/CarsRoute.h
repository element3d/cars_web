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
    std::function<void(const httplib::Request &, httplib::Response &)> NumCars();
    std::function<void(const httplib::Request &, httplib::Response &)> CarGet();
	  std::function<void(const httplib::Request &, httplib::Response &)> CarsGet();
	  std::function<void(const httplib::Request &, httplib::Response &)> CarsPost();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsPut();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsDelete();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsSetAvatar();
	  std::function<void(const httplib::Request &, httplib::Response &)> CarsUploadAvatar();
	  std::function<void(const httplib::Request &, httplib::Response &)> CarsUploadImage();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsDeleteImage();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsUpdateStars();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsGetStars();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsGetUserVoteStars();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsAddView();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsRefresh();
    std::function<void(const httplib::Request &, httplib::Response &)> CarsToTop();

    std::function<void(const httplib::Request &, httplib::Response &)> CarsRequestModel();

    std::function<void(const httplib::Request&, httplib::Response&)> BandsGet();
    std::function<void(const httplib::Request&, httplib::Response&)> BandGet();


    void _ParseFilter(const httplib::Request& req, CarFilter& filter);


private:
    void ToJson(int totalNumCars, const std::vector<DBCar*> cars, std::string& json);

private:
	static CarsRoute* sInstance;
};

#endif // __CARS_ROUTE__
