#ifndef NETMANAGER
#define NETMANAGER

#include "net.h"
#include "ServerClient.h"
#include "NetConfig.hpp"

const int ANVPORT = 48161;
const int MAX_CLIENT = 1;
const int CONNECT_TIMEOUT = 10;
const int ROUND_START_BIT = 0x01;
const int COUNT_DOWN = 5;

// Sent to client in response to connection request
struct ConnectResponse
{
    char response[netNS::RESPONSE_SIZE];    // server response
    UCHAR   number;                         // player number if connected
};

struct PaddleStc 
{
	float x;
	float y;
	float z;
};

struct ToServerStc // stuff sent to a server from a client
{
	UCHAR playerN;
	AnnVect3 ClientPaddlePos;
};

struct Player
{
	PlayerStc playerData;
	PaddleStc paddleData;
};

struct ToClientStc // stuff sent to a client from the server
{
	//Player player[MAX_PLAYERS];
	UCHAR  gameState;
	//UCHAR  sounds; 

	AnnVect3 postition;
};

enum workerType{SERVER, CLIENT};

class NetworkWorker
{
public:
	NetworkWorker();
	static NetworkWorker* getSingleton();
	virtual int initialize(int port) = 0;
	virtual void update() = 0;
	NetConfig config;

	void setLocalPositon(AnnVect3 position);
	AnnVect3 getDistantPosition();
	workerType getType();

protected:
	workerType myType;
	static NetworkWorker* singleton;
	
	//Local is the position on te current game. distant is the position on the other pcover the net.
	AnnVect3 localPosiion, distantPosition, referencePuckPosiion;
};

class NetworkServer : public NetworkWorker
{
public:
	NetworkServer();
	//NetworkServer* getSingleton();

	//Net code methods
	int initialize(int port);
	void communicate(float frameTime);
	void doClientCommunication();
	void checkNetworkTimeout();
	void prepareDataForClient();
	void clientWantToJoin();

	void update();
private:

	//Networking stuff : 
	bool countDownOn;
	int countDownTimer;
	bool roundOver; // true when round is over
	Net net;
	ServerClient player[MAX_CLIENT];
	int error;
	char localIP[16];
	float netTime;
	bool connected;
	bool active;
	ToServerStc toServerData;
	ToClientStc toClientData;
	ConnectResponse connectResponse;
	int playerCount;    // number of players in game
	char remoteIP[16];
	USHORT port;
};

#endif