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

enum class EEventAccess 
{
  Allowed = 0,
  Denied = 1
};

struct EventUser 
{
  int Id;
  std::string Address;
  std::string Card;
  int Awarded;
};

struct DBEvent 
{
    int Id;
    std::string Title;
    EEventStatus Status;
    std::string Value1;
    std::string Value2;
    std::string Image;
    int Limits;
    int NumUsers;
    int RemainingNumUsers;
    EEventAccess Access;
    std::vector<EventUser> Users;

    std::string ToJson()
    {
        rapidjson::Document d;
        d.SetObject();
        d.AddMember("num_users", rapidjson::Value(NumUsers), d.GetAllocator());
        d.AddMember("remaining_num_users", rapidjson::Value(RemainingNumUsers), d.GetAllocator());
        d.AddMember("status", rapidjson::Value((int)Status), d.GetAllocator());
        d.AddMember("access", rapidjson::Value((int)Access), d.GetAllocator());
      
        rapidjson::Value users;
        users.SetArray();

        for (auto& user : Users) 
        {
            rapidjson::Value u;
            u.SetObject();
            u.AddMember("id", user.Id, d.GetAllocator());
            u.AddMember("awarded", user.Address.size() != 0 || user.Card.size() != 0, d.GetAllocator());
         
            users.PushBack(u, d.GetAllocator());
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
    bool GetEvents(int userId, std::vector<DBEvent*>& events);
    DBEvent* GetInception();
    DBEvent* GetPitStop(int userId);
    void InceptionAddUser(int userId);
    void PitStopAddUser(int userId);
    void EventsSetUser(int userId, int eventId, const std::string& userJson);
    void EventsSetUserCard(int userId, int eventId, const std::string& cardJson);

private:
    static EventsManager* sInstance;
};

#endif // __EVENTS_MANAGER__
