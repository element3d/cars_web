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
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartPost();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartGet();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartDelete();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartUploadAvatar();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartResponseUploadAvatar();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartResponsePost();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartResponseGet();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsGetNotifications();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsGetNotification();
    std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsDeleteNotification();
	std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsPost();
	std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsUploadAvatar();
	std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsUploadImage();

private:
    void ToJson(const std::vector<DBAutoPartRequest*> cars, std::string& json);

private:
	static AutoPartsRoute* sInstance;
};

#endif // __CARS_ROUTE__
