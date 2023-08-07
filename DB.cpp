#include "DB.h"

using namespace std;
 
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

void DB::set(string type, string key, string record)
{
	if (type == "string")
	{
		db_str[key] = record;
		keys_all[key] = "string";
	}
	else if (type == "number")
	{
		db_number[key] = stod(record);
		keys_all[key] = "number";
	}
	else
	{
		if (record == "true")
		{
			db_bool[key] = "true";
		}
		else db_bool[key] = "false";

		keys_all[key] = "bool";
	}
}

string DB::get(string key)
{
	string type = type_check(key);

	if (type == "string")
	{
		auto pair = db_str.find(key);
		if (pair != db_str.end())
		{
			return pair->second;
		}
		else return "";
	}
	else if (type == "number")
	{
		auto pair = db_number.find(key);
		if (pair != db_number.end())
		{
			return to_string(pair->second);
		}
		else return "";
	}
	else if (type == "bool")
	{
		auto pair = db_bool.find(key);
		if (pair != db_bool.end())
		{
			if (pair->second == 1)
			{
				return "true";
			}
			else return "false";
		}
		else return "";
	}
	else
	{
		return "No record at this key";
	}
}

bool DB::erase(string key)
{
	string type = type_check(key);

	if (type == "string")
	{
		db_str.erase(key);
		keys_all.erase(key);
		return true;
	}
	else if (type == "number")
	{
		db_number.erase(key);
		keys_all.erase(key);
		return true;
	}
	else if (type == "bool")
	{
		db_bool.erase(key);
		keys_all.erase(key);
		return true;
	}
	else
	{
		return false;
	}
}