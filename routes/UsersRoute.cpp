#include "UsersRoute.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"

UsersRoute* UsersRoute::sInstance = nullptr;

UsersRoute* UsersRoute::Get()
{
    if (!sInstance) sInstance = new UsersRoute();
    return sInstance;
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::Me()
{
    return [](const httplib::Request& req, httplib::Response& res){
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        std::string phone = decoded.get_payload_claim("phone").as_string();
        int userId = decoded.get_payload_claim("id").as_int();

        DBUser* pUser = UserManager::Get()->GetUser(userId);
        //for(auto& e : decoded.get_payload_claims())
          //  std::cout << e.first << " = " << e.second << std::endl;

        res.status = 200;
        std::string json = pUser->ToJson();
        res.set_content(json, "text/plain");
        delete pUser;

    };
}

