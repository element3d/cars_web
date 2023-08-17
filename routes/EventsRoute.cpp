#include "EventsRoute.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <jwt-cpp/jwt.h>

EventsRoute* EventsRoute::sInstance = nullptr;

EventsRoute* EventsRoute::Get()
{
    if (!sInstance) sInstance = new EventsRoute();
    return sInstance;
}

std::function<void(const httplib::Request &, httplib::Response &)> EventsRoute::GetInception()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        DBInception* pI = EventsManager::Get()->GetInception();
        if (!pI) 
        {
            res.status = 404;
            res.set_content("", "text/plain");
            return;
        }

        res.status = 200;
        res.set_content(pI->ToJson(), "text/plain");
        delete pI;
    };
}

std::function<void(const httplib::Request&, httplib::Response&)> EventsRoute::GetPitStop()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        DBInception* pI = EventsManager::Get()->GetPitStop();
        if (!pI)
        {
            res.status = 404;
            res.set_content("", "text/plain");
            return;
        }

        res.status = 200;
        res.set_content(pI->ToJson(), "text/plain");
        delete pI;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> EventsRoute::EventsSetUser()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int eventId = -1;
        if (req.has_param("event_id")) eventId = atoi(req.get_param_value("event_id", 0).c_str());

        EventsManager::Get()->EventsSetUser(userId, eventId, req.body);
      
        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> EventsRoute::EventsSetUserCard()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int eventId = -1;
        if (req.has_param("event_id")) eventId = atoi(req.get_param_value("event_id", 0).c_str());

        EventsManager::Get()->EventsSetUserCard(userId, eventId, req.body);
      
        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}