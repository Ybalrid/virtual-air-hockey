#ifndef MY_LEVEL
#define MY_LEVEL
#include <Annwvyn.h>
#include "net.h"
#include "netPlayer.h"
using namespace Annwvyn;

const int ANVPORT = 48161;
const int MAX_PLAYERS = 2;
const int CONNECT_TIMEOUT = 10;
const int ROUND_START_BIT = 0x01;
const int COUNT_DOWN = 5;


struct PlayerStc // state information for a player 
{
	float x;
	float y;
	float z;
	UCHAR playerN;
	UCHAR flags;
};

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
	UCHAR buttons;
	UCHAR playerN;
};

struct Player
{
	PlayerStc playerData;
	PaddleStc paddleData;
};

struct ToClientStc // stuff sent to a client from the server
{
	Player player[MAX_PLAYERS];
	UCHAR  gameState;
	UCHAR  sounds; 
};


//Each level you can create hinerits
//from AnnAbstractLevel
class MyLevel : public AnnAbstractLevel
{
private:
	bool countDownOn;        // true when count down is displayed
	int playerN; // our player number
	USHORT remotePort; // remote port number
	Net net;
	NetPlayer player[MAX_PLAYERS];
	int error;
	char localIP[16];
	float netTime;
	bool connected;
	bool active;
	bool clientConnected;
	float countDownTimer;
	UINT commErrors;
    UINT commWarnings;
    bool tryToConnect;
	UCHAR gameState; // current game state
	ToServerStc toServerData;
	ToClientStc toClientData;
	ConnectResponse connectResponse;
	int playerCount;    // number of players in game
	char remoteIP[16];
	USHORT port;
public:
	MyLevel();
	void load();
	int initializeServer(int port);
	void communicate(float frameTime);
	void doClientCommunication();
	void connectToServer();
	void getInfoFromServer();
	void sendInfoToServer();
	void roundStart();
	void checkNetworkTimeout();
	void runLogic();
};

#endif //MY_LEVEL