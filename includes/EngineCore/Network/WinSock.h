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

	static void set_receive(std::function<void(std::string)> func);

private:
	static std::function<void(std::string)> m_receive;

	static SOCKET m_sock;
	static SOCKET m_client;
	static bool m_isServer;
	static bool m_isWorking;
};