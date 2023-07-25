#ifndef __USER_MANAGER__
#define __USER_MANAGER__

#include <libpq-fe.h>
//#include <pqxx/pqxx>
#include <string>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <vector>

struct DBUser
{
    int Id;
    std::string Username;
    std::string Phone;
    std::string Password;
	int Type;
	std::string FirstName;
	std::string SecondName;
    std::string Avatar;
	int NumGolds;
	float Reputation;

    std::string ToJson(bool addNumGolds = false)
    {
        rapidjson::Document d;
        d.SetObject();
        d.AddMember("id", rapidjson::Value(Id), d.GetAllocator());
        rapidjson::Value p;

        p.SetString(Username.c_str(), d.GetAllocator());
        d.AddMember("username", p, d.GetAllocator());

        p.SetString(Phone.c_str(), d.GetAllocator());
        d.AddMember("phone", p, d.GetAllocator());

		d.AddMember("type", rapidjson::Value(Type), d.GetAllocator());

		p.SetString(FirstName.c_str(), d.GetAllocator());
		d.AddMember("first_name", p, d.GetAllocator());

		p.SetString(SecondName.c_str(), d.GetAllocator());
		d.AddMember("second_name", p, d.GetAllocator());

        p.SetString(Avatar.c_str(), d.GetAllocator());
        d.AddMember("avatar", p, d.GetAllocator());

        if (addNumGolds)
            d.AddMember("num_golds", rapidjson::Value(NumGolds), d.GetAllocator());
        //d.AddMember("reputation", rapidjson::Value(Reputation), d.GetAllocator());

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
    }
};

struct DBGift
{
    int Id;
    int Type;

    std::string ToJson()
    {
        rapidjson::Document d;
        d.SetObject();
        d.AddMember("id", rapidjson::Value(Id), d.GetAllocator());
        d.AddMember("type", rapidjson::Value(Type), d.GetAllocator());
        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
    }
};

class UserManager
{
public:
    static UserManager* Get();

//	void SetPG(PGconn* pPG);
//    void SetPsql(pqxx::connection* pPsql);
    int CreateUser(const std::string& username, const std::string& phone, const std::string& password, int type, const std::string& firstName, const std::string& secondName);
    DBUser* GetUser(int id);
    bool EditUser(int id, const std::string& firstName, const std::string& secondName, const std::string& phone);
    DBUser* GetUser(const std::string& username);
    bool ChangePassword(const std::string& phone, const std::string& password);
    int GetUserNumGolds(int id);
    void UserSetAutoPartMakes(int id, const std::vector<std::string>& makes);
    void UserSetAutoPartCategories(int id, const std::vector<int>& categories);
    std::vector<std::string> UserGetAutoPartMakes(int id);
    std::vector<int> UserGetAutoPartCategories(int id);
    bool UserEarnGold(int id);
    DBGift* GetUserGift(int id);
    bool UserReceiveGift(int giftId);

    bool SetUserAvatar(int userId, const std::string& avatarPath);


private:
    static UserManager* sInstance;
    //pqxx::connection* mPsql = nullptr;
//	PGconn* mPG = nullptr;
};

#endif // __USER_MANAGER__
