#pragma once

#define BUFF_SIZE 1024

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <functional>

class Stopwatch;

class WinSock
{
public:

	WinSock() = delete;
	~WinSock() = delete;

	WinSock(const WinSock&) = delete;
	WinSock(const WinSock&&) = delete;
	WinSock& operator=(const WinSock&) = delete;
	WinSock& operator=(const WinSock&&) = delete;

	static int init_WinSock();
	static void close_WinSock();
	static void disconnect();

	static void open_server(const char* addr, unsigned short port);
	static void open_client(const char* addr, unsigned short port);

	static int send_data(char* data, int size);

	static void set_receive_callback(std::function<void(char* data, int size)> func);

	static void set_ping_callback(std::function<void(double ping)> func);

	static void set_disconnect_callback(std::function<void()> func);

private:
	static std::function<void(char* data, int size)> m_receive_callback;
	static std::function<void(double ping)> m_ping_callback;
	static std::function<void()> m_disconnect_callback;

	static SOCKET m_sock;
	static SOCKET m_client;

	static bool m_isServer;
	static bool m_isWorking;
	static Stopwatch m_ping_timer;
};