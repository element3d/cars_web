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

class AutoPartManager
{
public:
	static AutoPartManager* Get();

	void SetPG(PGconn* pPG);
	int CreateAutoPart(const std::string& autoPartJson);
	void GetAutoParts(int userId, std::vector<DBAutoPart*>& autoParts);
	void GetTopAutoPartsByType(int type, std::vector<DBAutoPart*>& autoParts);
	bool SetAutoPartAvatar(int carId, const std::string& avatarPath);
	bool AddAutoPartImage(int carId, const std::string& imagePath);

private:
	bool _ParseGPResult(PGresult* res, std::vector<DBAutoPart*>& autoParts);

private:
	static AutoPartManager* sInstance;
	PGconn* mPG = nullptr;
};

#endif // __USER_MANAGER__
