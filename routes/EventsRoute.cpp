#include "EventsRoute.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <jwt-cpp/jwt.h>
#include "../managers/UserManager.h"

EventsRoute* EventsRoute::sInstance = nullptr;

EventsRoute* EventsRoute::Get()
{
    if (!sInstance) sInstance = new EventsRoute();
    return sInstance;
}

EventsRoute::EventsRoute() 
{
    mChosenUsers = { "98828482" };
}

std::function<void(const httplib::Request &, httplib::Response &)> EventsRoute::GetEvents()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::vector<DBEvent*> events;
        std::string token = req.get_header_value("Authentication");
        int userId = -1;
        DBUser* pMe = nullptr;
        try 
        {
            auto decoded = jwt::decode(token);
            userId = decoded.get_payload_claim("id").as_int();
            pMe = UserManager::Get()->GetUser(userId);
        } 
        catch(...) 
        {

        }
        bool b = EventsManager::Get()->GetEvents(userId, events);
        
        rapidjson::Document d;
        d.SetArray();

        bool chosen = false;
        if (pMe) 
        {
          auto it = std::find(mChosenUsers.begin(), mChosenUsers.end(), pMe->Phone);
          if (it != mChosenUsers.end()) chosen = true;
        }

        for (auto pEvent : events) 
        {
            EEventStatus status = chosen && (pEvent->Id == 1 || pEvent->Id == 2) ? EEventStatus::Finished : pEvent->Status;
            
            rapidjson::Value e;
            e.SetObject();
            e.AddMember("id", pEvent->Id, d.GetAllocator());
            e.AddMember("status", (int)status, d.GetAllocator());
            e.AddMember("remaining", pEvent->RemainingNumUsers, d.GetAllocator());
            e.AddMember("access", (int)pEvent->Access, d.GetAllocator());
            
            rapidjson::Value t;
            t.SetString(pEvent->Title.c_str(), pEvent->Title.size(), d.GetAllocator());
            e.AddMember("title", t, d.GetAllocator());

            t.SetString(pEvent->Value1.c_str(), pEvent->Value1.size(), d.GetAllocator());
            e.AddMember("value1", t, d.GetAllocator());

            t.SetString(pEvent->Value2.c_str(), pEvent->Value2.size(), d.GetAllocator());
            e.AddMember("value2", t, d.GetAllocator());

            t.SetString(pEvent->Image.c_str(), pEvent->Image.size(), d.GetAllocator());
            e.AddMember("image", t, d.GetAllocator());

            rapidjson::Value users;
            users.SetArray();

            for (auto& user : pEvent->Users) 
            {
                rapidjson::Value u;
                u.SetObject();
                u.AddMember("id", user.Id, d.GetAllocator());
                int awarded = user.Awarded;
                if (awarded == 1 && user.Address.size()) awarded = 1;
                else if (awarded == 1 && user.Card.size()) awarded = 10;
                else if (awarded == 2 && user.Address.size()) awarded = 2;
                else if (awarded == 2 && user.Card.size()) awarded = 20;

                u.AddMember("awarded", awarded, d.GetAllocator());
         
                users.PushBack(u, d.GetAllocator());
            }
            e.AddMember("users", users, d.GetAllocator());

            d.PushBack(e, d.GetAllocator());
            delete pEvent;
        }

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        res.status = 200;
        res.set_content(buffer.GetString(), "text/plain");  
    };
}

