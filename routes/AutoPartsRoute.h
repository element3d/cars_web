#ifndef __AUTO_PARTS_ROUTE__
#define __AUTO_PARTS_ROUTE__

#include "../httplib.h"
#include "../managers/AutoPartManager.h"

class AutoPartsRoute
{
public:
	static AutoPartsRoute* Get();

	std::function<void(const httplib::Request &, httplib::Response &)> MyAutoParts();
	std::function<void(const httplib::Request &, httplib::Response &)> TopAutoParts();
	std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsPost();
	std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsUploadAvatar();
	std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsUploadImage();

private:
	void ToJson(const std::vector<DBAutoPart*> cars, std::string& json);

private:
	static AutoPartsRoute* sInstance;
};

#endif // __CARS_ROUTE__
