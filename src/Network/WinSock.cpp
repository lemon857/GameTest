#include "EngineCore/Network/WinSock.h"

#include "EngineCore/System/Log.h"

#include <vector>
#include <thread>

SOCKET WinSock::m_sock;
SOCKET WinSock::m_client;
bool WinSock::m_isServer;
bool WinSock::m_isWorking;
std::function<void(WinSock::DataPacket)> WinSock::m_receive;

int WinSock::init_WinSock(bool isServer)
{
	// WinSock initialization
	WSADATA wsData;

	if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
		LOG_ERROR("Error WinSock version initializaion #");
		LOG_ERROR(WSAGetLastError());
		return 2;
	}
	else
		LOG_INFO("WinSock initialization is OK");

	m_isServer = isServer;
	m_isWorking = true;
	return 0;
}

void WinSock::close_WinSock()
{
	m_isWorking = false;
	if (m_isServer) closesocket(m_client);
	closesocket(m_sock);
	WSACleanup();
	LOG_INFO("WinSock shuit down");
}

void WinSock::open_client(const char* addr, unsigned short port)
{
	// init ip addr
	in_addr ip_to_num;

	if (inet_pton(AF_INET, addr, &ip_to_num) <= 0) {
		LOG_ERROR("Error in IP translation to special numeric format");
		return;
	}
	// Server socket initialization
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET) {
		LOG_ERROR("Error initialization socket # {0}", WSAGetLastError());
		close_WinSock();
		return;
	}
	else
		LOG_INFO("Socket initialization is OK");
	// Server socket binding
	sockaddr_in Info;
	ZeroMemory(&Info, sizeof(Info));	// Initializing servInfo structure

	Info.sin_family = AF_INET;
	Info.sin_addr = ip_to_num;
	Info.sin_port = htons(port);

	if (connect(m_sock, (sockaddr*)&Info, sizeof(Info)) != 0) {
		LOG_ERROR("Connection to Server is FAILED. Error # {0}", WSAGetLastError());		
		close_WinSock();
		return;
	}
	else
		LOG_INFO("Connection established SUCCESSFULLY. Ready to send a message to Server");

	

	std::thread t([&]() {
		char buff[BUFF_SIZE];							// Buffers for sending and receiving data
		short packet_size = 0;
		while (m_isWorking) {

			packet_size = recv(m_sock, buff, BUFF_SIZE, 0);

			if (packet_size == SOCKET_ERROR) {
				LOG_ERROR("Can't receive message from Server. Error # {0}", WSAGetLastError());
				close_WinSock();
				return;
			}
			else
			{
				std::string str =
					"type:" + std::to_string((short)buff[0]) +
					" lenght: " + std::to_string((int)buff[1]) +
					" count: " + std::to_string((int)buff[5]) + " text: ";
				m_receive({ false, buff, (TypeDataPacket)((char)buff[0]) });
				for (size_t i = 0; i < (int)(buff[5]); i++)
				{
					str += buff[i + 9];
				}
				LOG_INFO("Receive message: {0}", str);
			}
		}
		close_WinSock();
		});
	
	t.detach();	
}

void WinSock::open_server(const char* addr, unsigned short port)
{
	// init ip addr
	in_addr ip_to_num;

	if (inet_pton(AF_INET, addr, &ip_to_num) <= 0) {
		LOG_ERROR("Error in IP translation to special numeric format");
		return;
	}

	// Server socket initialization
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_sock == INVALID_SOCKET) {
		LOG_ERROR("Error initialization socket # {0}", WSAGetLastError());
		close_WinSock();
		return;
	}
	else
		LOG_INFO("Socket initialization is OK");
	// Server socket binding
	sockaddr_in Info;
	ZeroMemory(&Info, sizeof(Info));	// Initializing servInfo structure

	Info.sin_family = AF_INET;
	Info.sin_addr = ip_to_num;
	Info.sin_port = htons(port);

	if (bind(m_sock, (sockaddr*)&Info, sizeof(Info)) != 0) {
		LOG_ERROR("Error Socket binding to server info. Error # {0}", WSAGetLastError());
		close_WinSock();
		return;
	}
	else
		LOG_INFO("Binding socket to Server info is OK");
	
		
	std::thread t([&]() {
		//Starting to listen to any Clients

		if (listen(m_sock, SOMAXCONN) != 0) {
			LOG_WARN("Can't start to listen to. Error # {0}", WSAGetLastError());
			close_WinSock();
			return;
		}
		else {
			LOG_INFO("Listening...");
		}

		//Client socket creation and acception in case of connection
		sockaddr_in clientInfo;
		ZeroMemory(&clientInfo, sizeof(clientInfo));	// Initializing clientInfo structure

		int clientInfo_size = sizeof(clientInfo);

		m_client = accept(m_sock, (sockaddr*)&clientInfo, &clientInfo_size);

		if (m_client == INVALID_SOCKET) {
			LOG_ERROR("Client detected, but can't connect to a client. Error # {0}", WSAGetLastError());			
			close_WinSock();
			return;
		}
		else {
			LOG_INFO("Connection to a client established successfully");
			char clientIP[22];

			inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format

			LOG_INFO("Client connected with IP address: {0}", clientIP);

		}

		char buff[BUFF_SIZE];							// Buffers for sending and receiving data
		short packet_size = 0;
		while (m_isWorking) {

			packet_size = recv(m_client, buff, BUFF_SIZE, 0);

			if (packet_size == SOCKET_ERROR) {
				LOG_ERROR("Can't receive message from Client. Error # {0}", WSAGetLastError());
				close_WinSock();
				return;
			}
			else
			{
				std::string str =
					"type:" + std::to_string(int((char)buff[0])) +
					" lenght: " + std::to_string((int)buff[1]) +
					" count: " + std::to_string((int)buff[5]) + " text: ";
				m_receive({ false, buff, (TypeDataPacket)((char)buff[0]) });
				for (size_t i = 0; i < (int)(buff[5]); i++)
				{
					str += buff[i + 9];
				}
				LOG_INFO("Receive message: {0}", str);
			}
		}
		close_WinSock();
		});

	t.detach();
}

void WinSock::send_text(std::string text)
{
	char buff[BUFF_SIZE];

	buff[0] = (int)text.length();

	char* tempData = text.data();

	for (size_t i = 0; i < text.length(); i++)
	{
		buff[i + 4] = tempData[i];
	}

	send_data(buff, sizeof(int) + text.length(), TypeDataPacket::text);
}

void WinSock::set_receive(std::function<void(DataPacket)> func)
{
	m_receive = func;
}

int WinSock::send_data(char* data, int size, TypeDataPacket type)
{
	char buff[BUFF_SIZE];

	buff[0] = (char)(type);

	buff[1] = size + 5;

	for (size_t i = 0; i < size; i++)
	{
		buff[i + 5] = data[i];
	}

	if (send(m_isServer ? m_client : m_sock, buff, BUFF_SIZE, 0) == SOCKET_ERROR) {
		LOG_ERROR("Can't send message. Error # {0}", WSAGetLastError());
		close_WinSock();
		return -1;
	}
	return 0;
}