std::function<void(const httplib::Request&, httplib::Response&)> EventsRoute::GetRecentEvent() 
{
   return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        int userId = -1;
        DBUser* pMe = nullptr;
        try
        {
            auto decoded = jwt::decode(token);
            userId = decoded.get_payload_claim("id").as_int();
            pMe = UserManager::Get()->GetUser(userId);
        }
        catch (...)
        {

        }
        std::vector<DBEvent*> events;
        bool b = EventsManager::Get()->GetEvents(userId, events);

        
        bool chosen = false;
        if (pMe) 
        {
          auto it = std::find(mChosenUsers.begin(), mChosenUsers.end(), pMe->Phone);
          if (it != mChosenUsers.end()) chosen = true;
        }

        rapidjson::Document d;
        d.SetObject();
        for (auto pEvent : events)
        {
            if (chosen && (pEvent->Id == 1 || pEvent->Id == 2)) continue;
            if (pEvent->Status == EEventStatus::NotStarted || pEvent->Access != EEventAccess::Allowed) continue;

            if (pEvent->Status == EEventStatus::Started) 
            {
              EEventStatus status = chosen && (pEvent->Id == 1 || pEvent->Id == 2) ? EEventStatus::Finished : pEvent->Status;
            
              d.AddMember("id", pEvent->Id, d.GetAllocator());
              d.AddMember("status", (int)status, d.GetAllocator());
              d.AddMember("remaining", pEvent->RemainingNumUsers, d.GetAllocator());
              d.AddMember("access", (int)pEvent->Access, d.GetAllocator());
            
              rapidjson::Value t;
              t.SetString(pEvent->Title.c_str(), pEvent->Title.size(), d.GetAllocator());
              d.AddMember("title", t, d.GetAllocator());

              t.SetString(pEvent->Value1.c_str(), pEvent->Value1.size(), d.GetAllocator());
              d.AddMember("value1", t, d.GetAllocator());

              t.SetString(pEvent->Value2.c_str(), pEvent->Value2.size(), d.GetAllocator());
              d.AddMember("value2", t, d.GetAllocator());

              t.SetString(pEvent->Image.c_str(), pEvent->Image.size(), d.GetAllocator());
              d.AddMember("image", t, d.GetAllocator());

              rapidjson::Value users;
              users.SetArray();

              for (auto& user : pEvent->Users) 
              {
                  rapidjson::Value u;
                  u.SetObject();
                  u.AddMember("id", user.Id, d.GetAllocator());
                  int awarded = user.Awarded;
                  if (awarded == 1 && user.Address.size()) awarded = 1;
                  else if (awarded == 1 && user.Card.size()) awarded = 10;
                  else if (awarded == 2 && user.Address.size()) awarded = 2;
                  else if (awarded == 2 && user.Card.size()) awarded = 20;

                  u.AddMember("awarded", awarded, d.GetAllocator());
         
                  users.PushBack(u, d.GetAllocator());
              }
              d.AddMember("users", users, d.GetAllocator());

              delete pEvent;

               rapidjson::StringBuffer buffer;
              buffer.Clear();

              rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
              d.Accept(writer);
              res.status = 200;
              res.set_content(buffer.GetString(), "application/json");
              return;
            }
            else if (pEvent->Status == EEventStatus::Finished) 
            {
                for (auto& user : pEvent->Users) 
                {
                  if (user.Id == userId && user.Awarded == 0) 
                  {
                      EEventStatus status = chosen && (pEvent->Id == 1 || pEvent->Id == 2) ? EEventStatus::Finished : pEvent->Status;
                      d.AddMember("id", pEvent->Id, d.GetAllocator());
                      d.AddMember("status", (int)status, d.GetAllocator());
                      d.AddMember("remaining", pEvent->RemainingNumUsers, d.GetAllocator());
                      d.AddMember("access", (int)pEvent->Access, d.GetAllocator());
            
                      rapidjson::Value t;
                      t.SetString(pEvent->Title.c_str(), pEvent->Title.size(), d.GetAllocator());
                      d.AddMember("title", t, d.GetAllocator());

                      t.SetString(pEvent->Value1.c_str(), pEvent->Value1.size(), d.GetAllocator());
                      d.AddMember("value1", t, d.GetAllocator());

                      t.SetString(pEvent->Value2.c_str(), pEvent->Value2.size(), d.GetAllocator());
                      d.AddMember("value2", t, d.GetAllocator());

                      t.SetString(pEvent->Image.c_str(), pEvent->Image.size(), d.GetAllocator());
                      d.AddMember("image", t, d.GetAllocator());

                      rapidjson::Value users;
                      users.SetArray();

                      for (auto& user : pEvent->Users) 
                      {
                          rapidjson::Value u;
                          u.SetObject();
                          u.AddMember("id", user.Id, d.GetAllocator());
                          int awarded = user.Awarded;
                          if (awarded == 1 && user.Address.size()) awarded = 1;
                          else if (awarded == 1 && user.Card.size()) awarded = 10;
                          else if (awarded == 2 && user.Address.size()) awarded = 2;
                          else if (awarded == 2 && user.Card.size()) awarded = 20;

                          u.AddMember("awarded", awarded, d.GetAllocator());
         
                          users.PushBack(u, d.GetAllocator());
                      }
                      d.AddMember("users", users, d.GetAllocator());

                      delete pEvent;

                      rapidjson::StringBuffer buffer;
                      buffer.Clear();

                      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                      d.Accept(writer);
                      res.status = 200;
                      res.set_content(buffer.GetString(), "application/json");
                      return;
                  }
                }
            }
        }
      
        res.status = 404;
        res.set_content("OK", "application/json");
   };
}

