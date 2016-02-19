#include "stdafx.h"
#include "NetworkClient.hpp"

NetworkClient::NetworkClient()
{
	tryToConnect = true;
	clientConnected = false;
}
int NetworkClient::initialize(int port)
{
	last = now = AnnEngine::Instance()->getTimeFromStartUp(); 
	return 0;
}
void NetworkClient::update(){
	AnnDebug() << "client update";
	now = AnnEngine::Instance()->getTimeFromStartUp();
	float frameTime = now - last;
	last = now;
	communicate(frameTime);
}

// --- Do network communications ---
void NetworkClient::communicate(float frameTime)
{	
	AnnDebug() << "Com";
	if(clientConnected)
		getInfoFromServer(); // Get game state from server
	// Calculate elapsed time for network communications
	/*netTime += frameTime;
	if(netTime < netNS::NET_TIMER) // if not time to communicate*/
		//return;
	if(tryToConnect)
		connectToServer(); // Attempt to connect to game server
	else if(clientConnected)
	{
		checkNetworkTimeout(); // check for disconnect from server
		sendInfoToServer(); //
	}
	netTime -= netNS::NET_TIMER;

}

void NetworkClient::connectToServer() 
{
	AnnDebug() << "Call connect to server..." ;
	static int step = 0;
    static float waitTime;  // seconds we have waited for a response from server
    int size;
    int newPort;
    std::string str;
    std::stringstream ss;

    if(clientConnected)     // if connected
    {
        tryToConnect = false;
        AnnDebug() << "Currently connected";
        return;
    }

        AnnDebug() << "----- Vr AirHockey Client -----";
      
		switch (step)
		{
		case 0:
       

		newPort = config.port;
        if(newPort == 0)
            newPort = netNS::DEFAULT_PORT;
        if(newPort > netNS::MIN_PORT && newPort < 65536)
        {
            port = newPort;
        }
        else
        AnnDebug() << "Invalid port number";
		strcpy(remoteIP, config.serverAddress.c_str());     
        error = net.createClient(remoteIP, port, netNS::UDP);
        if(error != netNS::NET_OK)          // if error
        {
            AnnDebug() << net.getError(error);    // display error message
            tryToConnect = false;
            return;
        }
        // send request to join the server
        AnnDebug() << "Attempting to connect with server."; // display message
        toServerData.playerN = 255;        // playerN=255 is request to join
        size = sizeof(toServerData);
        AnnDebug() << "'Request to join' sent to server.";
        error = net.sendData((char*) &toServerData, size, remoteIP, port);
        AnnDebug() << net.getError(error);
        waitTime = 0;

		step = 4;
        break;

    case 4:
        waitTime+=netTime;      // add time since last call to connect
        // if we timed out with no reponse to our request to join
        if (waitTime > CONNECT_TIMEOUT)        
        {
            AnnDebug() << "'Request to join' timed out.";
            //tryToConnect = false;
            return;
        }
        // read ConnectResponse from server
        size = sizeof(connectResponse);
        error = net.readData((char*)&connectResponse, size, remoteIP, remotePort);
        if (error == netNS::NET_OK)     // if read was OK
        {
            if(size == 0)   // if no data received
                return;
            // if the server sent back the proper ID then we are connected
            if (strcmp(connectResponse.response, netNS::SERVER_ID) == 0) 
            {
                if (connectResponse.number < MAX_CLIENT)   // if valid player number
                {
                    playerN = connectResponse.number;       // set my player number
                    ss << "Connected as player number: " << playerN;
                    AnnDebug() << ss.str();
                    clientConnected = true;
                    commErrors = 0;
                    commWarnings = 0;
                } 
                else
                    AnnDebug() << "Invalid player number received from server.";
            } 
            else if (strcmp(connectResponse.response, netNS::SERVER_FULL) == 0) 
                AnnDebug() << "Server Full";
            else
            {
                AnnDebug() << "Invalid ID from server. Server sent:";
                AnnDebug() << connectResponse.response;
            }
        }
        else        // read error
        {
            AnnDebug() << net.getError(error);
        }
        tryToConnect = false;
        step = 0;
    }	
}

//=============================================================================
// Check for network timeout
//=============================================================================
void NetworkClient::checkNetworkTimeout()
{
    if (!clientConnected)
        return;
    commErrors++;                         // increment timeout count
    if (commErrors > netNS::MAX_ERRORS)   // if communication timeout
    {
        clientConnected = false;
        AnnDebug() << "***** Disconnected from server. *****";
    }
}


//=============================================================================
// Send the keypress codes to the server
//=============================================================================
void NetworkClient::sendInfoToServer()
{
    int size;
    // prepare structure to be sent
    //toServerData.buttons = buttonState;
    toServerData.playerN = playerN;
    // send data from client to server
    size = sizeof(toServerData);
    error = net.sendData((char*) &toServerData, size, remoteIP, remotePort);
}


//=============================================================================
// Get toClientData from server.
// called by client to get game state from server
//=============================================================================
void NetworkClient::getInfoFromServer()
{
    int size;
    size = sizeof(toClientData);
    int readStatus = net.readData((char *)&toClientData, size, remoteIP, remotePort);
    if( readStatus == netNS::NET_OK && size > 0) 
    {
		for(int i=0; i<MAX_CLIENT; i++)        // for all player positions
        {
            // load new data into each player
           // player[i].setNetData(toClientData.player[i].playerData);
           
        }

        // Game state
        // Bit 0 = roundStart
        // Bits 1-7 reserved for future use
        if((toClientData.gameState & ROUND_START_BIT) &&
            countDownOn == false)
            roundStart();

        gameState = toClientData.gameState; // save new game state

        commErrors = 0;
        commWarnings = 0;
    }

}


//=============================================================================
// Start a new round of play
//=============================================================================
void NetworkClient::roundStart()
{
    // Start ships on opposite sides of planet in stable clockwise orbit
    //player[0].setX();
    //player[1].setX();
    //player[0].setY();
    //player[1].setY();

    countDownTimer = COUNT_DOWN;
    countDownOn = true;
}


