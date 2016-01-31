#ifndef MY_LEVEL
#define MY_LEVEL
#include <Annwvyn.h>
#include "net.h"
using namespace Annwvyn;

const int ANVPORT = 48161;
const int MAX_PLAYERS = 2;

namespace Annwvyn {
	void setActive(AnnPlayer& p);
	void setConnected(AnnPlayer& p);
};

struct PlayerStc // state information for a player 
{
	float X;
	float Y;
	float radians;
	float rotation;
	UCHAR playerN;
	UCHAR flags;
};

struct ToServerStc // stuff sent to a server from a client
{
	UCHAR buttons;
	UCHAR playerN;
};

struct Player
{
	PlayerStc shipData; 
	
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
	Net net;
	AnnPlayer player[MAX_PLAYERS];
	int error;
	char localIP[16];
	float netTime;
	bool connected;
	bool active;
public:
	MyLevel();
	void load();
	int initializeServer(int port);
	void communicate(float frameTime);
	void checkNetworkTimeout();
	void setConnected(bool c) { connected = c; };
	void setActive(bool a) { active = a; }
	void runLogic();
};

#endif //MY_LEVEL