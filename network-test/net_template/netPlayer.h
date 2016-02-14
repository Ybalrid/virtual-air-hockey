#pragma once
class NetPlayer
{
private:
	bool connected;
	bool active;
	int timeout;
public:
	NetPlayer(void);

	// Set netConnected boolean
    void setConnected(bool c) {connected = c;}

	// Set netConnected boolean
    void setActive(bool c) {active = c;}

	bool getConnected() {return connected;}
	bool getActive() {return active;}

	// Increment netTimeout count
    void incTimeout() {timeout++;}
	int getTimeout() {return timeout;}
	

	
	~NetPlayer(void);
};

