#pragma once

#include "../httplib.h"

class AnalyticsRoute
{
public:
    static AnalyticsRoute* Get();

    std::function<void(const httplib::Request &, httplib::Response &)> PostPage();
    std::function<void(const httplib::Request &, httplib::Response &)> PostEvent();


private:
    static AnalyticsRoute* sInstance;
};