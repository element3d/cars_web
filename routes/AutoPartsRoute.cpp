#include "AutoPartsRoute.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
//#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/AutoPartManager.h"
#include <fstream>
#include "../stlplus/file_system.hpp"

AutoPartsRoute* AutoPartsRoute::sInstance = nullptr;

AutoPartsRoute* AutoPartsRoute::Get()
{
	if (!sInstance) sInstance = new AutoPartsRoute();
	return sInstance;
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::MyAutoParts()
{
	return [this](const httplib::Request& req, httplib::Response& res) {
		std::string token = req.get_header_value("Authentication");

		auto decoded = jwt::decode(token);

		int userId = decoded.get_payload_claim("id").as_int();

        std::vector<DBAutoPartRequest*> autoParts;
		AutoPartManager::Get()->GetAutoParts(userId, autoParts);
		//for(auto& e : decoded.get_payload_claims())
		//  std::cout << e.first << " = " << e.second << std::endl;

		res.status = 200;
		std::string json;
		ToJson(autoParts, json);
		res.set_content(json, "application/json");
		for (auto pAutoPart : autoParts)
			delete pAutoPart;

	};
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::TopAutoParts()
{
	return [this](const httplib::Request& req, httplib::Response& res) {
		/*int serie = -1;
		if (req.has_param("serie")) serie = atoi(req.get_param_value("serie", 0).c_str());*/
		int type = -1;
		if (req.has_param("type"))
		{
			type = atoi(req.get_param_value("type", 0).c_str());
		}
		std::vector<DBAutoPart*> autoParts;
		/*if (serie != -1)
			CarManager::Get()->GetTopCarsBySerie(serie, cars);
		else*/
			AutoPartManager::Get()->GetTopAutoPartsByType(type, autoParts);

		res.status = 200;
		std::string json;
//		ToJson(autoParts, json);
//		res.set_content(json, "application/json");
//		for (auto pAutoPart : autoParts)
//			delete pAutoPart;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartPost()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        int autoPartId = AutoPartManager::Get()->CreateAutoPart(userId, req.body);
        if (autoPartId == -1)
        {
            res.status = 500;
            return;
        }
        res.set_content(std::to_string(autoPartId).c_str(), "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartGet()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        int autoPartId = atoi(req.get_param_value("id", 0).c_str());
        rapidjson::Document d;
        AutoPartManager::Get()->GetAutoPart(userId, autoPartId, d);
        //for(auto& e : decoded.get_payload_claims())
        //  std::cout << e.first << " = " << e.second << std::endl;

        res.status = 200;
        std::string json;
        rapidjson::StringBuffer buffer;
        buffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        json = buffer.GetString();
        res.set_content(json, "application/json");

    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartDelete()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        int autoPartId = atoi(req.get_param_value("id", 0).c_str());
        rapidjson::Document d;
        AutoPartManager::Get()->DeleteAutoPart(userId, autoPartId);

        res.status = 200;
        res.set_content("OK", "application/json");
    };
}

#include "../anvir/avir.h"
#include "../stb_image/stb_image_write.h"
#include "../stb_image/stb_image.h"

static void Upload(void * data, int size, std::string fullPath) {
    int w, h, c;
    unsigned char* d = stbi_load_from_memory((unsigned char*)data, size, &w, &h, &c, 0);

    int nw = 400;
    int nh = int(400.f * h / w);
    unsigned char* dd = new unsigned char[nw* nh * c];
    avir :: CImageResizer<> ImageResizer( 8 );
    ImageResizer.resizeImage( d, w, h, 0, dd, nw, nh, c, 0 );

    stbi_write_jpg(fullPath.c_str(), nw, nh, c, dd, 100);
    stbi_image_free(d);
    delete[] dd;
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartUploadAvatar()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        httplib::MultipartFormData image_file = req.get_file_value("image_file");
        if (!image_file.content.size())
        {
             res.status = 200;
             return;
        }

        std::string autoPartId = req.get_param_value("id", 0).c_str();

        if (!stlplus::folder_exists("data")) stlplus::folder_create("data");
        std::string autoPartsDir = "data/auto_parts";
        if (!stlplus::folder_exists(autoPartsDir)) stlplus::folder_create(autoPartsDir);
        std::string userDir = autoPartsDir + "/" + std::to_string(userId);
        if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
        std::string carDir = userDir + "/" + autoPartId;
        if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
        std::string filename = carDir + "/avatar.jpg";
//        std::ofstream ofs(filename, std::ios::binary);
//        ofs << image_file.content;

        Upload((unsigned char*)image_file.content.c_str(), image_file.content.size(), filename);

        AutoPartManager::Get()->SetAutoPartAvatar(atoi(autoPartId.c_str()), filename);
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartResponseUploadAvatar()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        httplib::MultipartFormData image_file = req.get_file_value("image_file");
        if (!image_file.content.size())
        {
             res.status = 200;
             return;
        }

        std::string autoPartId = req.get_param_value("id", 0).c_str();

        if (!stlplus::folder_exists("data")) stlplus::folder_create("data");
        std::string autoPartsDir = "data/auto_part_responses";
        if (!stlplus::folder_exists(autoPartsDir)) stlplus::folder_create(autoPartsDir);
        std::string userDir = autoPartsDir + "/" + std::to_string(userId);
        if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
        std::string carDir = userDir + "/" + autoPartId;
        if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
        std::string filename = carDir + "/avatar.jpg";
//        std::ofstream ofs(filename, std::ios::binary);
//        ofs << image_file.content;

        Upload((unsigned char*)image_file.content.c_str(), image_file.content.size(), filename);

        AutoPartManager::Get()->SetAutoPartResponseAvatar(atoi(autoPartId.c_str()), filename);
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartResponsePost()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        int autoPartId = AutoPartManager::Get()->CreateAutoPartResponse(userId, req.body);
        if (autoPartId == -1)
        {
            res.status = 500;
            return;
        }
        res.set_content(std::to_string(autoPartId).c_str(), "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartResponseGet()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();
        int id = atoi(req.get_param_value("id", 0).c_str());

        rapidjson::Document d;
        AutoPartManager::Get()->GetAutoPartResponse(userId, id, d);

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        std::string json = buffer.GetString();

        res.set_content(json.c_str(), "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartsGetNotifications()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        rapidjson::Document d;
        AutoPartManager::Get()->GetAutoPartNotifications(userId, d);

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        std::string json = buffer.GetString();

        res.set_content(json.c_str(), "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartsGetNotification()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();
        int id = atoi(req.get_param_value("id", 0).c_str());


        rapidjson::Document d;
        AutoPartManager::Get()->GetAutoPartNotification(id, d);

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        std::string json = buffer.GetString();

        res.set_content(json.c_str(), "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartsDeleteNotification()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();
        int id = atoi(req.get_param_value("id", 0).c_str());


        AutoPartManager::Get()->DeleteAutoPartNotification(id, userId);

        res.set_content("OK", "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartsPost()
{
	return [](const httplib::Request& req, httplib::Response& res) {
        int autoPartId = AutoPartManager::Get()->CreateAutoPart(0, req.body);
		if (autoPartId == -1)
		{
			res.status = 500;
			return;
		}
		res.set_content(std::to_string(autoPartId).c_str(), "text/plain");
		res.status = 200;
	};
}

#include "../stlplus/file_system.hpp"

/*std::string random_string(size_t length)
{
auto randchar = []() -> char
{
const char charset[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";
const size_t max_index = (sizeof(charset) - 1);
return charset[rand() % max_index];
};
std::string str(length, 0);
std::generate_n(str.begin(), length, randchar);
return str;
}
*/
static std::string random_string(std::size_t length)
{
	const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

	std::string random_string;

	for (std::size_t i = 0; i < length; ++i)
	{
		random_string += CHARACTERS[distribution(generator)];
	}

	return random_string;
}

/*std::string  gen_random(int len) {
	std::string s;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return s;
}*/

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartsUploadAvatar()
{
	return [](const httplib::Request& req, httplib::Response& res) {
		if (!req.has_param("user_id") || !req.has_param("auto_part_id"))
		{
			res.status = 206;
			return;
		}
		std::string userId = (req.get_param_value("user_id", 0).c_str());
		std::string autoPartId = req.get_param_value("auto_part_id", 0).c_str();
		httplib::MultipartFormData image_file = req.get_file_value("image_file");

		if (!stlplus::folder_exists("data")) stlplus::folder_create("data");
		std::string autoPartsDir = "data/auto_parts";
		if (!stlplus::folder_exists(autoPartsDir)) stlplus::folder_create(autoPartsDir);
		std::string userDir = autoPartsDir + "/" + userId;
		if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
		std::string carDir = userDir + "/" + autoPartId;
		if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
		std::string filename = carDir + "/avatar.jpg";
		std::ofstream ofs(filename, std::ios::binary);
		ofs << image_file.content;
		AutoPartManager::Get()->SetAutoPartAvatar(atoi(autoPartId.c_str()), filename);
		res.status = 200;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartsUploadImage()
{
	return [](const httplib::Request& req, httplib::Response& res) {
		if (!req.has_param("user_id") || !req.has_param("auto_part_id"))
		{
			res.status = 206;
			return;
		}
		std::string userId = (req.get_param_value("user_id", 0).c_str());
		std::string autoPartId = req.get_param_value("auto_part_id", 0).c_str();
		httplib::MultipartFormData image_file = req.get_file_value("image_file");

		if (!stlplus::folder_exists("data")) stlplus::folder_create("data");
		std::string autoPartsDir = "data/auto_parts";
		if (!stlplus::folder_exists(autoPartsDir)) stlplus::folder_create(autoPartsDir);
		std::string userDir = autoPartsDir + "/" + userId;
		if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
		std::string carDir = userDir + "/" + autoPartId;
		if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
		std::string filename = carDir + "/" + random_string(10) + ".jpg";
		std::ofstream ofs(filename, std::ios::binary);
		ofs << image_file.content;

		AutoPartManager::Get()->AddAutoPartImage(atoi(autoPartId.c_str()), filename);

		res.status = 200;
	};
}

void AutoPartsRoute::ToJson(const std::vector<DBAutoPartRequest*> autoParts, std::string& json)
{
	rapidjson::Document d;
	d.SetArray();
	for (auto pAutoPart : autoParts)
	{
		rapidjson::Value o;
		o.SetObject();
		rapidjson::Value v;

		v.SetInt(pAutoPart->Id);
		o.AddMember("id", v, d.GetAllocator());

		v.SetInt(pAutoPart->UserId);
        o.AddMember("userId", v, d.GetAllocator());

        v.SetInt(pAutoPart->Category);
        o.AddMember("category", v, d.GetAllocator());

        v.SetString(pAutoPart->Make.c_str(), d.GetAllocator());
        o.AddMember("make", v, d.GetAllocator());

        v.SetString(pAutoPart->Serie.c_str(), d.GetAllocator());
        o.AddMember("serie", v, d.GetAllocator());

        v.SetString(pAutoPart->Model.c_str(), d.GetAllocator());
        o.AddMember("model", v, d.GetAllocator());

        v.SetString(pAutoPart->Description.c_str(), d.GetAllocator());
        o.AddMember("description", v, d.GetAllocator());

        v.SetString(pAutoPart->Avatar.c_str(), d.GetAllocator());
        o.AddMember("avatar", v, d.GetAllocator());

        v.SetInt(pAutoPart->ResponseCount);
        o.AddMember("numResponses", v, d.GetAllocator());

		/*v.SetInt(pCar->OnTop);
		o.AddMember("on_top", v, d.GetAllocator());*/

//		v.SetArray();
//		for (auto& i : pAutoPart->Images)
//		{
//			rapidjson::Value va;
//			va.SetString(i.c_str(), d.GetAllocator());
//			v.PushBack(va, d.GetAllocator());
//		}
//		o.AddMember("images", v, d.GetAllocator());

		d.PushBack(o, d.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	json = buffer.GetString();
}
