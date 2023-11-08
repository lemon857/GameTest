#pragma once

#define BUFF_SIZE 1024

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <functional>

class WinSock
{
public:
	enum TypeDataPacket
	{
		text = 1,
		binData = 2,
		answer = 3,
		uncorrect = 0
	};

	struct DataPacket
	{
		bool is_read;
		char* data_buff;
		TypeDataPacket type;
	};
	
	WinSock() = delete;
	~WinSock() = delete;

	WinSock(const WinSock&) = delete;
	WinSock(const WinSock&&) = delete;
	WinSock& operator=(const WinSock&) = delete;
	WinSock& operator=(const WinSock&&) = delete;

	static int init_WinSock(bool isServer);
	static void close_WinSock();

	static void open_server(const char* addr, unsigned short port);
	static void open_client(const char* addr, unsigned short port);

	static void send_text(std::string text);

	static int send_data(char* data, int size, TypeDataPacket type);

	static void set_receive(std::function<void(DataPacket)> func);

private:
	static std::function<void(DataPacket)> m_receive;

	static SOCKET m_sock;
	static SOCKET m_client;

	static bool m_isServer;
	static bool m_isWorking;
};