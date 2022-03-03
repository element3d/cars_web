#include "AutoPartsRoute.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/AutoPartManager.h"
#include <fstream>

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

		std::string phone = decoded.get_payload_claim("phone").as_string();
		int userId = decoded.get_payload_claim("id").as_int();

		std::vector<DBAutoPart*> autoParts;
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
		ToJson(autoParts, json);
		res.set_content(json, "application/json");
		for (auto pAutoPart : autoParts)
			delete pAutoPart;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> AutoPartsRoute::AutoPartsPost()
{
	return [](const httplib::Request& req, httplib::Response& res) {
		int autoPartId = AutoPartManager::Get()->CreateAutoPart(req.body);
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

void AutoPartsRoute::ToJson(const std::vector<DBAutoPart*> autoParts, std::string& json)
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
		o.AddMember("user_id", v, d.GetAllocator());

		v.SetInt(pAutoPart->Type);
		o.AddMember("type", v, d.GetAllocator());

		v.SetInt(pAutoPart->Subtype);
		o.AddMember("sub_type", v, d.GetAllocator());

		v.SetInt(pAutoPart->Country);
		o.AddMember("country", v, d.GetAllocator());

		v.SetInt(pAutoPart->Province);
		o.AddMember("province", v, d.GetAllocator());

		v.SetInt(pAutoPart->SubProvince);
		o.AddMember("sub_province", v, d.GetAllocator());

		v.SetInt(pAutoPart->Originality);
		o.AddMember("originality", v, d.GetAllocator());

		v.SetInt(pAutoPart->Condition);
		o.AddMember("condition", v, d.GetAllocator());

		v.SetInt(pAutoPart->Price);
		o.AddMember("price", v, d.GetAllocator());

		v.SetString(pAutoPart->Avatar.c_str(), d.GetAllocator());
		o.AddMember("avatar", v, d.GetAllocator());

		/*v.SetInt(pCar->OnTop);
		o.AddMember("on_top", v, d.GetAllocator());*/

		v.SetArray();
		for (auto& i : pAutoPart->Images)
		{
			rapidjson::Value va;
			va.SetString(i.c_str(), d.GetAllocator());
			v.PushBack(va, d.GetAllocator());
		}
		o.AddMember("images", v, d.GetAllocator());

		d.PushBack(o, d.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	json = buffer.GetString();
}