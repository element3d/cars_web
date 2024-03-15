#ifndef __USERS_ROUTE__
#define __USERS_ROUTE__

#include "../httplib.h"

class UsersRoute
{
public:
    static UsersRoute* Get();

    std::function<void(const httplib::Request &, httplib::Response &)> Me();
    std::function<void(const httplib::Request &, httplib::Response &)> MeDelete();
    std::function<void(const httplib::Request &, httplib::Response &)> MeHandshake();
    std::function<void(const httplib::Request &, httplib::Response &)> MeUploadAvatar();
    std::function<void(const httplib::Request &, httplib::Response &)> MeUpdateAvatar();
    std::function<void(const httplib::Request &, httplib::Response &)> MeUpdateCover();
    std::function<void(const httplib::Request &, httplib::Response &)> GetUser();
    std::function<void(const httplib::Request &, httplib::Response &)> UserHandshake();
    std::function<void(const httplib::Request &, httplib::Response &)> EditUser();
    std::function<void(const httplib::Request &, httplib::Response &)> GetUserCars();
    std::function<void(const httplib::Request &, httplib::Response &)> UserGetAutoPartMakes();
    std::function<void(const httplib::Request &, httplib::Response &)> UserSetAutoPartMakes();
    std::function<void(const httplib::Request &, httplib::Response &)> UserGetAutoPartCategories();
    std::function<void(const httplib::Request &, httplib::Response &)> UserSetAutoPartCategories();
    std::function<void(const httplib::Request &, httplib::Response &)> GetUserNumGolds();
    std::function<void(const httplib::Request &, httplib::Response &)> UserEarnGold();
    std::function<void(const httplib::Request &, httplib::Response &)> UserGetGifts();
    std::function<void(const httplib::Request &, httplib::Response &)> UserReceiveGift();
    std::function<void(const httplib::Request &, httplib::Response &)> UserAddDevice();
private:
    static UsersRoute* sInstance;
};

#endif // __USERS_ROUTE__
