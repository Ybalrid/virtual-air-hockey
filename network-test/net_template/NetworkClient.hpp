#ifndef NETCLI
#define NETCLI

#include "NetworkServer.hpp"
#include "NetConfig.hpp"



class NetworkClient : public NetworkWorker
{
public:
	NetworkClient();
	void update();
	int initialize(int port);


	void communicate(float frameTime);
	void doClientCommunication();
	void connectToServer();
	void getInfoFromServer();
	void sendInfoToServer();
	void roundStart();
	void checkNetworkTimeout();
	NetConfig config;

private:
	bool countDownOn;        // true when count down is displayed
	int playerN; // our player number
	USHORT remotePort; // remote port number
	Net net;
	//NetPlayer player[MAX_PLAYERS];
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

};

#endif //NETCLI