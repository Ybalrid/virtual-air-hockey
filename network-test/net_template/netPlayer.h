#pragma once
class netPlayer
{
private:
	bool connected;
	bool active;
	int timeout;
public:
	netPlayer(void);

	// Set netConnected boolean
    void setConnected(bool c) {connected = c;}

	// Set netConnected boolean
    void setActive(bool c) {active = c;}

	bool getConnected() {return connected;}
	bool getActive() {return active;}

	// Increment netTimeout count
    void incTimeout() {timeout++;}
	int getTimeout() {return timeout;}
	

	
	~netPlayer(void);
};

