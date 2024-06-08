#ifndef __AUTH_ROUTE__
#define __AUTH_ROUTE__

#include "../httplib.h"

class AuthRoute
{
public:
    static AuthRoute* Get();

    std::function<void(const httplib::Request &, httplib::Response &)> SignUpVerify();
    std::function<void(const httplib::Request &, httplib::Response &)> SignUp();
    std::function<void(const httplib::Request &, httplib::Response &)> SignUpV2();
    std::function<void(const httplib::Request &, httplib::Response &)> SignIn();
    std::function<void(const httplib::Request &, httplib::Response &)> SignInForgot();
    std::function<void(const httplib::Request &, httplib::Response &)> ChangePassword();
    std::function<void(const httplib::Request &, httplib::Response &)> SignInGoogle();
    std::function<void(const httplib::Request &, httplib::Response &)> SignInGoogleWithEmail();


private:
    static AuthRoute* sInstance;
};

#endif // __AUTH_ROUTE__
