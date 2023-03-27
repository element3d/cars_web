#ifndef __AUTO_PART_MANAGER__
#define __AUTO_PART_MANAGER__

#include <libpq-fe.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <vector>
#include <string>

struct DBAutoPart
{
	int Id;
	int UserId;
	int Type;
    int Subtype;
	int Country;
	int Province;
	int SubProvince;
	int Originality;
	int Condition;
	int Price;
	std::vector<int> Fits;
	std::string Avatar;
	std::vector<std::string> Images;
};


struct DBAutoPartRequest
{
    int Id;
    int UserId;
    int Category;
    std::string Make;
    std::string Serie;
    std::string Model;
    std::string Description;
    std::string Avatar;
    int ResponseCount;
};

class AutoPartManager
{
public:
	static AutoPartManager* Get();

	void SetPG(PGconn* pPG);
    int CreateAutoPart(int userId, const std::string& autoPartJson);
    int CreateAutoPartResponse(int userId, const std::string& autoPartResponseJson);
    void GetAutoPartResponse(int userId, int id,  rapidjson::Document& d);
    void GetAutoPartNotifications(int userId, rapidjson::Document& d);
    void GetAutoPartNotification(int id, rapidjson::Document& d);
    void DeleteAutoPartNotification(int id, int resUserId);
    void GetAutoParts(int userId, std::vector<DBAutoPartRequest*>& autoParts);
    void GetAutoPart(int userId, int autoPartId, rapidjson::Document& d);
    void DeleteAutoPart(int userId, int autoPartId);
	void GetTopAutoPartsByType(int type, std::vector<DBAutoPart*>& autoParts);
    bool SetAutoPartAvatar(int autoPartId, const std::string& avatarPath);
    bool SetAutoPartResponseAvatar(int autoPartId, const std::string& avatarPath);
	bool AddAutoPartImage(int carId, const std::string& imagePath);

private:
    bool _ParseGPResult(PGresult* res, std::vector<DBAutoPartRequest*>& autoParts);

private:
	static AutoPartManager* sInstance;
//	PGconn* mPG = nullptr;
};

#endif // __USER_MANAGER__
