#include "MessagesRoute.h"
#include <jwt-cpp/jwt.h>

MessagesRoute* MessagesRoute::sInstance = nullptr;

MessagesRoute* MessagesRoute::Get() 
{
    if (!sInstance) sInstance = new MessagesRoute();
    return sInstance;
}

MessagesRoute::MessagesRoute() {}

std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::MessagesGet()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
        res.set_header("Content-Type", "text/html; charset=utf-8");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
        res.set_header("Access-Control-Allow-Origin", "*");
        
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int to = atoi(req.get_param_value("to", 0).c_str());


        /*rapidjson::Document dd;
        dd.Parse(req.body.c_str());
        int to = dd["to"].GetInt();*/
        rapidjson::Document d;
        bool b = MessagesManager::Get()->MessagesGet(userId, to, d);
        if (b) 
        {
            res.status = 200;
            rapidjson::StringBuffer buffer;
	        buffer.Clear();

	        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	        d.Accept(writer);
	        std::string json = buffer.GetString();
            res.set_content(json, "application/json");
        }
        else 
        {
            res.status = 500;
        }
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::MessagesPost()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
        res.set_header("Content-Type", "text/html; charset=utf-8");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
        res.set_header("Access-Control-Allow-Origin", "*");
        
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();    

        rapidjson::Document d;
        d.Parse(req.body.c_str());
        int convId = d["conv_id"].GetInt();
        int to = d["to"].GetInt();
        std::string msg = d["msg"].GetString();

        int cId = MessagesManager::Get()->MessagesPost(convId, userId, to, msg);
        if (cId > 0)
        {
            res.status = 200;
            rapidjson::Document d;
            d.SetObject();
            d.AddMember("conv_id", cId, d.GetAllocator());
            rapidjson::StringBuffer buffer;
	        buffer.Clear();

	        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	        d.Accept(writer);
	        std::string json = buffer.GetString();
            res.set_content(json, "application/json");
        }
        else 
        {
            res.status = 500;
        }
    };

}
