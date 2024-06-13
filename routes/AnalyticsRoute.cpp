#include "AnalyticsRoute.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
//#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"

AnalyticsRoute* AnalyticsRoute::sInstance = nullptr;

AnalyticsRoute* AnalyticsRoute::Get()
{
    if (!sInstance) sInstance = new AnalyticsRoute();
    return sInstance;
}

std::function<void(const httplib::Request &, httplib::Response &)> AnalyticsRoute::PostPage()
{
    return [](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string platform = document["platform"].GetString();
        std::string browser = document["browser"].GetString();

        res.status = 200;
    };
}