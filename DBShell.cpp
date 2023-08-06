#include "DBShell.h"

using namespace std;

void shell::record_add(string type, string key, string record)
{
	data.set(type, key, record);
}

string shell::record_get(string key)
{	
	return data.get(key);
}

bool shell::record_delete(string key)
{
	return data.erase(key);
}

int shell::DB_size()
{
	return int(data.keys_send().size());
}

vector<string> shell::keys_get()
{
	return data.keys_send();
}