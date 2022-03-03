//
//  upload.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//

#include <fstream>
#include "httplib.h"
#include <iostream>
#include <pqxx/pqxx>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <jwt-cpp/jwt.h>
#include "managers/UserManager.h"
#include "routes/UsersRoute.h"
#include "routes/CarsRoute.h"
#include "routes/AutoPartsRoute.h"

using namespace httplib;
using namespace std;
using namespace pqxx;


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

int main(void) {
    Server svr;

	PGconn* pg = PQconnectdb("host=127.0.0.1 port=5432 dbname=cars user=postgres");

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

	UserManager::Get()->SetPG(pg);//SetPsql(psql);
	CarManager::Get()->SetPG(pg);
	AutoPartManager::Get()->SetPG(pg);
	if (PQstatus(pg) != CONNECTION_OK)
	{
		fprintf(stderr, "Connection to database failed: %s",
			PQerrorMessage(pg));
	//	exit_nicely(conn);
	}

    svr.Get("/", [](const Request & /*req*/, Response &res) {
        res.set_content(html, "text/html");
    });

    svr.Post("/api/v1/signup", AuthRoute::Get()->SignUp());
    svr.Post("/api/v1/signin", AuthRoute::Get()->SignIn());

    svr.Get("/api/v1/me", UsersRoute::Get()->Me());

	svr.Get("/api/v1/my_cars", CarsRoute::Get()->MyCars());
	svr.Get("/api/v1/top_cars", CarsRoute::Get()->TopCars());
	svr.Get("/api/v1/cars", CarsRoute::Get()->CarsGet());
	svr.Post("/api/v1/cars", CarsRoute::Get()->CarsPost());
	svr.Post("/api/v1/cars/avatar", CarsRoute::Get()->CarsUploadAvatar());
	svr.Post("/api/v1/cars/image", CarsRoute::Get()->CarsUploadImage());

	svr.Get("/api/v1/my_auto_parts", AutoPartsRoute::Get()->MyAutoParts());
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

    svr.listen("192.168.1.5", 1234);
}
