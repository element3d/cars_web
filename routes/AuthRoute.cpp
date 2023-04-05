#include "AuthRoute.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
//#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"

AuthRoute* AuthRoute::sInstance = nullptr;

AuthRoute* AuthRoute::Get()
{
    if (!sInstance) sInstance = new AuthRoute();
    return sInstance;
}

int sign_in(const std::string& username, const std::string& password, std::string& token)
{
    DBUser* pUser = UserManager::Get()->GetUser(username);
    if (!pUser)
    {
        return 404;
    }


    if (pUser->Password != password) return 403;

    token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_payload_claim("username", picojson::value(pUser->Username))
        .set_payload_claim("id", picojson::value(int64_t(pUser->Id)))
        .set_payload_claim("type", picojson::value(int64_t(pUser->Type)))
        .sign(jwt::algorithm::hs256{"secret"});

    std::cout << "TOKEN " << token << std::endl;
    delete pUser;
    return 200;
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignUp()
{
    return [](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");
        std::cout << req.body;

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string username = document["username"].GetString();
        std::string phone = document["phone"].GetString();
        std::string pwd = document["password"].GetString();
		int type = document["type"].GetInt();
		std::string firstName = document["first_name"].GetString();
		std::string secondName = document["second_name"].GetString();

        int userId = UserManager::Get()->CreateUser(username, phone, pwd, type, firstName, secondName);

        res.status = 200;
        res.set_content(std::to_string(userId), "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignIn()
{
    return [](const httplib::Request& req, httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
     res.set_header("Access-Control-Allow-Methods", "*");
      res.set_header("Access-Control-Allow-Headers", "*");

        std::cout << req.body;

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string username = document["username"].GetString();
        std::string pwd = document["password"].GetString();

        std::string token;
        int status = sign_in(username, pwd, token);
       // res.set_header("Access-Control-Allow-Origin", "*");
		if (!token.size())
		{
      res.status = status;
			return;
		}
        res.status = 200;
        res.set_content(token, "text/plain");
    };
}
