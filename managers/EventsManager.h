#ifndef __EVENTS_MANAGER__
#define __EVENTS_MANAGER__

#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

enum class EEventStatus 
{
  Started = 0,
  NotStarted = 1,
  Finished = 2
};

struct DBInception 
{
    int NumUsers;
    int RemainingNumUsers;
    EEventStatus Status;
    std::vector<int> Users;

    std::string ToJson()
    {
        rapidjson::Document d;
        d.SetObject();
        d.AddMember("num_users", rapidjson::Value(NumUsers), d.GetAllocator());
        d.AddMember("remaining_num_users", rapidjson::Value(RemainingNumUsers), d.GetAllocator());
        d.AddMember("status", rapidjson::Value((int)Status), d.GetAllocator());
      
        rapidjson::Value users;
        users.SetArray();
        for (auto& userId : Users) 
        {
          users.PushBack(userId, d.GetAllocator());
        }
        d.AddMember("users", users, d.GetAllocator());

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        return buffer.GetString();
    }
};

class EventsManager
{
public:
    static EventsManager* Get();
    DBInception* GetInception();
    void InceptionAddUser(int userId);

private:
    static EventsManager* sInstance;
};

#endif // __EVENTS_MANAGER__
