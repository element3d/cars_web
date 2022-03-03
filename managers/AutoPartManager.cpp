#include "AutoPartManager.h"
#include <iostream>


AutoPartManager* AutoPartManager::sInstance = nullptr;

AutoPartManager* AutoPartManager::Get()
{
	if (!sInstance) sInstance = new AutoPartManager();

	return sInstance;
}

void AutoPartManager::SetPG(PGconn* pPG)
{
	mPG = pPG;
}

int AutoPartManager::CreateAutoPart(const std::string& autoPartJson)
{
	rapidjson::Document d;
	d.Parse(autoPartJson.c_str());

	std::string sql = "INSERT INTO auto_parts(user_id , country, province, sub_province, type, sub_type, originality, condition, price) VALUES ("
		+ std::to_string(d["user_id"].GetInt()) + ", "
		+ std::to_string(d["country"].GetInt()) + ", "
		+ std::to_string(d["province"].GetInt()) + ", "
		+ std::to_string(d["sub_province"].GetInt()) + ", "
		+ std::to_string(d["type"].GetInt()) + ", "
		+ std::to_string(d["sub_type"].GetInt()) + ", "
		+ std::to_string(d["originality"].GetInt()) + ", "
		+ std::to_string(d["condition"].GetInt()) + ", "
		+ std::to_string(d["price"].GetInt()) + ""
		");";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return -1;
	}

	sql = "SELECT currval('auto_parts_id_seq');";
	res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return -1;
	}
	char* temp = (char*)calloc(256, sizeof(char));
	int rec_count = PQntuples(res);
	strcpy(temp, PQgetvalue(res, 0, 0));
	int id = atoi(temp);
	free(temp);
	return id;
}

void AutoPartManager::GetAutoParts(int userId, std::vector<DBAutoPart*>& autoParts)
{
	std::string sql = "SELECT * FROM auto_parts WHERE user_id = "
		+ std::to_string(userId) + ";";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}

	_ParseGPResult(res, autoParts);
	/*char* temp = (char*)calloc(256, sizeof(char));

	int rec_count = PQntuples(res);
	for (int i = 0; i < rec_count; i++)
	{

		DBAutoPart* pAutoPart = new DBAutoPart();
		strcpy(temp, PQgetvalue(res, i, 0));
		pAutoPart->Id = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 1));
		pAutoPart->UserId = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 2));
		pAutoPart->Country = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 3));
		pAutoPart->Province = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 4));
		pAutoPart->SubProvince = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 5));
		pAutoPart->Type = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 6));
		pAutoPart->Subtype = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 7));
		pAutoPart->Originality = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 8));
		pAutoPart->Condition = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 9));
		pAutoPart->Price = atof(temp);

		strcpy(temp, PQgetvalue(res, i, 11));
		pAutoPart->Avatar = temp;

		{
			std::string sql = "SELECT image_path FROM auto_part_images WHERE auto_part_id = "
				+ std::to_string(pAutoPart->Id) + ";";

			PGresult* res = PQexec(mPG, sql.c_str());
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				char* err = PQerrorMessage(mPG);
				fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
				PQclear(res);
				//exit_nicely(conn);
			}
			else
			{
				int imageCount = PQntuples(res);
				char* tmp = (char*)calloc(256, sizeof(char));
				for (int img = 0; img < imageCount; img++)
				{
					strcpy(tmp, PQgetvalue(res, img, 0));
					pAutoPart->Images.push_back(std::string(tmp));
				}
				free(tmp);
			}
			PQclear(res);
		}

		autoParts.push_back(pAutoPart);
	}
	free(temp);
	PQclear(res);*/
}

void AutoPartManager::GetTopAutoPartsByType(int type, std::vector<DBAutoPart*>& autoParts)
{
	std::string sql;
	if (type == -1)
		sql = "SELECT * FROM auto_parts;";
	else
		sql = "SELECT * FROM auto_parts WHERE type = " + std::to_string(type) + ";";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		//exit_nicely(conn);
		return;
	}

	_ParseGPResult(res, autoParts);
}

bool AutoPartManager::SetAutoPartAvatar(int autoPartId, const std::string& avatarPath)
{
	std::string sql = "UPDATE auto_parts SET avatar = '" + avatarPath + "' WHERE id = " + std::to_string(autoPartId) + ";";
	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "Add auto part image failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return false;
	}

	return true;
}

bool AutoPartManager::AddAutoPartImage(int autoPartId, const std::string& imagePath)
{
	std::string sql = "INSERT INTO auto_part_images(auto_part_id, image_path) VALUES ("
		+ std::to_string(autoPartId) + ", '"
		+ imagePath + "'"
		+ ");";

	PGresult* res = PQexec(mPG, sql.c_str());
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		char* err = PQerrorMessage(mPG);
		fprintf(stderr, "Add auto part image failed: %s", PQerrorMessage(mPG));
		PQclear(res);
		return false;
	}

	return true;
}

bool AutoPartManager::_ParseGPResult(PGresult* res, std::vector<DBAutoPart*>& autoParts)
{
	char* temp = (char*)calloc(256, sizeof(char));

	int rec_count = PQntuples(res);
	for (int i = 0; i < rec_count; i++)
	{

		DBAutoPart* pAutoPart = new DBAutoPart();
		strcpy(temp, PQgetvalue(res, i, 0));
		pAutoPart->Id = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 1));
		pAutoPart->UserId = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 2));
		pAutoPart->Country = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 3));
		pAutoPart->Province = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 4));
		pAutoPart->SubProvince = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 5));
		pAutoPart->Type = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 6));
		pAutoPart->Subtype = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 7));
		pAutoPart->Originality = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 8));
		pAutoPart->Condition = atoi(temp);

		strcpy(temp, PQgetvalue(res, i, 9));
		pAutoPart->Price = atof(temp);

		strcpy(temp, PQgetvalue(res, i, 11));
		pAutoPart->Avatar = temp;

		{
			std::string sql = "SELECT image_path FROM auto_part_images WHERE auto_part_id = "
				+ std::to_string(pAutoPart->Id) + ";";

			PGresult* res = PQexec(mPG, sql.c_str());
			if (PQresultStatus(res) != PGRES_TUPLES_OK)
			{
				char* err = PQerrorMessage(mPG);
				fprintf(stderr, "SELECT failed: %s", PQerrorMessage(mPG));
				PQclear(res);
				//exit_nicely(conn);
			}
			else
			{
				int imageCount = PQntuples(res);
				char* tmp = (char*)calloc(256, sizeof(char));
				for (int img = 0; img < imageCount; img++)
				{
					strcpy(tmp, PQgetvalue(res, img, 0));
					pAutoPart->Images.push_back(std::string(tmp));
				}
				free(tmp);
			}
			PQclear(res);
		}

		autoParts.push_back(pAutoPart);
	}
	free(temp);
	PQclear(res);
	return true;
}