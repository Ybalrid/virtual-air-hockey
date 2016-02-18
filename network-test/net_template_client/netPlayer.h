#pragma once
#include "stdafx.h"
#include "Annwvyn.h"

using namespace Annwvyn;


struct PlayerStc
{
	float X;
	float Y;
	float Z;
	UCHAR playerN;
	UCHAR flags;
};


class NetPlayer
{
private:
	int commErrors;
	bool connected;
	bool active;
	int timeout;
	UCHAR buttons;
	char netIP[16];
	int  commWarnings;   // count of communication warnings
public:
	NetPlayer(void);

	void setNetData(PlayerStc ss);

	// Set netConnected boolean
    void setConnected(bool c) {connected = c;}

	// Set netConnected boolean
    void setActive(bool c) {active = c;}

	bool getConnected() {return connected;}
	bool getActive() {return active;}

	// Increment netTimeout count
    void incTimeout() {timeout++;}

	// Set timeout
    void setTimeout(int t) {timeout = t;}

	int getTimeout() {return timeout;}

	// Set buttons
    void setButtons(UCHAR b)  {buttons = b;}

	// Return buttons
    UCHAR getButtons()      {return buttons;}

	// Return IP address of this player as dotted quad; nnn.nnn.nnn.nnn
    const char* getNetIP()  {return netIP;}

	// Set IP address of this player as dotted quad; nnn.nnn.nnn.nnn
    void setNetIP(const char* address)  {strcpy_s(netIP, address);}
    
	// Return commWarnings
    int getCommWarnings()   {return commWarnings;}
	void setCommWarnings(int w) {commWarnings = w;}

	// Return Ship Data
    PlayerStc getNetData();

	void setNetData(PlayerStc ss);

	int getcommErrors() { return commErrors;}
	// Set commErrors
    void setCommErrors(int e) {commErrors = e;}

	~NetPlayer(void);
};

