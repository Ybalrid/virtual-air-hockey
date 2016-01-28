#ifndef MY_LEVEL
#define MY_LEVEL

#include <Annwvyn.h>
using namespace Annwvyn;

const int ANVPORT = 48161;
const int MAX_PLAYERS = 2;

struct PlayerStc // state information for a player 
{
	float X;
	float Y;
	float radians;
	float rotation;
	short score;
	UCHAR playerN;
	UCHAR flags;
};

struct ToServerStc // stuff sent to a server from a client
{
	UCHAR buttons;
	UCHAR playerN;
};

struct ToClientStc // stuff sent to a client from the server
{
	Player player[MAX_PLAYERS];
	UCHAR  gameState;
	UCHAR  sounds; 
};

struct Player
{
	PlayerStc shipData; 
	//TorpedoStc torepedoData; TODO
};


//Each level you can create hinerits
//from AnnAbstractLevel
class MyLevel : public AnnAbstractLevel
{
private:
	Net net;
	AnnPlayer player[MAX_PLAYERS];
	int error;
	char localIP[16];
	float netTime;
public:
	MyLevel();
	void load();
	int initializeServer(int port);
	void communicate(float frameTime);
	void checkNetworkTimeout();
	void runLogic();
};

#endif //MY_LEVEL