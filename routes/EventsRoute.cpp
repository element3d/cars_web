#include "EventsRoute.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

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