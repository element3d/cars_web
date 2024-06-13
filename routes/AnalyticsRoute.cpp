#include "AnalyticsRoute.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
//#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"
#include "../managers/PQManager.h"

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
        std::string deviceId = document["deviceId"].GetString();
        if (deviceId == "gh8rctmll7hiea9gwtosoo")
        {
            res.status = 200;
            return;
        }

        int userId = document["userId"].GetInt();
        std::string page = document["page"].GetString();
        std::string path = document["path"].GetString();
        std::string platform = document["platform"].GetString();
        std::string browser = document["browser"].GetString();

        bool isMobile = document["isMobile"].GetBool();
        using namespace std::chrono;
        uint64_t ts = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        
        std::string sql = "INSERT INTO an_page_views(user_id, page, path, platform, browser, device_id, is_mobile, timestamp) VALUES(" 
            + std::to_string(userId) + ",'" 
            + page + "','"
            + path + "','"
            + platform + "','"
            + browser + "','"
            + deviceId + "',"
            + std::to_string((int)isMobile) + ","
            + std::to_string(ts) 
            + ");";

        PGconn* pg = ConnectionPool::Get()->getConnection();
        PGresult* ret = PQexec(pg, sql.c_str());
        if (PQresultStatus(ret) != PGRES_COMMAND_OK)
        {
            fprintf(stderr, "Failed to add Inception user: %s", PQerrorMessage(pg));
            PQclear(ret);
            ConnectionPool::Get()->releaseConnection(pg);
            return;
        }

        PQclear(ret);
        ConnectionPool::Get()->releaseConnection(pg);
        res.status = 200;
    };
}