#ifndef __USERS_ROUTE__
#define __USERS_ROUTE__

#include "../httplib.h"

class UsersRoute
{
public:
    static UsersRoute* Get();

    std::function<void(const httplib::Request &, httplib::Response &)> Me();


private:
    static UsersRoute* sInstance;
};

#endif // __USERS_ROUTE__
