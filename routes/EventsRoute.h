#ifndef __EVENTS_ROUTE__
#define __EVENTS_ROUTE__

#include "../httplib.h"
#include "../managers/EventsManager.h"

class EventsRoute
{
public:
	  static EventsRoute* Get();

	  std::function<void(const httplib::Request &, httplib::Response &)> GetInception();
	  std::function<void(const httplib::Request &, httplib::Response &)> EventsSetUser();
	  std::function<void(const httplib::Request &, httplib::Response &)> EventsSetUserCard();

private:
	static EventsRoute* sInstance;
};

#endif // __EVENTS_ROUTE__
