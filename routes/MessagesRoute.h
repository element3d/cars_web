#ifndef __MESSAGES_ROUTE__
#define __MESSAGES_ROUTE__

#include "../httplib.h"
#include "../managers/MessagesManager.h"

class MessagesRoute
{
public:
	static MessagesRoute* Get();
	MessagesRoute();

	std::function<void(const httplib::Request &, httplib::Response &)> MessagesGet();
	std::function<void(const httplib::Request &, httplib::Response &)> MessagesPost();
	std::function<void(const httplib::Request &, httplib::Response &)> MessagesUploadImage();

	std::function<void(const httplib::Request &, httplib::Response &)> ConversationsGet();
	std::function<void(const httplib::Request &, httplib::Response &)> ConversationsPost();

private:
	static MessagesRoute* sInstance;
};

#endif // __MESSAGES_ROUTE__