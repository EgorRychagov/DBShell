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
};

class DB_str : public DB 
{
public:
	void set(std::string key, std::string record);
	std::string get(std::string key);
	void erase(std::string key);
};

class DB_number : public DB 
{
public:
	void set(std::string key, std::string record);
	double get(std::string key);
	void erase(std::string key);
};

class DB_bool : public DB 
{
public:
	void set(std::string key, std::string record);
	int get(std::string key);
	void erase(std::string key);
};