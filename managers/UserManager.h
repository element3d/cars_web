#ifndef __USER_MANAGER__
#define __USER_MANAGER__

#include <libpq-fe.h>
#include <pqxx/pqxx>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>

struct DBUser
{
    int Id;
    std::string Phone;
    std::string Password;
	int Type;
	std::string FirstName;
	std::string SecondName;
	int NumGolds;
	float Reputation;

    std::string ToJson()
    {
        rapidjson::Document d;
        d.SetObject();
        d.AddMember("id", rapidjson::Value(Id), d.GetAllocator());
        rapidjson::Value p;
        p.SetString(Phone.c_str(), d.GetAllocator());
        d.AddMember("phone", p, d.GetAllocator());

		d.AddMember("type", rapidjson::Value(Type), d.GetAllocator());

		p.SetString(FirstName.c_str(), d.GetAllocator());
		d.AddMember("first_name", p, d.GetAllocator());

		p.SetString(SecondName.c_str(), d.GetAllocator());
		d.AddMember("second_name", p, d.GetAllocator());

		d.AddMember("num_golds", rapidjson::Value(NumGolds), d.GetAllocator());
		d.AddMember("reputation", rapidjson::Value(Reputation), d.GetAllocator());

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

	void SetPG(PGconn* pPG);
    void SetPsql(pqxx::connection* pPsql);
    bool CreateUser(const std::string& phone, const std::string& password, int type, const std::string& firstName, const std::string& secondName);
    DBUser* GetUser(int id);
    DBUser* GetUser(const std::string& phone);

private:
    static UserManager* sInstance;
    pqxx::connection* mPsql = nullptr;
	PGconn* mPG = nullptr;
};

#endif // __USER_MANAGER__
