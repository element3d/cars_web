#ifndef __MASSEGES_MANAGER__
#define __MASSEGES_MANAGER__

#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


class MessagesManager
{
public:
    static MessagesManager* Get();

    bool MessagesGet(int from, int to, rapidjson::Document& d);
    int MessagesPost(int convId, int from, int to, const std::string& msg);

private:
    int _CreateConversation(int from, int to);

private:
    static MessagesManager* sInstance;
};

#endif // __MASSEGES_MANAGER__
