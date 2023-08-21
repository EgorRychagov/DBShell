#pragma once
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

class timer
{
	std::chrono::time_point< std::chrono::high_resolution_clock> start_point;

public:
	timer()
	{
		start_point = std::chrono::high_resolution_clock::now();
	}

	~timer()
	{
		stop();
	}

	void stop()
	{
		auto end_point = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_point).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();

		//auto duration = end - start;
		double ms = (end - start) * 0.001;
		std::cout << "* Done in  " << ms << "ms *\n";
	}
};

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