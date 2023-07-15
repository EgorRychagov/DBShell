#include<iostream>
#include<string>
#include<map>
#include<vector>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class shell
{
	map<string, string> DB;	

public:

	void record_add(string key, string record)
	{
		DB[key] = record;
	}

	string record_get(string key)
	{
		auto pair = DB.find(key);

		if (pair != DB.end())
		{
			return pair->second;
		}
		else
		{
			return "Error: No record at this key";
		}
	}

	bool record_delete(string key)
	{		
		auto pair = DB.find(key);

		if (pair != DB.end())
		{
			DB.erase(key);
			return true;
		}

		return false;
	}

	int DB_size()
	{
		return DB.size();
	}

	vector<string> keys_get()
	{
		vector<string> keys;

		for (map<string, string>::iterator it = DB.begin(); it != DB.end(); ++it)
		{
			keys.push_back(it->first);
		}	

		return keys;
	}
};