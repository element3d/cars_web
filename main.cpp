//
//  upload.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//

#include <fstream>
#include "httplib.h"
#include <iostream>
//#include <pqxx/pqxx>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <jwt-cpp/jwt.h>
#include "managers/UserManager.h"
#include "routes/UsersRoute.h"
#include "routes/CarsRoute.h"
#include "routes/AutoPartsRoute.h"
#include "routes/EventsRoute.h"
#include "routes/MessagesRoute.h"

using namespace httplib;
using namespace std;
// using namespace pqxx;


const char *html = R"(
<form id="formElem">
  <input type="file" name="image_file" accept="image/*">
  <input type="file" name="text_file" accept="text/*">
  <input type="submit">
</form>
<script>
  formElem.onsubmit = async (e) => {
    e.preventDefault();
    let res = await fetch('/post', {
      method: 'POST',
      body: new FormData(formElem)
    });
    console.log(await res.text());
  };
</script>
)";






#include "routes/AuthRoute.h"
#include <libpq-fe.h>

int main(void) 
{
    Server svr;

    PGconn* pg = PQconnectdb("host=127.0.0.1 port=5432 dbname=cars user=postgres password=postgres");



/*	connection cc("dbname = cars user = postgres password = postgres \
        hostaddr = 127.0.0.1 port = 5432");
    connection* psql;
    try
    {
        psql = new connection("dbname = cars user = postgres password = postgres \
        hostaddr = 127.0.0.1 port = 5432");
        if (psql->is_open())
        {
            cout << "Opened database successfully: " << psql->dbname() << endl;
        }
        else
        {
            cout << "Can't open database" << endl;
            return 1;
        }
        psql->disconnect ();
    }
    catch (const std::exception &e)
    {
        cerr << e.what() << std::endl;
        return 1;
    }*/

    //AutoPartManager::Get()->SetPG(pg);
	if (PQstatus(pg) != CONNECTION_OK)
	{
		fprintf(stderr, "Connection to database failed: %s",
			PQerrorMessage(pg));
	//	exit_nicely(conn);
	}


    svr.Get("/cars", [](const Request & req, Response &res) {

        std::string make = req.get_param_value("make");
        std::string serie = req.get_param_value("serie");
        std::string line;
        std::string html;
        std::string htmlFilename = "./www/find_car.html";
        if (serie.size()) htmlFilename = "./www/find_car_models.html";
        else if (make.size()) htmlFilename = "./www/find_car_series.html";
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {

                html += line;
            }
            myfile.close();
        }

        if (make.size())
        {
            std::regex reg("<CPP_MAKE>");
            html = regex_replace(html, reg, make);
        }
        if (serie.size())
        {
            std::regex reg("<CPP_SERIE>");
            html = regex_replace(html, reg, serie);
        }
        res.set_content(html, "text/html");
    });

    svr.Get("/auto_parts", [](const Request & req, Response &res) {
        std::string line;
        std::string html;
        std::string htmlFilename = "./www/find_auto_part.html";

        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {

                html += line;
            }
            myfile.close();
        }


        res.set_content(html, "text/html");
    });

    svr.Get("/car_list", [](const Request& req, Response &res) {

        std::string line;
        std::string html;
        std::ifstream myfile ("./www/find_car_list_page.html");
        CarFilter filter;
        CarsRoute::Get()->_ParseFilter(req, filter);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }

        std::regex reg("<CPP_FILTERS>");
        std::string query = std::to_string(filter.View);
        query += ", " + std::to_string(filter.Page);
        query += ", " + std::to_string(filter.OnSale);
        query += ", " + std::to_string(filter.Province);
        query += ", '" + filter.Make + "'";
        query += ", '" + filter.Class + "'";
        query += ", '" + filter.Model + "'";
        query += ", " + std::to_string(filter.PriceFrom);
        query += ", " + std::to_string(filter.PriceTo);
        query += ", " + std::to_string(filter.YearFrom);
        query += ", " + std::to_string(filter.YearTo);
        query += ", " + std::to_string(filter.BodyType);
        query += ", " + std::to_string(filter.EngineType);
        query += ", " + std::to_string(filter.EngineSizeFrom);
        query += ", " + std::to_string(filter.EngineSizeTo);
        query += ", " + std::to_string(filter.Transmission);
        query += ", " + std::to_string(filter.Color);
        query += ", " + std::to_string(filter.SteeringWheel);
        query += ", " + std::to_string(filter.Exchange);
        query += ", " + std::to_string(filter.CustomsCleared);
        html = regex_replace(html, reg, query);

        res.set_content(html, "text/html");
    });

    svr.Get("/car_search", [](const Request& req, Response &res) {

        std::string line;
        std::string html;
        std::ifstream myfile ("./www/search_page.html");
        CarFilter filter;
        CarsRoute::Get()->_ParseFilter(req, filter);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }

        std::regex reg("<CPP_FILTERS>");
        std::string query = std::to_string(filter.Page);
        query += ", " + std::to_string(filter.OnSale);
        query += ", " + std::to_string(filter.Province);
        query += ", '" + filter.Make + "'";
        query += ", '" + filter.Class + "'";
        query += ", '" + filter.Model + "'";
        query += ", " + std::to_string(filter.PriceFrom);
        query += ", " + std::to_string(filter.PriceTo);
        query += ", " + std::to_string(filter.YearFrom);
        query += ", " + std::to_string(filter.YearTo);
        query += ", " + std::to_string(filter.BodyType);
        query += ", " + std::to_string(filter.EngineType);
        query += ", " + std::to_string(filter.EngineSizeFrom);
        query += ", " + std::to_string(filter.EngineSizeTo);
        query += ", " + std::to_string(filter.Transmission);
        query += ", " + std::to_string(filter.Color);
        query += ", " + std::to_string(filter.SteeringWheel);
        query += ", " + std::to_string(filter.Exchange);
        query += ", " + std::to_string(filter.CustomsCleared);
        html = regex_replace(html, reg, query);

        res.set_content(html, "text/html");
    });

    svr.Get("/car", [](const Request & req, Response &res) {

        std::string carId = req.get_param_value("id");

        std::string line;
        std::string html;
        std::string htmlFilename = "./www/car_page.html";
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }

        {
            std::regex reg("<CPP_CAR_ID>");
            html = regex_replace(html, reg, carId);
        }

        res.set_content(html, "text/html");
    });

    svr.Get("/add_car", [](const Request & req, Response &res) {

        std::string line;
        std::string html;
        std::string htmlFilename = "./www/add_car_page.html";
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }

        res.set_content(html, "text/html");
    });

    /*svr.Get("/garage", [](const Request & req, Response &res) {
        std::string line;
        std::string html;
        std::string htmlFilename = "./www/garage.html";
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }
        res.set_content(html, "text/html");
    });*/

    svr.Options("/(.*)",
			[&](const Request & /*req*/, Response &res) {
			res.set_header("Access-Control-Allow-Methods", " POST, GET, DELETE, PUT, OPTIONS");
			res.set_header("Content-Type", "text/html; charset=utf-8");
			res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept, Authentication");
			res.set_header("Access-Control-Allow-Origin", "*");
			res.set_header("Connection", "close");
		});

        

    svr.Get("/notifications", [](const Request & req, Response &res) {
        std::string line;
        std::string html;
        std::string htmlFilename = "./www/auto_part_notes.html";
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }
        res.set_content(html, "text/html");
    });

    svr.Get("/notification", [](const Request & req, Response &res) {
        std::string line;
        std::string html;
        std::string htmlFilename = "./www/auto_part_note.html";
        std::string id = req.get_param_value("id");
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }
        {
            std::regex reg("<CPP_ID>");
            html = regex_replace(html, reg, id);
        }
        res.set_content(html, "text/html");
    });

    svr.Get("/auto_part_response", [](const Request & req, Response &res) {
        std::string line;
        std::string html;
        std::string htmlFilename = "./www/auto_part_response_page.html";
        std::string id = req.get_param_value("id");
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }
        {
            std::regex reg("<CPP_ID>");
            html = regex_replace(html, reg, id);
        }
        res.set_content(html, "text/html");
    });


    svr.Get("/auto_part", [](const Request & req, Response &res) {
        std::string line;
        std::string html;
        std::string htmlFilename = "./www/auto_part_page.html";
        std::string id = req.get_param_value("id");
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }
        {
            std::regex reg("<CPP_ID>");
            html = regex_replace(html, reg, id);
        }
        res.set_content(html, "text/html");
    });


    svr.Get("/events", [](const Request & req, Response &res) {

        std::string carId = req.get_param_value("id");

        std::string line;
        std::string html;
        std::string htmlFilename = "./www/events_page.html";
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }

        {
            std::regex reg("<CPP_CAR_ID>");
            html = regex_replace(html, reg, carId);
        }

        res.set_content(html, "text/html");
    });

    svr.Get("/purchase", [](const Request & req, Response &res) {

        std::string carId = req.get_param_value("id");

        std::string line;
        std::string html;
        std::string htmlFilename = "./www/purchase_page.html";
        std::ifstream myfile (htmlFilename);
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }

        {
            std::regex reg("<CPP_CAR_ID>");
            html = regex_replace(html, reg, carId);
        }

        res.set_content(html, "text/html");
    });

    svr.set_mount_point("/", "./www");
    svr.Get("/", [](const Request & req, Response &res) {
        std::string lang = req.get_param_value("lang");
        std::string line;
        std::string html;
        std::ifstream myfile ("./www/index.html");
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                html += line;
            }
            myfile.close();
        }

        //if (lang.size())
        {
            std::regex reg("<CPP_CODE>");
            html = regex_replace(html, reg, lang);
        }
        res.set_content(html, "text/html");
    });


    svr.Post("/api/v1/signup", AuthRoute::Get()->SignUp());
    svr.Post("/api/v1/signup/verify", AuthRoute::Get()->SignUpVerify());
    svr.Post("/api/v1/signin/forgot", AuthRoute::Get()->SignInForgot());
    svr.Post("/api/v1/signin", AuthRoute::Get()->SignIn());
    svr.Post("/api/v1/users/password", AuthRoute::Get()->ChangePassword());

    svr.Get("/api/v1/me", UsersRoute::Get()->Me());
    svr.Get("/api/v1/me/handshake", UsersRoute::Get()->MeHandshake());
    svr.Post("/api/v1/me/avatar", UsersRoute::Get()->MeUploadAvatar());
    svr.Put("/api/v1/me/avatar", UsersRoute::Get()->MeUpdateAvatar());
    svr.Put("/api/v1/me/cover", UsersRoute::Get()->MeUpdateCover());
    svr.Get("/api/v1/user", UsersRoute::Get()->GetUser());
    svr.Get("/api/v1/user/handshake", UsersRoute::Get()->UserHandshake());
    svr.Put("/api/v1/user", UsersRoute::Get()->EditUser());
    svr.Get("/api/v1/users/cars", UsersRoute::Get()->GetUserCars());
    svr.Get("/api/v1/users/auto_part_makes", UsersRoute::Get()->UserGetAutoPartMakes());
    svr.Put("/api/v1/users/auto_part_makes", UsersRoute::Get()->UserSetAutoPartMakes());
    svr.Get("/api/v1/users/auto_part_categories", UsersRoute::Get()->UserGetAutoPartCategories());
    svr.Put("/api/v1/users/auto_part_categories", UsersRoute::Get()->UserSetAutoPartCategories());
    svr.Get("/api/v1/users/num_golds", UsersRoute::Get()->GetUserNumGolds());
    svr.Get("/api/v1/users/earn_gold", UsersRoute::Get()->UserEarnGold());
    svr.Get("/api/v1/users/gifts", UsersRoute::Get()->UserGetGifts());
    svr.Post("/api/v1/users/gifts/receive", UsersRoute::Get()->UserReceiveGift());

	svr.Get("/api/v1/my_cars", CarsRoute::Get()->MyCars());
	svr.Get("/api/v1/top_cars", CarsRoute::Get()->TopCars());
    svr.Get("/api/v1/cars/count", CarsRoute::Get()->NumCars());
    svr.Get("/api/v1/car", CarsRoute::Get()->CarGet());
	svr.Get("/api/v1/cars", CarsRoute::Get()->CarsGet());
	svr.Post("/api/v1/cars", CarsRoute::Get()->CarsPost());
    svr.Put("/api/v1/cars", CarsRoute::Get()->CarsPut());
    svr.Delete("/api/v1/cars", CarsRoute::Get()->CarsDelete());
	svr.Post("/api/v1/cars/avatar", CarsRoute::Get()->CarsUploadAvatar());
    svr.Put("/api/v1/cars/avatar", CarsRoute::Get()->CarsSetAvatar());
	svr.Post("/api/v1/cars/image", CarsRoute::Get()->CarsUploadImage());
    svr.Delete("/api/v1/cars/image", CarsRoute::Get()->CarsDeleteImage());
    svr.Put("/api/v1/cars/stars", CarsRoute::Get()->CarsUpdateStars());
    svr.Get("/api/v1/cars/stars", CarsRoute::Get()->CarsGetStars());
    svr.Get("/api/v1/cars/user_stars", CarsRoute::Get()->CarsGetUserVoteStars());
    svr.Put("/api/v1/cars/view", CarsRoute::Get()->CarsAddView());
    svr.Get("/api/v1/cars/refresh", CarsRoute::Get()->CarsRefresh());
    svr.Post("/api/v1/cars/req_model", CarsRoute::Get()->CarsRequestModel());

    svr.Get("/api/v1/bands", CarsRoute::Get()->BandsGet());
    svr.Get("/api/v1/band", CarsRoute::Get()->BandGet());


    svr.Get("/api/v1/events", EventsRoute::Get()->GetEvents());
    svr.Get("/api/v1/events/recent", EventsRoute::Get()->GetRecentEvent());
    svr.Get("/api/v1/events/notes", EventsRoute::Get()->GetEventsNotes());
    svr.Get("/api/v1/events/inception", EventsRoute::Get()->GetInception());
    svr.Get("/api/v1/events/pit_stop", EventsRoute::Get()->GetPitStop());
    svr.Post("/api/v1/events/user", EventsRoute::Get()->EventsSetUser());
    svr.Post("/api/v1/events/user_card", EventsRoute::Get()->EventsSetUserCard());

    svr.Get("/api/v1/messages", MessagesRoute::Get()->MessagesGet());
    svr.Get("/api/v1/messages/pending", MessagesRoute::Get()->MessagesGetPending());
    svr.Post("/api/v1/messages", MessagesRoute::Get()->MessagesPost());
    svr.Post("/api/v1/messages/image", MessagesRoute::Get()->MessagesUploadImage());

    svr.Get("/api/v1/conversations", MessagesRoute::Get()->ConversationsGet());
    svr.Get("/api/v1/conversations/pending", MessagesRoute::Get()->ConversationsGetPending());
    svr.Post("/api/v1/conversations", MessagesRoute::Get()->ConversationsPost());
    svr.Delete("/api/v1/conversations", MessagesRoute::Get()->ConversationsDelete());

	svr.Get("/api/v1/my_auto_parts", AutoPartsRoute::Get()->MyAutoParts());
    svr.Post("/api/v1/auto_part", AutoPartsRoute::Get()->AutoPartPost());
    svr.Get("/api/v1/auto_part", AutoPartsRoute::Get()->AutoPartGet());
    svr.Delete("/api/v1/auto_part", AutoPartsRoute::Get()->AutoPartDelete());
    svr.Post("/api/v1/auto_part/avatar", AutoPartsRoute::Get()->AutoPartUploadAvatar());
    svr.Post("/api/v1/auto_part_response", AutoPartsRoute::Get()->AutoPartResponsePost());
    svr.Post("/api/v1/auto_part_response/avatar", AutoPartsRoute::Get()->AutoPartResponseUploadAvatar());
    svr.Get("/api/v1/auto_part_response", AutoPartsRoute::Get()->AutoPartResponseGet());
    svr.Get("/api/v1/auto_parts/notifications", AutoPartsRoute::Get()->AutoPartsGetNotifications());
    svr.Get("/api/v1/auto_parts/notification", AutoPartsRoute::Get()->AutoPartsGetNotification());
    svr.Delete("/api/v1/auto_parts/notification", AutoPartsRoute::Get()->AutoPartsDeleteNotification());
	svr.Post("/api/v1/auto_parts", AutoPartsRoute::Get()->AutoPartsPost());
	svr.Get("/api/v1/top_auto_parts", AutoPartsRoute::Get()->TopAutoParts());
	svr.Post("/api/v1/auto_parts/avatar", AutoPartsRoute::Get()->AutoPartsUploadAvatar());
	svr.Post("/api/v1/auto_parts/image", AutoPartsRoute::Get()->AutoPartsUploadImage());

    svr.Post("/post", [](const Request &req, Response &res) {
		req.params;
        auto image_file = req.get_file_value("image_file");
        auto text_file = req.get_file_value("text_file");

        cout << "image file length: " << image_file.content.length() << endl
             << "image file name: " << image_file.filename << endl
             << "text file length: " << text_file.content.length() << endl
             << "text file name: " << text_file.filename << endl;

        {
          ofstream ofs(image_file.filename, ios::binary);
          ofs << image_file.content;
        }
        {
          ofstream ofs(text_file.filename);
          ofs << text_file.content;
        }

        res.set_content("done", "text/plain");
    });

    auto ret = svr.set_mount_point("/assets", "./assets");
	ret = svr.set_mount_point("/data", "./data");

    svr.listen("192.168.1.6", 1234);
}
