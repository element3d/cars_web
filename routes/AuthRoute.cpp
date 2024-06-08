#include "AuthRoute.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
//#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"

AuthRoute* AuthRoute::sInstance = nullptr;

AuthRoute* AuthRoute::Get()
{
    if (!sInstance) sInstance = new AuthRoute();
    return sInstance;
}

int sign_in(const std::string& username, const std::string& password, std::string& token)
{
    DBUser* pUser = UserManager::Get()->GetUser(username);
    if (!pUser)
    {
        return 404;
    }


    if (pUser->Password != password) return 403;

    token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_payload_claim("id", picojson::value(int64_t(pUser->Id)))
        .set_payload_claim("type", picojson::value(int64_t(pUser->Type)))
        .sign(jwt::algorithm::hs256{"secret"});

    delete pUser;
    return 200;
}

#include <cpr/cpr.h>
bool verify_phone_number(const std::string& phone) 
{
    std::string account_sid = "SK76522356cf80d9d737b97156309ba04f";
    std::string auth_token = "GLhxmBQXbc47H3zXhIUdCtn2ayaTeSiB";
    std::string service_sid = "VA86f2076b23370a9e3436e042643b3ac6";
    std::string to_number = phone;

    std::string url = "https://verify.twilio.com/v2/Services/" + service_sid + "/Verifications";

    std::string api_key_auth = account_sid + ":" + auth_token;
    std::string base64_auth = api_key_auth;//cpr::util::base64_encode(api_key_auth);

    cpr::Payload payload = 
    {
        {"To", to_number},
        {"Channel", "sms"}
    };

    cpr::Response response = cpr::Post(cpr::Url{url},
                                       cpr::Authentication{account_sid, auth_token},
                                       cpr::Payload{payload});

    if (response.status_code == 201) 
    {
        std::cout << "Verification request sent successfully" << std::endl;
        return true;
    } 
    else 
    {
        std::cerr << "Failed to send verification request: " << response.text << std::endl;
        return false;
    }

}

