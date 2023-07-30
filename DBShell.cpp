#include "DBShell.h"

using namespace std;

void shell::record_add(string type, string key, string record)
{
	if (type == "string")
	{
		db_str.set(key, record);
	}
	else if (type == "number")
	{
		db_number.set(key, record);
	}
	else
	{
		db_bool.set(key, record);
	}
}

string shell::record_get(string key)
{
	string type = db.type_check(key);

	if (type == "string")
	{
		return db_str.get(key);
	}
	else if (type == "number")
	{
		return to_string(db_number.get(key));
	}
	else if (type == "bool")
	{
		return to_string(db_bool.get(key));
	}
	else
	{
		return "No record at this key";
	}
}

bool shell::record_delete(string key)
{
	string type = db.type_check(key);

	if (type == "string")
	{
		db_str.erase(key);
		return true;
	}
	else if (type == "number")
	{
		db_number.erase(key);
		return true;
	}
	else if (type == "bool")
	{
		db_bool.erase(key);
		return true;
	}
	else
	{
		return false;
	}
}

int shell::DB_size()
{
	return int(db.keys_send().size());
}

vector<string> shell::keys_get()
{
	return db.keys_send();
}