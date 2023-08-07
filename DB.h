#pragma once
#include <map>
#include <string>
#include <vector>

class DB
{
protected:
	std::map<std::string, std::string> keys_all;

	std::map<std::string, std::string> db_str;
	std::map<std::string, double> db_number;
	std::map<std::string, bool> db_bool;

public:
	std::vector<std::string> keys_send();
	std::string type_check(std::string key);

	void set(std::string type, std::string key, std::string record);
	std::string get(std::string key);
	bool erase(std::string key);
};