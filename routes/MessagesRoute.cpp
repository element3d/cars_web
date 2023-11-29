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
        int page = atoi(req.get_param_value("page", 0).c_str());

        /*rapidjson::Document dd;
        dd.Parse(req.body.c_str());
        int to = dd["to"].GetInt();*/
        rapidjson::Document d;
        bool b = MessagesManager::Get()->MessagesGet(userId, to, page, d);
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

std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::MessagesGetPending()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
        res.set_header("Content-Type", "text/html; charset=utf-8");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
        res.set_header("Access-Control-Allow-Origin", "*");
        
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        long long ts = atoll(req.get_param_value("ts", 0).c_str());
        int convId = atoi(req.get_param_value("conv_id", 0).c_str());


        /*rapidjson::Document dd;
        dd.Parse(req.body.c_str());
        int to = dd["to"].GetInt();*/
        rapidjson::Document d;
        bool b = MessagesManager::Get()->MessagesGetPending(convId, userId, ts, d);
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
        int type = d["type"].GetInt();
        long long ts = d["ts"].GetDouble();

        int cId = MessagesManager::Get()->MessagesPost(convId, userId, to, msg, type, ts);
        if (cId > 0)
        {
            res.status = 200;
            rapidjson::Document d;
            d.SetObject();
            d.AddMember("id", cId, d.GetAllocator());
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
#include "../stlplus/file_system.hpp"
#include "ImageUtils.h"
std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::MessagesUploadImage()
{
     return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
        res.set_header("Content-Type", "text/html; charset=utf-8");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
        res.set_header("Access-Control-Allow-Origin", "*");
        
        if (!req.has_param("message_id"))
        {
            res.status = 206;
            return;
        }

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        std::string messageId = (req.get_param_value("message_id", 0).c_str());
        std::string conversationId = (req.get_param_value("conversation_id", 0).c_str());

        httplib::MultipartFormData image_file = req.get_file_value("image_file");
        if (!image_file.content.size())
        {
             res.status = 200;
             return;
        }

        std::string dataDir = "data";
        // std::string dataDir = "/var/www/data";
        if (!stlplus::folder_exists(dataDir)) stlplus::folder_create(dataDir);
        std::string conversationsDir = dataDir + "/conversations";
        if (!stlplus::folder_exists(conversationsDir)) stlplus::folder_create(conversationsDir);
        std::string conversationDir = conversationsDir + "/" + conversationId;
        if (!stlplus::folder_exists(conversationDir)) stlplus::folder_create(conversationDir);
//        std::string carDir = userDir + "/" + carId;
//        if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
        std::string rn = GenRandomImageName(10);
        std::string filename = conversationDir + "/" + rn + ".webp";
//        std::ofstream ofs(filename, std::ios::binary);
//        ofs << image_file.content;

        EImageContentType ct = EImageContentType::Jpeg;
        if (image_file.content_type == std::string("image/webp")) ct = EImageContentType::Webp;
        DecodeAndSave((unsigned char*)image_file.content.c_str(), image_file.content.size(), filename, ct);

        MessagesManager::Get()->MessagesSetImage(atoi(messageId.c_str()), filename);
        res.status = 200;
        res.set_content(std::string("data/conversations/" + conversationId + "/" + rn + ".webp"), "text/plain");
    };
}


std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::ConversationsGet()
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
        bool b = MessagesManager::Get()->ConversationsGet(userId, d);
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

std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::ConversationsGetPending()
{
    return [](const httplib::Request& req, httplib::Response& res) {
       res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
        res.set_header("Content-Type", "text/html; charset=utf-8");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
        res.set_header("Access-Control-Allow-Origin", "*");
        
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        
        int count = MessagesManager::Get()->ConversationsGetPending(userId);
        res.status = 200;
        res.set_content(std::to_string(count), "text/plain");
       
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::ConversationsPost() 
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
        int to = d["to"].GetInt();

        int cId = MessagesManager::Get()->ConversationsPost(userId, to);
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

std::function<void(const httplib::Request &, httplib::Response &)> MessagesRoute::ConversationsDelete() 
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
        int convId = atoi(req.get_param_value("conversation_id", 0).c_str());


        int cId = MessagesManager::Get()->ConversationsDelete(userId, convId);
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

