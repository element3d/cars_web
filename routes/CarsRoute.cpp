#include "CarsRoute.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
//#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/CarManager.h"
#include <fstream>
#include <iostream>

CarsRoute* CarsRoute::sInstance = nullptr;

CarsRoute* CarsRoute::Get()
{
	if (!sInstance) sInstance = new CarsRoute();
	return sInstance;
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::MyCars()
{
	return [this](const httplib::Request& req, httplib::Response& res) {
    res.set_header("Access-Control-Allow-Methods", " POST, GET, OPTIONS");
		res.set_header("Content-Type", "text/html; charset=utf-8");
	  res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
		res.set_header("Access-Control-Allow-Origin", "*");
		res.set_header("Connection", "close");

		std::string token = req.get_header_value("Authentication");

		auto decoded = jwt::decode(token);

		int userId = decoded.get_payload_claim("id").as_int();

		std::vector<DBCar*> cars;
		CarManager::Get()->GetCars(userId, cars);
		//for(auto& e : decoded.get_payload_claims())
		//  std::cout << e.first << " = " << e.second << std::endl;

		res.status = 200;
		std::string json;
        ToJson(cars.size(), cars, json);
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
        ToJson(cars.size(), cars, json);
        res.set_header("Access-Control-Allow-Origin", "*");
		res.set_content(json, "application/json");
		for (auto pCar : cars)
			delete pCar;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::NumCars()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        std::vector<int> count;
        std::string make = "";
        std::string serie = "";
        if (req.has_param("make"))
        {
            make = req.get_param_value("make", 0).c_str();
        }
        if (make.size())
        {
            CarManager::Get()->GetNumCars(make, count);
        }
        else
        {
            if (req.has_param("serie"))
            {
                serie = req.get_param_value("serie", 0).c_str();
            }
            if (serie.size())
            {
                CarManager::Get()->GetNumCarsBySerie(serie, count);
            }
            else
            {
                CarManager::Get()->GetNumCars(count);
            }
        }

        if (!count.size())
        {
            res.status = 506;
            res.set_content("FAILED", "text/plain");
            return;
        }
        std::string json;
        rapidjson::Document d;
        d.SetArray();
        for (auto i : count)
        {
            rapidjson::Value v;
            v.SetInt(i);
            d.PushBack(v, d.GetAllocator());
        }
        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        json = buffer.GetString();

        res.status = 200;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(json, "application/json");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarGet()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
//r  .set_header("Access-Control-Allow-Origin", "*");
        int carId = atoi(req.get_param_value("car_id", 0).c_str());
        std::vector<DBCar*> cars;
        DBCar* pCar = CarManager::Get()->GetCar(carId);
        int totalNumCars = 0;
        if (pCar)
        {
            totalNumCars = 1;
            cars.push_back(pCar);
        }
        res.status = 200;
        std::string json;
        ToJson(totalNumCars, cars, json);
        res.set_content(json, "application/json");
        for (auto pCar : cars)
            delete pCar;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsGet()
{
	return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        CarFilter filter;
        _ParseFilter(req, filter);

        std::vector<DBCar*> cars;
        CarManager::Get()->GetCars(filter, filter.Page, cars);
        int totalNumCars =  CarManager::Get()->GetTotalNumCars(filter);
		res.status = 200;
		std::string json;
        ToJson(totalNumCars, cars, json);
		res.set_content(json, "application/json");
		for (auto pCar : cars)
			delete pCar;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsPost()
{
	return [](const httplib::Request& req, httplib::Response& res) {

     res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int carId = CarManager::Get()->CreateCar(userId, req.body);
		if (carId == -1)
		{
            res.status = 501;
			return;
		}
		res.set_content(std::to_string(carId).c_str(), "text/plain");
		res.status = 200;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsPut()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");
		    /*res.set_header("Content-Type", "text/html; charset=utf-8");
	      res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
		    res.set_header("Connection", "close");*/

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int carId = -1;
        if (req.has_param("car_id")) carId = atoi(req.get_param_value("car_id", 0).c_str());
        if (carId == -1)
        {
            res.status = 500;
            return;
        }
        bool success = CarManager::Get()->EditCar(userId, carId, req.body);
        if (!success)
        {
            res.status = 500;
            return;
        }
        res.set_content(/*std::to_string(carId).c_str()*/"OK", "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsDelete()
{
    return [](const httplib::Request& req, httplib::Response& res) {

         res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");


        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int carId = -1;
        if (req.has_param("car_id")) carId = atoi(req.get_param_value("car_id", 0).c_str());
        if (carId == -1)
        {
            res.status = 500;
            return;
        }
        bool success = CarManager::Get()->DeleteCar(userId, carId);
        if (!success)
        {
            res.status = 500;
            return;
        }
        res.set_content(/*std::to_string(carId).c_str()*/"OK", "text/plain");
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

#define STB_IMAGE_IMPLEMENTATION 1
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "../stb_image/stb_image.h"
#include "../stb_image/stb_image_write.h"

#include "../anvir/avir.h"
#include "../libwebp/src/webp/decode.h"

#include "ImageUtils.h"

#include <webp/encode.h> 


void Upload(void * data, int size, std::string fullPath, std::string filename, EImageContentType contentType) 
{
    int w, h, c;
    unsigned char* d = nullptr;

    if (contentType == EImageContentType::Webp)
    {
      c = 3;
      printf("decoding webp ... \n");
      d = WebPDecodeRGB((const uint8_t*)data, size, &w, &h);
    }
    else
      d = stbi_load_from_memory((unsigned char*)data, size, &w, &h, &c, 0);
      printf("decoding webp finished... \n");

    int nw = 400;
    int nh = int(400.f * h / w);
    unsigned char* dd = new unsigned char[nw* nh * c];
    avir :: CImageResizer<> ImageResizer( 8 );
    ImageResizer.resizeImage( d, w, h, 0, dd, nw, nh, c, 0 );

    WebPSave(dd, nw, nh, stlplus::folder_part(fullPath) + "/" + filename + ".webp");

    int aw = std::min(w, 2000);
    int ah = int(aw * h / w);
    unsigned char* ad = new unsigned char[aw* ah * c];
    ImageResizer.resizeImage( d, w, h, 0, ad, aw, ah, c, 0 );
    WebPSave(ad, aw, ah, stlplus::folder_part(fullPath) + "/" + filename + "_orig.webp");

    // stbi_write_jpg(std::string(stlplus::folder_part(fullPath) + "/" + filename + "_orig.jpg").c_str(), aw, ah, c, ad, 100);
    // stbi_write_jpg(fullPath.c_str(), nw, nh, c, dd, 100);
    stbi_image_free(d);
    delete[] dd;
    delete[] ad;
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsSetAvatar()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
        res.set_header("Content-Type", "text/html; charset=utf-8");
        res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
        res.set_header("Access-Control-Allow-Origin", "*");
        
        if (!req.has_param("car_id"))
        {
            res.status = 206;
            return;
        }
        std::string carId = req.get_param_value("car_id", 0).c_str();
        std::string imageId = req.get_param_value("image_id", 0).c_str();
        CarManager::Get()->SetCarAvatar(atoi(carId.c_str()), atoi(imageId.c_str()));
        res.status = 200;
    };
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
		
    std::string dataDir = "data";
  //   std::string dataDir = "/var/www/data";
		if (!stlplus::folder_exists(dataDir)) stlplus::folder_create(dataDir);
		std::string carsDir = dataDir + "/cars";
		if (!stlplus::folder_exists(carsDir)) stlplus::folder_create(carsDir);
		std::string userDir = carsDir + "/" + userId;
		if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
		std::string carDir = userDir + "/" + carId;
		if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
		std::string filename = carDir + "/avatar.webp";
//		std::ofstream ofs(filename, std::ios::binary);
//		ofs << image_file.content;

//        int w, h, c;
//        float* data = stbi_loadf_from_memory((unsigned char*)image_file.content.c_str(), image_file.content.size(), &w, &h, &c, 0);
//        stbi_write_jpg(filename.c_str(), w, h, c, data, 100);

   
        Upload((unsigned char*)image_file.content.c_str(), image_file.content.size(), filename, "avatar", EImageContentType::Jpeg);
		CarManager::Get()->SetCarAvatar(atoi(carId.c_str()), filename);
		res.status = 200;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsDeleteImage()
{
    return [](const httplib::Request& req, httplib::Response& res) {
       res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, OPTIONS");
		res.set_header("Content-Type", "text/html; charset=utf-8");
	  res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
		res.set_header("Access-Control-Allow-Origin", "*");
		res.set_header("Connection", "close");
        if (!req.has_param("car_id") || !req.has_param("image_id"))
        {
            res.status = 206;
            return;
        }
        std::string imageId = (req.get_param_value("image_id", 0).c_str());
        std::string carId = req.get_param_value("car_id", 0).c_str();
        CarManager::Get()->DeleteCarImage(atoi(carId.c_str()), atoi(imageId.c_str()));
        res.set_content("OK", "text/plain");
        res.status = 200;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsUploadImage()
{
	return [](const httplib::Request& req, httplib::Response& res) {

     res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
     res.set_header("Access-Control-Allow-Origin", "*");

        if (/*!req.has_param("user_id") ||*/ !req.has_param("car_id"))
		{
			res.status = 206;
			return;
		}

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

		std::string carId = req.get_param_value("car_id", 0).c_str();
		httplib::MultipartFormData image_file = req.get_file_value("image_file");
    httplib::MultipartFormData isAvatar = req.get_file_value("is_avatar");

    std::string dataDir = "data";
//     std::string dataDir = "/var/www/data";
		if (!stlplus::folder_exists(dataDir)) stlplus::folder_create(dataDir);
		std::string carsDir = dataDir + "/cars";
		if (!stlplus::folder_exists(carsDir)) stlplus::folder_create(carsDir);
        std::string userDir = carsDir + "/" + std::to_string(userId);
		if (!stlplus::folder_exists(userDir)) stlplus::folder_create(userDir);
		std::string carDir = userDir + "/"  + carId;
		if (!stlplus::folder_exists(carDir)) stlplus::folder_create(carDir);
        std::string rn = random_string(10);
        std::string filename = carDir + "/" + rn + ".webp";
//		std::ofstream ofs(filename, std::ios::binary);
//		ofs << image_file.content;

        EImageContentType ct = EImageContentType::Jpeg;
        if (image_file.content_type == std::string("image/webp")) ct = EImageContentType::Webp;

        Upload((unsigned char*)image_file.content.c_str(), image_file.content.size(), filename, rn, ct);
        bool isAva = atoi(isAvatar.content.c_str());

        int id = CarManager::Get()->AddCarImage(atoi(carId.c_str()), filename);
        if (isAva)
            CarManager::Get()->SetCarAvatar(atoi(carId.c_str()), id);

       //  res.set_header("Access-Control-Allow-Origin", "*");
        rapidjson::Document d;
        d.SetObject();
        rapidjson::Value v;
        v.SetInt(id);
        d.AddMember("id", v, d.GetAllocator());
	std::string url = "data/cars/";
	url += rn + ".webp";
        v.SetString(url.c_str(), url.size(), d.GetAllocator());
        d.AddMember("uri", v, d.GetAllocator());

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        std::string jsonStr = buffer.GetString();

        res.set_content(jsonStr, "application/json");
		res.status = 200;
	};
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsUpdateStars()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
         res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
     res.set_header("Access-Control-Allow-Origin", "*");
        std::string token = req.get_header_value("Authentication");

        auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();
        std::string carId = req.get_param_value("car_id", 0).c_str();
        std::string numStars = req.get_param_value("num_stars", 0).c_str();

        CarManager::Get()->UpdateCarStars(userId, atoi(carId.c_str()), atoi(numStars.c_str()));
        //for(auto& e : decoded.get_payload_claims())
        //  std::cout << e.first << " = " << e.second << std::endl;

        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsGetStars()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
      res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
     res.set_header("Access-Control-Allow-Origin", "*");

        std::string carId = req.get_param_value("car_id", 0).c_str();

        int numStars = CarManager::Get()->GetCarStars(atoi(carId.c_str()));
        //for(auto& e : decoded.get_payload_claims())
        //  std::cout << e.first << " = " << e.second << std::endl;

        res.status = 200;
        res.set_content(std::to_string(numStars), "text/plain");
    };
}


std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsGetUserVoteStars()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
      res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
     res.set_header("Access-Control-Allow-Origin", "*");

        std::string carId = req.get_param_value("car_id", 0).c_str();
        std::string token = req.get_header_value("Authentication");
          auto decoded = jwt::decode(token);

        int userId = decoded.get_payload_claim("id").as_int();

        int numStars = CarManager::Get()->GetCarUserVoteStars(atoi(carId.c_str()), userId);
        //for(auto& e : decoded.get_payload_claims())
        //  std::cout << e.first << " = " << e.second << std::endl;

        res.status = 200;
        res.set_content(std::to_string(numStars), "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> CarsRoute::CarsRefresh()
{
    return [this](const httplib::Request& req, httplib::Response& res) {

        std::string carId = req.get_param_value("car_id", 0).c_str();

        CarManager::Get()->Refresh(atoi(carId.c_str()));

        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

void CarsRoute::_ParseFilter(const httplib::Request& req, CarFilter& filter)
{
    int province = -1;
    if (req.has_param("province")) province = atoi(req.get_param_value("province", 0).c_str());

    std::string submodel = "all";
    if (req.has_param("submodel")) submodel = req.get_param_value("submodel", 0).c_str();
    std::string model = "all";
    if (req.has_param("model")) model = req.get_param_value("model", 0).c_str();
    std::string serie = "all";
    if (req.has_param("serie")) serie = req.get_param_value("serie", 0).c_str();
    std::string make = "all";
    if (req.has_param("make")) make = req.get_param_value("make", 0).c_str();

    int priceFrom = -1;
    if (req.has_param("price_from")) priceFrom = atoi(req.get_param_value("price_from", 0).c_str());
    int priceTo = -1;
    if (req.has_param("price_to")) priceTo = atoi(req.get_param_value("price_to", 0).c_str());

    int yearFrom = -1;
    if (req.has_param("year_from")) yearFrom = atoi(req.get_param_value("year_from", 0).c_str());
    int yearTo = -1;
    if (req.has_param("year_to")) yearTo = atoi(req.get_param_value("year_to", 0).c_str());

    int bodyType = -1;
    if (req.has_param("body_type")) bodyType = atoi(req.get_param_value("body_type", 0).c_str());
    int driveType = -1;
    if (req.has_param("drive_type")) driveType = atoi(req.get_param_value("drive_type", 0).c_str());
    int engineType = -1;
    if (req.has_param("engine_type")) engineType = atoi(req.get_param_value("engine_type", 0).c_str());
    int transmission = -1;
    if (req.has_param("transmission")) transmission = atoi(req.get_param_value("transmission", 0).c_str());
    int steeringWheel = -1;
    if (req.has_param("stearing_wheel")) steeringWheel = atoi(req.get_param_value("stearing_wheel", 0).c_str());
    int color = -1;
    if (req.has_param("color")) color = atoi(req.get_param_value("color", 0).c_str());
    int customsCleared = 1;
    if (req.has_param("customs_cleared")) customsCleared = atoi(req.get_param_value("customs_cleared", 0).c_str());
    int exchange = -1;
    if (req.has_param("exchange")) exchange = atoi(req.get_param_value("exchange", 0).c_str());

    int engineSizeFrom = -1;
    if (req.has_param("engine_size_from")) engineSizeFrom = atoi(req.get_param_value("engine_size_from", 0).c_str());
    int engineSizeTo = -1;
    if (req.has_param("engine_size_to")) engineSizeTo = atoi(req.get_param_value("engine_size_to", 0).c_str());

    int page = 1;
    if (req.has_param("page")) page = atoi(req.get_param_value("page", 0).c_str());

    int view = 0;
    if (req.has_param("view")) view = atoi(req.get_param_value("view", 0).c_str());

    int onSale = 1;
    if (req.has_param("on_sale")) onSale = atoi(req.get_param_value("on_sale", 0).c_str());

    int limit = 4;
    if (req.has_param("limit")) limit = atoi(req.get_param_value("limit", 0).c_str());

    filter.Province = province;
    filter.Page = page;
    filter.Make = make;
    filter.Class = serie;
    filter.Model = model;
    filter.SubModel = submodel;
    filter.PriceFrom = priceFrom;
    filter.PriceTo = priceTo;
    filter.YearFrom = yearFrom;
    filter.YearTo = yearTo;
    filter.BodyType = bodyType;
    filter.DriveType = driveType;
    filter.EngineType = engineType;
    filter.EngineSizeFrom = engineSizeFrom;
    filter.EngineSizeTo = engineSizeTo;
    filter.Transmission = transmission;
    filter.SteeringWheel = steeringWheel;
    filter.CustomsCleared = customsCleared;
    filter.Exchange = exchange;
    filter.Color = color;
    filter.View = view;
    filter.OnSale = onSale;
    filter.Limit = limit;
}

void CarsRoute::ToJson(int totalNumCars, const std::vector<DBCar*> cars, std::string& json)
{
	rapidjson::Document d;
    d.SetObject();
    rapidjson::Value tnc;
    tnc.SetInt(totalNumCars);
    d.AddMember("totalNumCars", tnc, d.GetAllocator());

    rapidjson::Value vcars;
    vcars.SetArray();
	for (auto pCar : cars)
	{
		rapidjson::Value o;
		o.SetObject();
		rapidjson::Value v;

		v.SetInt(pCar->Id);
		o.AddMember("id", v, d.GetAllocator());

		v.SetInt(pCar->UserId);
		o.AddMember("user_id", v, d.GetAllocator());

        v.SetString(pCar->Make.c_str(), d.GetAllocator());
		o.AddMember("make", v, d.GetAllocator());

        v.SetString(pCar->Class.c_str(), d.GetAllocator());
		o.AddMember("class", v, d.GetAllocator());

        v.SetString(pCar->Model.c_str(), d.GetAllocator());
		o.AddMember("model", v, d.GetAllocator());

		v.SetString(pCar->Submodel.c_str(), d.GetAllocator());
		o.AddMember("submodel", v, d.GetAllocator());

		v.SetInt(pCar->Country);
		o.AddMember("country", v, d.GetAllocator());

		v.SetInt(pCar->Province);
		o.AddMember("province", v, d.GetAllocator());

		v.SetInt(pCar->SubProvince);
		o.AddMember("sub_province", v, d.GetAllocator());

//		v.SetInt(pCar->Class);
//		o.AddMember("class", v, d.GetAllocator());

//		v.SetInt(pCar->Model);
//		o.AddMember("model", v, d.GetAllocator());

		v.SetInt(pCar->Price);
		o.AddMember("price", v, d.GetAllocator());

		v.SetInt(pCar->BodyType);
		o.AddMember("body_type", v, d.GetAllocator());

		v.SetInt(pCar->Year);
		o.AddMember("year", v, d.GetAllocator());

		v.SetInt(pCar->EngineType);
		o.AddMember("engine_type", v, d.GetAllocator());

        v.SetFloat(pCar->EngineSize);
		o.AddMember("engine_size", v, d.GetAllocator());

		v.SetInt(pCar->EnginePower);
		o.AddMember("engine_power", v, d.GetAllocator());

		v.SetInt(pCar->DriveType);
		o.AddMember("drive_type", v, d.GetAllocator());

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

        v.SetInt(pCar->Mileage);
        o.AddMember("mileage", v, d.GetAllocator());

        v.SetString(pCar->Description.c_str(), d.GetAllocator());
        o.AddMember("description", v, d.GetAllocator());

//		v.SetString(pCar->Avatar.c_str(), d.GetAllocator());
//		o.AddMember("avatar", v, d.GetAllocator());

        v.SetInt(pCar->AvatarImageId);
        o.AddMember("avatar_image_id", v, d.GetAllocator());

		v.SetInt(pCar->OnSale);
		o.AddMember("on_sale", v, d.GetAllocator());

		v.SetInt(pCar->OnTop);
		o.AddMember("on_top", v, d.GetAllocator());

        v.SetDouble(pCar->RefreshTs);
        o.AddMember("refresh_ts", v, d.GetAllocator());

    v.SetInt(pCar->Rank);
    o.AddMember("rank", v, d.GetAllocator());

		v.SetArray();
		for (auto& i : pCar->Images)
		{
            rapidjson::Value vcar;
            vcar.SetObject();
            rapidjson::Value va;
            va.SetString(i.ImagePath.c_str(), d.GetAllocator());
            vcar.AddMember("uri", va, d.GetAllocator());
            va.SetInt(i.Id);
            vcar.AddMember("id", va, d.GetAllocator());

            v.PushBack(vcar, d.GetAllocator());
		}
		o.AddMember("images", v,d.GetAllocator());

        vcars.PushBack(o, d.GetAllocator());
	}
    d.AddMember("cars", vcars, d.GetAllocator());
	
	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	json = buffer.GetString();
}
