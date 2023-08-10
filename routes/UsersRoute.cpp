#include "UsersRoute.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
//#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"
#include "../stlplus/file_system.hpp"
#include "ImageUtils.h"

UsersRoute* UsersRoute::sInstance = nullptr;

UsersRoute* UsersRoute::Get()
{
    if (!sInstance) sInstance = new UsersRoute();
    return sInstance;
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::Me()
{
    return [](const httplib::Request& req, httplib::Response& res){
      res.set_header("Access-Control-Allow-Methods", " POST, GET, OPTIONS");
			res.set_header("Content-Type", "text/html; charset=utf-8");
			res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Connection", "close");
        /*res.set_header("Access-Control-Allow-Origin", "http://localhost:3000");
           res.set_header("Access-Control-Allow-Methods", "GET");
      res.set_header("Access-Control-Allow-Headers", "authentication");*/
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        DBUser* pUser = UserManager::Get()->GetUser(userId);
        if (!pUser) 
        {
          res.status = 404;
          return;
        }
        //for(auto& e : decoded.get_payload_claims())
          //  std::cout << e.first << " = " << e.second << std::endl;

        res.status = 200;
        std::string json = pUser->ToJson(true);
        res.set_content(json, "text/plain");
        delete pUser;

    };
}

#include "../anvir/avir.h"
#include "../stb_image/stb_image_write.h"
#include "../stb_image/stb_image.h"
#include <webp/decode.h>

void Upload(void * data, int size, std::string fullPath, EImageContentType contentType) {
    int w, h, c;
    unsigned char* d = nullptr;
    if (contentType == EImageContentType::Webp)
    {
      c = 3;
      d = WebPDecodeRGB((const uint8_t*)data, size, &w, &h);
    }
    else
      d = stbi_load_from_memory((unsigned char*)data, size, &w, &h, &c, 0);

    int nw = 400;
    int nh = int(400.f * h / w);
    unsigned char* dd = new unsigned char[nw* nh * c];
    avir :: CImageResizer<> ImageResizer( 8 );
    ImageResizer.resizeImage( d, w, h, 0, dd, nw, nh, c, 0 );

    WebPSave(dd, nw, nh, fullPath);

    // stbi_write_jpg(fullPath.c_str(), nw, nh, c, dd, 100);
    stbi_image_free(d);
    delete[] dd;

}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::MeUploadAvatar()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("user_id"))
        {
            res.status = 206;
            return;
        }


        std::string userId = (req.get_param_value("user_id", 0).c_str());

        httplib::MultipartFormData image_file = req.get_file_value("image_file");
        if (!image_file.content.size())
        {
             res.status = 200;
             return;
        }

        std::string dataDir = "data";
        // std::string dataDir = "/var/www/data";
        if (!stlplus::folder_exists(dataDir)) stlplus::folder_create(dataDir);
        std::string carsDir = dataDir + "/users";
        if (!stlplus::folder_exists(carsDir)) stlplus::folder_create(carsDir);
        std::string userDir = carsDir + "/" + userId;
        if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
//        std::string carDir = userDir + "/" + carId;
//        if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
        std::string filename = userDir + "/avatar.webp";
