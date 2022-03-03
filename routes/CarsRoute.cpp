#include "CarsRoute.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/CarManager.h"
#include <fstream>

CarsRoute* CarsRoute::sInstance = nullptr;

CarsRoute* CarsRoute::Get()
{
	if (!sInstance) sInstance = new CarsRoute();
	return sInstance;
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::MyCars()
{
	return [this](const httplib::Request& req, httplib::Response& res) {
		std::string token = req.get_header_value("Authentication");

		auto decoded = jwt::decode(token);

		std::string phone = decoded.get_payload_claim("phone").as_string();
		int userId = decoded.get_payload_claim("id").as_int();

		std::vector<DBCar*> cars;
		CarManager::Get()->GetCars(userId, cars);
		//for(auto& e : decoded.get_payload_claims())
		//  std::cout << e.first << " = " << e.second << std::endl;

		res.status = 200;
		std::string json;
		ToJson(cars, json);
		res.set_content(json, "application/json");
		for (auto pCar : cars)
			delete pCar;

	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::TopCars()
{
	return [this](const httplib::Request& req, httplib::Response& res) {
		int serie = -1;
		if (req.has_param("serie")) serie = atoi(req.get_param_value("serie", 0).c_str());
		int make = -1;
		if (req.has_param("make"))
		{
			make = atoi(req.get_param_value("make", 0).c_str());
		}
		std::vector<DBCar*> cars;
		if (serie != -1)
			CarManager::Get()->GetTopCarsBySerie(serie, cars);
		else
			CarManager::Get()->GetTopCarsByMake(make, cars);

		res.status = 200;
		std::string json;
		ToJson(cars, json);
		res.set_content(json, "application/json");
		for (auto pCar : cars)
			delete pCar;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsGet()
{
	return [this](const httplib::Request& req, httplib::Response& res) {
		int model = -1;
		if (req.has_param("model")) model = atoi(req.get_param_value("model", 0).c_str());
		int serie = -1;
		if (req.has_param("serie")) serie = atoi(req.get_param_value("serie", 0).c_str());
		int make = -1;
		if (req.has_param("make")) make = atoi(req.get_param_value("make", 0).c_str());
		
		std::vector<DBCar*> cars;
		CarFilter filter;
		filter.Make = make;
		filter.Class = serie;
		filter.Model = model;
		CarManager::Get()->GetCars(filter, cars);

		res.status = 200;
		std::string json;
		ToJson(cars, json);
		res.set_content(json, "application/json");
		for (auto pCar : cars)
			delete pCar;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsPost()
{
	return [](const httplib::Request& req, httplib::Response& res) {
		int carId = CarManager::Get()->CreateCar(req.body);
		if (carId == -1)
		{
			res.status = 500;
			return;
		}
		res.set_content(std::to_string(carId).c_str(), "text/plain");
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

std::string  gen_random(int len) {
	std::string s;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return s;
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsUploadAvatar()
{
	return [](const httplib::Request& req, httplib::Response& res) {
		if (!req.has_param("user_id") || !req.has_param("car_id"))
		{
			res.status = 206;
			return;
		}
		std::string userId = (req.get_param_value("user_id", 0).c_str());
		std::string carId = req.get_param_value("car_id", 0).c_str();
		httplib::MultipartFormData image_file = req.get_file_value("image_file");
		
		if (!stlplus::folder_exists("data")) stlplus::folder_create("data");
		std::string carsDir = "data/cars";
		if (!stlplus::folder_exists(carsDir)) stlplus::folder_create(carsDir);
		std::string userDir = carsDir + "/" + userId;
		if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
		std::string carDir = userDir + "/" + carId;
		if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
		std::string filename = carDir + "/avatar.jpg";
		std::ofstream ofs(filename, std::ios::binary);
		ofs << image_file.content;
		CarManager::Get()->SetCarAvatar(atoi(carId.c_str()), filename);
		res.status = 200;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsUploadImage()
{
	return [](const httplib::Request& req, httplib::Response& res) {
		if (!req.has_param("user_id") || !req.has_param("car_id"))
		{
			res.status = 206;
			return;
		}
		std::string userId = (req.get_param_value("user_id", 0).c_str());
		std::string carId = req.get_param_value("car_id", 0).c_str();
		httplib::MultipartFormData image_file = req.get_file_value("image_file");

		if (!stlplus::folder_exists("data")) stlplus::folder_create("data");
		std::string carsDir = "data/cars";
		if (!stlplus::folder_exists(carsDir)) stlplus::folder_create(carsDir);
		std::string userDir = carsDir + "/" + userId;
		if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
		std::string carDir = userDir + "/"  + carId;
		if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
		std::string filename = carDir + "/" + random_string(10) + ".jpg";
		std::ofstream ofs(filename, std::ios::binary);
		ofs << image_file.content;

		CarManager::Get()->AddCarImage(atoi(carId.c_str()), filename);

		res.status = 200;
	};
}

void CarsRoute::ToJson(const std::vector<DBCar*> cars, std::string& json)
{
	rapidjson::Document d;
	d.SetArray();
	for (auto pCar : cars)
	{
		rapidjson::Value o;
		o.SetObject();
		rapidjson::Value v;

		v.SetInt(pCar->Id);
		o.AddMember("id", v, d.GetAllocator());

		v.SetInt(pCar->UserId);
		o.AddMember("user_id", v, d.GetAllocator());

		v.SetInt(pCar->Make);
		o.AddMember("make", v, d.GetAllocator());

		v.SetInt(pCar->Class);
		o.AddMember("class", v, d.GetAllocator());

		v.SetInt(pCar->Model);
		o.AddMember("model", v, d.GetAllocator());

		v.SetInt(pCar->Submodel);
		o.AddMember("sub_model", v, d.GetAllocator());

		v.SetInt(pCar->Country);
		o.AddMember("country", v, d.GetAllocator());

		v.SetInt(pCar->Province);
		o.AddMember("province", v, d.GetAllocator());

		v.SetInt(pCar->SubProvince);
		o.AddMember("sub_province", v, d.GetAllocator());

		v.SetInt(pCar->Class);
		o.AddMember("class", v, d.GetAllocator());

		v.SetInt(pCar->Model);
		o.AddMember("model", v, d.GetAllocator());

		v.SetInt(pCar->Price);
		o.AddMember("price", v, d.GetAllocator());

		v.SetInt(pCar->BodyType);
		o.AddMember("body_type", v, d.GetAllocator());

		v.SetInt(pCar->Year);
		o.AddMember("year", v, d.GetAllocator());

		v.SetInt(pCar->EngineType);
		o.AddMember("engine_type", v, d.GetAllocator());

		v.SetInt(pCar->EngineSize);
		o.AddMember("engine_size", v, d.GetAllocator());

		v.SetInt(pCar->EnginePower);
		o.AddMember("engine_power", v, d.GetAllocator());

		v.SetInt(pCar->Acc0To100);
		o.AddMember("acc_0_100", v, d.GetAllocator());

		v.SetInt(pCar->Transmission);
		o.AddMember("transmission", v, d.GetAllocator());

		v.SetInt(pCar->StearingWheel);
		o.AddMember("stearing_wheel", v, d.GetAllocator());

		v.SetInt(pCar->Exchange);
		o.AddMember("exchange", v, d.GetAllocator());

		v.SetInt(pCar->CustomsCleared);
		o.AddMember("customs_cleared", v, d.GetAllocator());

		v.SetInt(pCar->Color);
		o.AddMember("color", v, d.GetAllocator());

		v.SetString(pCar->Avatar.c_str(), d.GetAllocator());
		o.AddMember("avatar", v, d.GetAllocator());

		v.SetInt(pCar->OnSale);
		o.AddMember("on_sale", v, d.GetAllocator());

		v.SetInt(pCar->OnTop);
		o.AddMember("on_top", v, d.GetAllocator());

		v.SetArray();
		for (auto& i : pCar->Images)
		{
			rapidjson::Value va;
			va.SetString(i.c_str(), d.GetAllocator());
			v.PushBack(va, d.GetAllocator());
		}
		o.AddMember("images", v,d.GetAllocator());

		d.PushBack(o, d.GetAllocator());
	}
	
	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	json = buffer.GetString();
}