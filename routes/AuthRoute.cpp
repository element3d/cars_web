#include "AuthRoute.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"

AuthRoute* AuthRoute::sInstance = nullptr;

AuthRoute* AuthRoute::Get()
{
    if (!sInstance) sInstance = new AuthRoute();
    return sInstance;
}

std::string sign_in(const std::string& phone, const std::string& password)
{
    DBUser* pUser = UserManager::Get()->GetUser(phone);
	if (!pUser) return "";
    /*std::string sql = "SELECT * FROM users WHERE phone = '"
            + phone + "';";

    pqxx::nontransaction N(*psql);
    pqxx::result R;
    try
    {
        R = N.exec(sql);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return "";
    }

    int id;
    std::string ph;
    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c)
    {
        id = c[0].as<int>();
        ph = c[1].as<std::string>();
    }*/

    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_payload_claim("phone", picojson::value(pUser->Phone))
        .set_payload_claim("id", picojson::value(int64_t(pUser->Id)))
        .sign(jwt::algorithm::hs256{"secret"});

    std::cout << "TOKEN " << token << std::endl;
    delete pUser;
    return token;
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignUp()
{
    return [](const httplib::Request& req, httplib::Response& res){
        std::cout << req.body;

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string phone = document["phone"].GetString();
        std::string pwd = document["password"].GetString();
		int type = document["type"].GetInt();
		std::string firstName = document["first_name"].GetString();
		std::string secondName = document["second_name"].GetString();

        UserManager::Get()->CreateUser(phone, pwd, type, firstName, secondName);

        res.status = 200;
        res.set_content(pwd, "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignIn()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::cout << req.body;

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string phone = document["phone"].GetString();
        std::string pwd = document["password"].GetString();

        std::string token = sign_in(phone, pwd);
		if (!token.size())
		{
			res.status = 403;
			return;
		}
        res.status = 200;
        res.set_content(token, "text/plain");
    };
}
