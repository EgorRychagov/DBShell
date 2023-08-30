#include "DB.h"
#include <sstream>
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

std::vector<std::string> request_maker(std::string request)
{
	std::string line;
	std::string word;
	std::vector<std::string> words;

	std::basic_stringstream linestream = std::stringstream(request);
	getline(linestream, line);
	line.erase(line.begin() + line.length() - 1);	// getting rid of \r
	std::basic_stringstream wordstream = std::stringstream(line);
	
	while (getline(wordstream, word, ' ')) 
	{
		words.push_back(word);
	}
	
	return words;
}

void say(SOCKET s, std::string msg)
{
	const char* c_msg = msg.c_str();
	send(s, c_msg, int(msg.length()), 0);
}

int main()
{
	DB entrance{};

	bool benchmarking;

	std::string mode;
	std::cout << "Mode (DEF/BM): ";	
	std::cin >> mode;

	if (mode == "BM") benchmarking = true;
	else benchmarking = false;
	
	if (benchmarking)
	{
		// benchmark skript

		std::vector<std::string> bench_types;
		std::vector<std::string> bench_keys;
		std::vector<std::string> bench_records;

		int stages = 4;
		int amount = 100;
		int initial_size = 10000;

		// table preparation
		std::cout << "Type\tAVG Time (ms)\tInitial Size\n\n";

		for (int stage = 0; stage < stages; stage++)
		{
			// initial data			

			for (int i = 0; i < initial_size; i++) // string
			{
				entrance.set("string", std::to_string(i), "str");
			}
			for (int i = 0; i < initial_size; i++) // bool
			{
				entrance.set("bool", std::to_string(initial_size + i), "true");
			}
			for (int i = 0; i < initial_size; i++) // numerical
			{
				entrance.set("number", std::to_string(initial_size * 2 + i), std::to_string(i));
			}

			// coocking data for recording

			for (int i = 0; i < amount; i++) // string
			{
				bench_types.push_back("string");
				bench_keys.push_back(std::to_string(initial_size * 3 + i));
				bench_records.push_back("str");
			}
			for (int i = 0; i < amount; i++) // bool
			{
				bench_types.push_back("bool");
				bench_keys.push_back(std::to_string(initial_size * 3 + i + amount));
				bench_records.push_back("true");
			}
			for (int i = 0; i < amount; i++) // numerical
			{
				bench_types.push_back("number");
				bench_keys.push_back(std::to_string(initial_size * 3 + i + amount * 2));
				bench_records.push_back(std::to_string(i));
			}

			// time measurment

			std::vector<double> str_timings;
			std::vector<double> bool_timings;
			std::vector<double> num_timings;
			std::chrono::time_point< std::chrono::high_resolution_clock> start_point;

			for (int i = 0; i < amount; i++) // string
			{
				start_point = std::chrono::high_resolution_clock::now();

				entrance.set(bench_types[i], bench_keys[i], bench_records[i]);

				auto end_point = std::chrono::high_resolution_clock::now();
				auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_point).time_since_epoch().count();
				auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();
				double ms = (end - start) * 0.001;
				str_timings.push_back(ms);
			}
			for (int i = 0; i < amount; i++) // bool
			{
				start_point = std::chrono::high_resolution_clock::now();

				entrance.set(bench_types[amount + i], bench_keys[amount + i], bench_records[i]);

				auto end_point = std::chrono::high_resolution_clock::now();
				auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_point).time_since_epoch().count();
				auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();
				double ms = (end - start) * 0.001;
				bool_timings.push_back(ms);
			}
			for (int i = 0; i < amount; i++) // numerical
			{
				start_point = std::chrono::high_resolution_clock::now();

				entrance.set(bench_types[amount * 2 + i], bench_keys[amount * 2 + i], bench_records[amount * 2 + i]);

				auto end_point = std::chrono::high_resolution_clock::now();
				auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_point).time_since_epoch().count();
				auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_point).time_since_epoch().count();
				double ms = (end - start) * 0.001;
				num_timings.push_back(ms);
			}

			// avg time analysis

			double str_avg = 0;
			double bool_avg = 0;
			double num_avg = 0;

			for (int i = 0; i < amount; i++)
			{
				str_avg += str_timings[i];
				bool_avg += bool_timings[i];
				num_avg += num_timings[i];
			}
			str_avg /= amount;
			bool_avg /= amount;
			num_avg /= amount;
			
			std::cout << "String\t" << str_avg << "\t\t" << initial_size << std::endl;
			std::cout << "Bool\t" << bool_avg << "\t\t" << initial_size << std::endl;
			std::cout << "Double\t" << num_avg << "\t\t" << initial_size << std::endl;
			
			// refreshing

			initial_size *= 10;
			bench_types.clear();
			bench_keys.clear();
			bench_records.clear();
			entrance.drop();
		}
	}
	else
	{
		// initialize winsocket
		WSAData ws_data{};
		WORD ver = MAKEWORD(2, 2);
		int ws_ok = WSAStartup(ver, &ws_data);
		if (ws_ok != 0)
		{
			std::cerr << "Can't initialize winsock. Quitting." << std::endl;
			return 1;
		}

		// create a socket
		SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
		if (listening == INVALID_SOCKET)
		{
			std::cerr << "Can't create a socket. Quitting." << std::endl;
			return 1;
		}

		// binding ip adress and port to a socket
		//in_addr local{};
		sockaddr_in hint{};
		hint.sin_family = AF_INET;
		hint.sin_port = htons(54000);
		inet_pton(AF_INET, "localhost", &(hint.sin_addr)); // localhost / my_ip = 
		bind(listening, (sockaddr*)&hint, sizeof(hint));

		// telling winsock that socket is for listening
		listen(listening, SOMAXCONN);

		// w8ing for a connection
		sockaddr_in client{};
		int client_size = sizeof(client);
		SOCKET client_socket = accept(listening, (sockaddr*)&client, &client_size);

		if (client_socket == INVALID_SOCKET)
		{
			std::cerr << "Can't create client's socket" << std::endl;
		}
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];
		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			std::cout << host << "connected on port" << service << std::endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			std::cout << host << "connected on port" << htons(client.sin_port) << std::endl;
		}

		// closing listening socket
		closesocket(listening);

		// action's definition

		std::unordered_map<std::string, int> command;

		command["string"] = 0;
		command["number"] = 0;
		command["bool"] = 0;

		command["erase"] = 1;
		command["get"] = 2;
		command["size"] = 3;
		command["keys"] = 4;
		command["disconnect"] = 5;
		command["help"] = 6;

		std::string msg;
		msg = "View available commands via typing 'help'\n";
		say(client_socket, msg);

		std::string action = "";
		std::string key;
		std::string record;
		std::string type;
		std::vector<std::string> request;
		std::vector<std::string> keys;

		// getiing data and making echo message
		char buf[4096];

		while (true)
		{
			ZeroMemory(buf, 4096);

			msg = "> ";
			say(client_socket, msg);

			// recieving data
			int bytes_recieved = recv(client_socket, buf, 4096, 0);

			if (bytes_recieved == SOCKET_ERROR)
			{
				std::cerr << "error in recv(). Quitting." << std::endl;
				break;
			}
			else if (bytes_recieved == 0)
			{
				std::cout << "client disconnected" << std::endl;
				break;
			}

			// making request
			std::string str_buf(buf);

			request = request_maker(str_buf);
			if (!request.size())
			{
				continue;
			}

			action = request[0];

			if (command.find(request[0]) == command.end())
			{
				msg = "There is no such an action\n";
				say(client_socket, msg);
			}
			else
			{
				switch (command[action])
				{
				case 0:					
					type = request[0];
					key = request[1];
					record = request[2];

					entrance.set(type, key, record);
					msg = "Note was added to the key <";
					say(client_socket, msg);
					say(client_socket, key + ">\n");					

					break;
				case 1:
					
					key = request[1];

					if (entrance.del(key))
					{
						msg = "Erased\n";
						say(client_socket, msg);
					}
					else
					{
						msg = "There is no record at this key\n";
						say(client_socket, msg);
					}					

					break;
				case 2:					
					key = request[1];
					record = entrance.get(key) + "\n";
					say(client_socket, record);

					break;
				case 3:
					record = std::to_string(entrance.keys_send().size()) + "\n";
					say(client_socket, record);

					break;
				case 4:
					keys = entrance.keys_send();

					if (keys.size() != 0)
					{
						for (int i = 0; i < keys.size(); i++)
						{
							say(client_socket, keys[i] + " ");
						}
						say(client_socket, "\n");
					}
					else
					{
						msg = "Data base is empty now\n";
							say(client_socket, msg);
					}

					break;
				case 5:
					// closing client socket
					closesocket(client_socket);
					//cleaning up winsock
					WSACleanup();
					msg = "Disconnected\n";
					say(client_socket, msg);

					return 1;
					break;
				case 6:
					msg = "Available commands:\n"
						"<type> <key> <record> (creates record)\n"
						"erase <key> (erases record at the key)\n"
						"get <key> (returns record at the key)\n"
						"size (returns quantity of records)\n"
						"keys (returns existing keys)\n"
						"disconnect\n";
					say(client_socket, msg);

					break;
				}
			}
		}
	}

	return 0;
}