#include "DB.h"

using namespace std;

// data base func-s
 
vector<string> DB::keys_send()
{
	vector<string> keys;
	for (map<string, string>::iterator it = keys_all.begin(); it != keys_all.end(); ++it)
	{
		keys.push_back(it->first);
	}
	return keys;
}

string DB::type_check(string key)
{
	auto pair = keys_all.find(key);
	if (pair != keys_all.end())
	{
		return pair->second; //basicaly type
	}
	else return "";
}

// string data func-s

void DB_str::set(string key, string record)
{
	db_str[key] = record;
	keys_all[key] = "string";
}

string DB_str::get(string key)
{
	auto pair = db_str.find(key);
	if (pair != db_str.end())
	{
		return pair->second;
	}
}

void DB_str::erase(string key)
{
	db_str.erase(key);
	keys_all.erase(key);
}

// number data func-s

void DB_number::set(string key, string record)
{
	db_number[key] = stod(record);
	keys_all[key] = "number";
}

double DB_number::get(string key)
{
	auto pair = db_number.find(key);
	if (pair != db_number.end())
	{
		return pair->second;
	}
}

void DB_number::erase(string key)
{
	db_number.erase(key);
	keys_all.erase(key);
}

// bool data func-s

void DB_bool::set(string key, string record)
{
	if (record == "true")
	{
		db_bool[key] = "true";
	}
	else db_bool[key] = "false";

	keys_all[key] = "bool";
}

int DB_bool::get(string key)
{
	auto pair = db_bool.find(key);
	if (pair != db_bool.end())
	{
		return int(pair->second);
	}
}

void DB_bool::erase(string key)
{
	db_bool.erase(key);
	keys_all.erase(key);
}