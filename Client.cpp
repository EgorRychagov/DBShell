#include "DBshell.cpp"
#include <sstream>
#include <chrono>


class timer
{
	chrono::time_point< chrono::high_resolution_clock> start_point;
public:
	timer()
	{
		start_point = chrono::high_resolution_clock::now();
	}

	~timer()
	{
		stop();
	}

	void stop() 
	{
		auto end_point = chrono::high_resolution_clock::now();

		auto start = chrono::time_point_cast<chrono::microseconds>(start_point).time_since_epoch().count();
		auto end = chrono::time_point_cast<chrono::microseconds>(end_point).time_since_epoch().count();

		auto duration = end - start;
		double ms = duration * 0.001;
		cout << duration << "us ( " << ms << "ms )";
	}
};

vector<string> request_maker(string request) 
{
	string line;
	string word;
	vector<string> words;

	basic_stringstream linestream = stringstream(request);
	getline(linestream, line);
	line.erase(line.begin() + line.length() - 1);	// getting rid of \r
	basic_stringstream wordstream = stringstream(line);
	
	while (getline(wordstream, word, ' ')) 
	{
		words.push_back(word);
	}
	
	return words;
}

void say(SOCKET s, string msg)
{
	const char* c_msg = msg.c_str();
	send(s, c_msg, msg.length(), 0);
}

int main()//main()
{
	shell entrance;

	// initialize winsocket
	WSAData ws_data{};
	WORD ver = MAKEWORD(2, 2);
	int ws_ok = WSAStartup(ver, &ws_data);
	if (ws_ok != 0)
	{
		cerr << "Can't initialize winsock. Quitting." << endl;
		return 1;
	}

	// create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket. Quitting." << endl;
		return 1;
	}

	// binding ip adress and port to a socket
	//in_addr local{};
	sockaddr_in hint{};
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	inet_pton(AF_INET, "localhost", &(hint.sin_addr)); // my_ip = 192.168.31.51
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// telling winsock that socket is for listening
	listen(listening, SOMAXCONN);

	// w8ing for a connection
	sockaddr_in client{};
	int client_size = sizeof(client);
	SOCKET client_socket = accept(listening, (sockaddr*)&client, &client_size);

	if (client_socket == INVALID_SOCKET)
	{
		cerr << "Can't create client's socket" << endl;
	}
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "connected on port" << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" << htons(client.sin_port) << endl;
	}

	// closing listening socket
	closesocket(listening);

	// action's definition

	map<string, int> command;
	command["set"] = 0;
	command["delete"] = 1;
	command["get"] = 2;
	command["size"] = 3;
	command["keys"] = 4;
	command["disconnect"] = 5;
	command["help"] = 6;

	string msg;
	msg = "View available commands via typing 'help'\n";
	say(client_socket, msg);

	string action = "";
	string key;
	string record;
	vector<string> request;
	vector<string> keys;

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
			cerr << "error in recv(). Quitting." << endl;
			break;
		}
		else if (bytes_recieved == 0)
		{
			cout << "client disconnected" << endl;
			break;
		}
			
		// making request
		string str_buf(buf);

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
				key = request[1];
				record = request[2];

				entrance.record_add(key, record);
				msg = "Note was added to the key ";
				say(client_socket, msg);
				say(client_socket, key);
				
				break;
			case 1:
				key = request[1];

				if (entrance.record_delete(key))
				{
					msg = "Note was succesfully deleted\n";
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
				record = entrance.record_get(key);
				say(client_socket, record);
				
				break;
			case 3:
				record = to_string(entrance.DB_size());
				say(client_socket, record);
				
				break;
			case 4:
				keys = entrance.keys_get();

				if (keys.size() != 0)
				{
					for (int i = 0; i < keys.size(); i++)
					{
						say(client_socket, keys[i] + "\t");
					}
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
					"set <key> <record> (creates record with a key)\n"
					"delete <key> (deletes record at the key)\n"
					"get <key> (returns record at the key)\n"
					"size (returns quantity of records)\n"
					"keys (returns existing keys)\n"
					"disconnect\n";
				say(client_socket, msg);

				break;
			}
		}
	}

	return 0;
}