//        std::ofstream ofs(filename, std::ios::binary);
//        ofs << image_file.content;

        EImageContentType ct = EImageContentType::Jpeg;
        if (image_file.content_type == std::string("image/webp")) ct = EImageContentType::Webp;
        Upload((unsigned char*)image_file.content.c_str(), image_file.content.size(), filename, ct);

        UserManager::Get()->SetUserAvatar(atoi(userId.c_str()), std::string("data/users/") + userId + "/avatar.webp");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::MeUpdateAvatar()
{
    return [](const httplib::Request& req, httplib::Response& res) {
//        if (!req.has_param("user_id"))
//        {
//            res.status = 206;
//            return;
//        }

       res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
     res.set_header("Access-Control-Allow-Origin", "*");
//        std::string userId = (req.get_param_value("user_id", 0).c_str());
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

//        std::string phone = decoded.get_payload_claim("phone").as_string();
        std::string userId = std::to_string(decoded.get_payload_claim("id").as_int());

        httplib::MultipartFormData image_file = req.get_file_value("image_file");

        std::string dataDir = "data";
        // std::string dataDir = "/var/www/data";
        if (!stlplus::folder_exists(dataDir)) stlplus::folder_create(dataDir);
        std::string carsDir = dataDir + "/users";
        if (!stlplus::folder_exists(carsDir)) stlplus::folder_create(carsDir);
        std::string userDir = carsDir + "/" + userId;
        if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
//        std::string carDir = userDir + "/" + carId;
//        if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
        std::string filename = userDir + "/avatar.webp";
//        std::ofstream ofs(filename, std::ios::binary);
//        ofs << image_file.content;

        EImageContentType ct = EImageContentType::Jpeg;
        if (image_file.content_type == std::string("image/webp")) ct = EImageContentType::Webp;
        Upload((unsigned char*)image_file.content.c_str(), image_file.content.size(), filename, ct);

        UserManager::Get()->SetUserAvatar(atoi(userId.c_str()), std::string("data/users/") + userId + "/avatar.webp");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::UserEarnGold()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        UserManager::Get()->UserEarnGold(userId);

        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::UserGetGifts()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        DBGift* pGift = UserManager::Get()->GetUserGift(userId);
        std::string json = "{}";
        if (pGift) json = pGift->ToJson();
        res.status = 200;
        res.set_content(json, "application/json");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::UserReceiveGift()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        rapidjson::Document d;
        d.Parse(req.body.c_str());
        if (!d.HasMember("gift_id"))
        {
            res.status = 404;
            res.set_content("ERROR", "text/plain");
            return;
        }

        int giftId = d["gift_id"].GetInt();

        UserManager::Get()->UserReceiveGift(giftId);
        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::UserGetAutoPartCategories()
{
return [this](const httplib::Request& req, httplib::Response& res) {
    std::string token = req.get_header_value("Authentication");
    auto decoded = jwt::decode(token);

    int userId = decoded.get_payload_claim("id").as_int();
    int type = decoded.get_payload_claim("type").as_int();

    if (type != 1)
    {
        res.status = 401;
        res.set_content("[]", "text/plain");
        return;
    }

    std::vector<int> cats = UserManager::Get()->UserGetAutoPartCategories(userId);

    rapidjson::Document d;
    d.SetArray();
    for (auto& c : cats)
    {
        rapidjson::Value v;
        v.SetInt(c);
        d.PushBack(v, d.GetAllocator());
    }

    rapidjson::StringBuffer buffer;
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    res.status = 200;
    res.set_content(buffer.GetString(), "text/plain");
};
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::UserGetAutoPartMakes()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();
        int type = decoded.get_payload_claim("type").as_int();

        if (type != 1)
        {
            res.status = 401;
            res.set_content("[]", "text/plain");
            return;
        }

        std::vector<std::string> makes = UserManager::Get()->UserGetAutoPartMakes(userId);

        rapidjson::Document d;
        d.SetArray();
        for (auto& make : makes)
        {
            rapidjson::Value v;
            v.SetString(make.c_str(), d.GetAllocator());
            d.PushBack(v, d.GetAllocator());
        }

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        res.status = 200;
        res.set_content(buffer.GetString(), "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::UserSetAutoPartMakes()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();
        int type = decoded.get_payload_claim("type").as_int();

        if (type != 1)
        {
            res.status = 401;
            res.set_content("NOT PERMITTED", "text/plain");
            return;
        }

        rapidjson::Document d;
        d.Parse(req.body.c_str());
        std::vector<std::string> makes;
        for(auto &e : d.GetArray())
        {
            makes.push_back(e.GetString());
        }

        UserManager::Get()->UserSetAutoPartMakes(userId, makes);

        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::UserSetAutoPartCategories()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();
        int type = decoded.get_payload_claim("type").as_int();

        if (type != 1)
        {
            res.status = 401;
            res.set_content("NOT PERMITTED", "text/plain");
            return;
        }

        rapidjson::Document d;
        d.Parse(req.body.c_str());
        std::vector<int> makes;
        for(auto &e : d.GetArray())
        {
            makes.push_back(e.GetInt());
        }

        UserManager::Get()->UserSetAutoPartCategories(userId, makes);

        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::GetUserNumGolds()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        int numGolds = UserManager::Get()->GetUserNumGolds(userId);

        res.status = 200;
        res.set_content(std::to_string(numGolds), "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::EditUser()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        rapidjson::Document d;
        d.Parse(req.body.c_str());
        bool r = UserManager::Get()->EditUser(userId, d["first_name"].GetString(), d["second_name"].GetString(), d["phone"].GetString());

        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::GetUser()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
         res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        DBUser* pUser = nullptr;

        if (req.has_param("user_id"))
        {
            std::string userId = (req.get_param_value("user_id", 0).c_str());
            pUser = UserManager::Get()->GetUser(atoi(userId.c_str()));
        }
        else if (req.has_param("username"))
        {
            std::string username = (req.get_param_value("username", 0).c_str());
            pUser = UserManager::Get()->GetUser(username);
        }
        if (!pUser)
        {
            res.status = 404;
            res.set_content("{}", "application/json");
            return;
        }


        res.status = 200;
        std::string json = pUser->ToJson();
        res.set_content(json, "application/json");
        delete pUser;

    };
}

#include "../managers/CarManager.h"

std::function<void(const httplib::Request &, httplib::Response &)> UsersRoute::GetUserCars()
{
  return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");
        
        std::string userId = (req.get_param_value("user_id", 0).c_str());
        std::vector<DBCar*> cars;
        CarManager::Get()->GetCars(atoi(userId.c_str()), cars);
        
        std::string json;
        CarManager::Get()->ToJson(cars.size(), cars, json);
        res.set_content(json, "application/json");
        for (auto pCar : cars)
          delete pCar;
     

        res.status = 200;
        res.set_content(json, "application/json");
    };
}

