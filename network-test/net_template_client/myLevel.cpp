#include "stdafx.h"
#include "myLevel.hpp"
#include "net.h"

MyLevel::MyLevel() : AnnAbstractLevel()
{
}

void MyLevel::load()
{
	//For having a lighter syntax :
	auto engine(AnnEngine::Instance());
	
	//Load Sinbad:
	auto Sinbad (engine->createGameObject("Sinbad.mesh"));
	Sinbad->setUpPhysics(100, phyShapeType::boxShape);
	//Add it to the level list
	levelContent.push_back(Sinbad);

	//Load Ground:
	auto Ground (engine->createGameObject("Ground.mesh"));
	Ground->setPos(0,-2,0);
	Ground->setUpPhysics();
	//Add it to the level list
	levelContent.push_back(Ground);

	//Create a light source
	auto light(engine->addLight());
	light->setPosition(0,1,3);
	//Add it to the level lst
	levelLighting.push_back(light);

	engine->setAmbiantLight(Ogre::ColourValue::White/2);

	engine->getPlayer()->setPosition(AnnVect3(0,1,0));
	

	engine->resetPlayerPhysics();

}


// --- Do network communications ---
void MyLevel::communicate(float frameTime)
{
	if(clientConnected)
		getInfoFromServer(); // Get game state from server
	// Calculate elapsed time for network communications
	netTime += frameTime;
	if(netTime < netNS::NET_TIMER) // if not time to communicate
		return;
	if(tryToConnect)
		connectToServer(); // Attempt to connect to game server
	else if(clientConnected)
	{
		checkNetworkTimeout(); // check for disconnect from server
		sendInfoToServer(); //
	}
	netTime -= netNS::NET_TIMER;

}

void MyLevel::connecToServer() 
{
	static int step = 0;
    static float waitTime;  // seconds we have waited for a response from server
    int size;
    int newPort;
    std::string str;
    std::stringstream ss;

    if(clientConnected)     // if connected
    {
        tryToConnect = false;
        console->print("Currently connected");
        return;
    }

    switch(step)
    {
    case 0:
        AnnDebug() << "----- Vr AirHockey Client -----";
        AnnDebug() << "Enter IP address or name of server:" ;
        //console->clearInput();              // clear input text
        //console->show();
        step = 1;
        break;

    case 1:
        // str = console->getInput(); logic to get user input
        if(str == "")                       // if no address entered
            return;
        // strcpy_s(remoteIP, str.c_str());    // copy address to remoteIP
        //console->clearInput();              // clear input text
        AnnDebug() << "Enter port number, 0 selects default 48161: ";
        step = 2;
        break;

    case 2:
        // str = console->getInput(); logic to get the user input
        if (str == "")
            return;
        newPort = atoi(str.c_str());
        if(newPort == 0)
            newPort = netNS::DEFAULT_PORT;
        if(newPort > netNS::MIN_PORT && newPort < 65536)
        {
            port = newPort;
            step = 3;
        }
        else
        AnnDebug() << "Invalid port number";
        // console->clearInput();
        break;

    case 3:
        // create UDP client
        error = net.createClient(remoteIP, port, netNS::UDP);
        if(error != netNS::NET_OK)          // if error
        {
            console->print(net.getError(error));    // display error message
            tryToConnect = false;
            step = 0;
            return;
        }
        // send request to join the server
        AnnDebug() << "Attempting to connect with server."; // display message
        toServerData.playerN = 255;        // playerN=255 is request to join
        size = sizeof(toServerData);
        AnnDebug() << "'Request to join' sent to server.";
        error = net.sendData((char*) &toServerData, size, remoteIP, port);
        console->print(net.getError(error));
        waitTime = 0;
        step = 4;
        break;

    case 4:
        waitTime+=netTime;      // add time since last call to connect
        // if we timed out with no reponse to our request to join
        if (waitTime > CONNECT_TIMEOUT)        
        {
            step = 3;               // send request again
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
                if (connectResponse.number < MAX_PLAYERS)   // if valid player number
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
void MyLevel::checkNetworkTimeout()
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
void MyLevel::sendInfoToServer()
{
    int size;
    // prepare structure to be sent
    toServerData.buttons = buttonState;
    toServerData.playerN = playerN;
    // send data from client to server
    size = sizeof(toServerData);
    error = net.sendData((char*) &toServerData, size, remoteIP, remotePort);
}


//=============================================================================
// Get toClientData from server.
// called by client to get game state from server
//=============================================================================
void MyLevel::getInfoFromServer()
{
    int size;
    size = sizeof(toClientData);
    int readStatus = net.readData((char *)&toClientData, size, remoteIP, remotePort);
    if( readStatus == netNS::NET_OK && size > 0) 
    {
        for(int i=0; i<MAX_PLAYERS; i++)        // for all player positions
        {
            // load new data into each player
            player[i].setNetData(toClientData.player[i].playerData);
           
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




void MyLevel::runLogic()
{
	float frameTime = AnnEngine::Instance()->getTimeFromStartUp();
	communicate(frameTime);
}