std::function<void(const httplib::Request&, httplib::Response&)> EventsRoute::GetEventsNotes()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        int userId = -1;
        DBUser* pMe = nullptr;
        try
        {
            auto decoded = jwt::decode(token);
            userId = decoded.get_payload_claim("id").as_int();
            pMe = UserManager::Get()->GetUser(userId);
        }
        catch (...)
        {

        }
        std::vector<DBEvent*> events;
        bool b = EventsManager::Get()->GetEvents(userId, events);

        
        bool chosen = false;
        if (pMe) 
        {
          auto it = std::find(mChosenUsers.begin(), mChosenUsers.end(), pMe->Phone);
          if (it != mChosenUsers.end()) chosen = true;
        }

        rapidjson::Document d;
        d.SetObject();
        int numNotes = 0;
        for (auto pEvent : events)
        {
            if (chosen && (pEvent->Id == 1 || pEvent->Id == 2)) continue;
            if (pEvent->Status == EEventStatus::Started) numNotes++;
        }
        d.AddMember("events", numNotes, d.GetAllocator());

        rapidjson::StringBuffer buffer;
        buffer.Clear();

        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);
        res.status = 200;
        res.set_content(buffer.GetString(), "application/json");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> EventsRoute::GetInception()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        DBEvent* pI = EventsManager::Get()->GetInception();
        if (!pI) 
        {
            res.status = 404;
            res.set_content("", "text/plain");
            return;
        }

        res.status = 200;
        res.set_content(pI->ToJson(), "text/plain");
        delete pI;
    };
}

std::function<void(const httplib::Request&, httplib::Response&)> EventsRoute::GetPitStop()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        int userId = -1;
        try 
        {
            auto decoded = jwt::decode(token);
            userId = decoded.get_payload_claim("id").as_int();

        } 
        catch(...) 
        {

        }

        DBEvent* pI = EventsManager::Get()->GetPitStop(userId);
        if (!pI)
        {
            res.status = 404;
            res.set_content("", "text/plain");
            return;
        }

        res.status = 200;
        res.set_content(pI->ToJson(), "text/plain");
        delete pI;
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> EventsRoute::EventsSetUser()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int eventId = -1;
        if (req.has_param("event_id")) eventId = atoi(req.get_param_value("event_id", 0).c_str());

        EventsManager::Get()->EventsSetUser(userId, eventId, req.body);
      
        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}

std::function<void(const httplib::Request &, httplib::Response &)> EventsRoute::EventsSetUserCard()
{
    return [this](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Methods", " POST, GET, PUT, OPTIONS");
        res.set_header("Access-Control-Allow-Origin", "*");

        std::string token = req.get_header_value("Authentication");
        auto decoded = jwt::decode(token);
        int userId = decoded.get_payload_claim("id").as_int();

        int eventId = -1;
        if (req.has_param("event_id")) eventId = atoi(req.get_param_value("event_id", 0).c_str());

        EventsManager::Get()->EventsSetUserCard(userId, eventId, req.body);
      
        res.status = 200;
        res.set_content("OK", "text/plain");
    };
}