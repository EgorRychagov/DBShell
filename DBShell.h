#pragma once
#include "DB.h"
#include <map>
#include <string>
#include <vector>

class shell
{
	DB data;

public:
	void record_add(std::string type, std::string key, std::string record);
	std::string record_get(std::string key);
	bool record_delete(std::string key);
	int DB_size();
	std::vector<std::string> keys_get();
};