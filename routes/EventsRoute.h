#ifndef __EVENTS_ROUTE__
#define __EVENTS_ROUTE__

#include "../httplib.h"
#include "../managers/EventsManager.h"

class EventsRoute
{
public:
	  static EventsRoute* Get();
		EventsRoute();

	  std::function<void(const httplib::Request &, httplib::Response &)> GetEvents();
		std::function<void(const httplib::Request &, httplib::Response &)> GetRecentEvent();
	  std::function<void(const httplib::Request&, httplib::Response&)> GetEventsNotes();
	  std::function<void(const httplib::Request &, httplib::Response &)> GetInception();
	  std::function<void(const httplib::Request&, httplib::Response&)> GetPitStop();
	  std::function<void(const httplib::Request &, httplib::Response &)> EventsSetUser();
	  std::function<void(const httplib::Request &, httplib::Response &)> EventsSetUserCard();

private:
	static EventsRoute* sInstance;
	std::vector<std::string> mChosenUsers;
};

#endif // __EVENTS_ROUTE__
