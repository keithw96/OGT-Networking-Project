#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <vector>
#include "Circle.h"

enum Packet
{
	P_ChatMessage,
	P_Test,
	P_CircleData,
	P_Authoritative,
	P_SetPlayer,
	P_EndGame
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

private:
	bool sendall(int ID, char * data, int totalbytes);
	bool recvall(int ID, char * data, int totalbytes);

	bool SendInt(int ID, int _int);
	bool GetInt(int ID, int & _int);

	bool SendCircle(int ID, Circle & _circle);
	bool GetCircle(int ID, Circle & _circle);

	bool getEndGame(int ID, bool& _end);
	bool sendEndGame(int ID, bool& _end);

	bool SendPacketType(int ID, Packet _packettype);
	bool GetPacketType(int ID, Packet & _packettype);

	bool SendString(int ID, std::string & _string);
	bool GetString(int ID, std::string & _string);

	bool ProcessPacket(int ID, Packet _packettype);

	bool sendSetPlayer(int ID, Circle & _circle);

	static void ClientHandlerThread(int ID);

private:
	SOCKET Connections[3];
	int TotalConnections = 0;
	int maxRoomConnections = 3;
	int currentRoomConnections = 0;
	
	
	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr);
	SOCKET sListen;
	bool enemy1Initialized = false;
	bool enemy2Initialized = false;
};

static Server * serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.