bool verify_twilio_code(const std::string& phone, const std::string& code) 
{
   std::string account_sid = "SK76522356cf80d9d737b97156309ba04f";
    std::string auth_token = "GLhxmBQXbc47H3zXhIUdCtn2ayaTeSiB";
    std::string service_sid = "VA86f2076b23370a9e3436e042643b3ac6";
    std::string to_number = phone;
    std::string verification_code = code;

    std::string url = "https://verify.twilio.com/v2/Services/" + service_sid + "/VerificationCheck";
    std::string username = account_sid;
    std::string password = auth_token;

    cpr::Payload payload = 
    {
        {"To", to_number},
        {"Code", verification_code}
    };

    cpr::Response response = cpr::Post(cpr::Url{url},
                                       cpr::Authentication{username, password},
                                       cpr::Payload{payload});

    if (response.status_code == 200) 
    {
        std::cout << "Verification check successful" << std::endl;
        return true;
    } else 
    {
        std::cerr << "Failed to perform verification check: " << response.text << std::endl;
        return false;
    }
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignUpVerify()
{
    return [](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string phone = document["phone"].GetString();
        DBUser* pUser = UserManager::Get()->GetUser(phone);
        if (pUser)
        {
            delete pUser;
            res.status = 403;
            res.set_content("", "text/plain");
            return;
        }

        bool success = verify_phone_number(std::string("+374") + phone);
        if (success)
          res.status = 200;
        else
          res.status = 400;
        res.set_content("", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::ChangePassword()
{
    return [](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string phone = document["phone"].GetString();
        std::string password = document["password"].GetString();
        std::string code = document["code"].GetString();
        DBUser* pUser = UserManager::Get()->GetUser(phone);
        if (!pUser)
        {
            res.status = 404;
            res.set_content("", "text/plain");
            return;
        }

        bool success = verify_twilio_code(std::string("+374") + phone, code);
        if (success) 
        {
          if (UserManager::Get()->ChangePassword(phone, password))
            res.status = 200;
          else 
            res.status = 400;
        }
        else
          res.status = 400;
        res.set_content("", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignInForgot()
{
    return [](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string phone = document["phone"].GetString();
        DBUser* pUser = UserManager::Get()->GetUser(phone);
        if (!pUser)
        {
            res.status = 404;
            res.set_content("", "text/plain");
            return;
        }

        bool success = verify_phone_number(std::string("+374") + phone);
        if (success)
          res.status = 200;
        else
          res.status = 400;
        res.set_content("", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignUp()
{
    return [](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string phone = document["phone"].GetString();
        std::string pwd = document["password"].GetString();

        DBUser* pUser = UserManager::Get()->GetUser(phone);
        if (pUser)
        {
            delete pUser;
            res.status = 403;
            res.set_content("", "text/plain");
            return;
        }

		    int type = document["type"].GetInt();
		    std::string firstName = document["first_name"].GetString();
		    std::string code = document["code"].GetString();

        bool ret = verify_twilio_code(std::string("+374") + phone, code);
        if (!ret) 
        {
           res.status = 400;
           res.set_content("", "text/plain");
           return;
        }
        int userId = UserManager::Get()->CreateUser(phone, pwd, type, firstName);

        std::string token;
        int status = sign_in(phone, pwd, token);
		    if (!token.size())
		    {
          res.status = status;
			    return;
		    }

        res.status = 200;
        res.set_content(token, "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignUpV2()
{
     return [](const httplib::Request& req, httplib::Response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string phone = document["phone"].GetString();
        std::string pwd = document["password"].GetString();

        DBUser* pUser = UserManager::Get()->GetUser(phone);
        if (pUser)
        {
            delete pUser;
            res.status = 403;
            res.set_content("", "text/plain");
            return;
        }

		std::string firstName = document["name"].GetString();
        int userId = UserManager::Get()->CreateUser(phone, pwd, 0, firstName);

        std::string token;
        int status = sign_in(phone, pwd, token);
		if (!token.size())
		{
            res.status = status;
			return;
		}

        res.status = 200;
        res.set_content(token, "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignInGoogle()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        std::string gtoken = req.get_header_value("Authentication");

        std::string url = "https://www.googleapis.com/oauth2/v2/userinfo";

        // Set up headers with the Authorization token
        cpr::Header headers{{"Authorization", "Bearer " + gtoken}};

        // Make the GET request
        cpr::Response r = cpr::Get(cpr::Url{url}, headers);

        // Check if the request was successful
        if (r.status_code == 200) 
        {
            rapidjson::Document document;
            document.Parse(r.text.c_str());

            std::string name = document["name"].GetString();
            std::string email = document["email"].GetString();

            int userId;
            DBUser* pUser =  UserManager::Get()->GetUserByEmail(email);
            if (pUser) 
            {   
                userId= pUser->Id;
                delete pUser;
            }
            else 
            {
                userId = UserManager::Get()->CreateUser(email, name);
                if (userId < 0) 
                {
                    res.status = 403;
                    res.set_content("Error", "text/plain");
                    return;
                }
            }

            std::string token = jwt::create()
            .set_issuer("auth0")
            .set_type("JWS")
            .set_payload_claim("id", picojson::value(int64_t(userId)))
            .set_payload_claim("auth_type", picojson::value("google"))
            .set_payload_claim("token2", picojson::value(gtoken))
            .sign(jwt::algorithm::hs256{"secret"});

            res.status = 200;
            res.set_content(token, "text/plain");
            return;
        } 
        else 
        {
            res.status = 403;
            res.set_content("Error", "text/plain");
            return;
        }
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignInGoogleWithEmail()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");

        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string email = document["email"].GetString();
        std::string name = document["name"].GetString();
       
        int userId;
        DBUser* pUser =  UserManager::Get()->GetUserByEmail(email);
        if (pUser) 
        {   
            userId= pUser->Id;
            delete pUser;
        }
        else 
        {
            userId = UserManager::Get()->CreateUser(email, name);
            if (userId < 0) 
            {
                res.status = 403;
                res.set_content("Error", "text/plain");
                return;
            }
        }

        std::string token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_payload_claim("id", picojson::value(int64_t(userId)))
        .set_payload_claim("auth_type", picojson::value("google"))
        .sign(jwt::algorithm::hs256{"secret"});

        res.status = 200;
        res.set_content(token, "text/plain");
        return;
        
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> AuthRoute::SignIn()
{
    return [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "*");
        res.set_header("Access-Control-Allow-Headers", "*");


        rapidjson::Document document;
        document.Parse(req.body.c_str());

        std::string username = document["username"].GetString();
        std::string pwd = document["password"].GetString();

        std::string token;
        int status = sign_in(username, pwd, token);
       // res.set_header("Access-Control-Allow-Origin", "*");
		if (!token.size())
		{
      res.status = status;
			return;
		}
        res.status = 200;
        res.set_content(token, "text/plain");
    };
}
