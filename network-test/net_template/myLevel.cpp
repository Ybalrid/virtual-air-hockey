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

int MyLevel::initializeServer(int port)
{
	std::stringstream ss;
	if(port < netNS::MIN_PORT)
	{
		AnnDebug() << "Invalid port number";
		return netNS::NET_ERROR;
	}
	// ------ init network stuff -------
	error = net.createServer(port, netNS::UDP);
	if(error != netNS::NET_OK)
	{
		AnnDebug() << net.getError(error);
		return netNS::NET_ERROR;
	}

	for (int i=0; i<MAX_PLAYERS; i++)
	{
		player[i].setConnected(false);
		player[i].setActive(false);
	}

	// AnnEngine::Instance()->getPlayer()->setPosition(AnnVect3(0,0,2.1));

	AnnDebug() << "---- Server ----";
	net.getLocalIP(localIP);
	ss << "Server IP: " << localIP;
	AnnDebug() << ss.str();
	ss.str("");
	ss << "Port: " << port;
	AnnDebug() << ss.str();
	return netNS::NET_OK;
}

// --- Do network communications ---
void MyLevel::communicate(float frameTime)
{
	// communicate with client 
	doClientCommunication(); 
	// Calculate elapsed time for network communications
	netTime += frameTime;
	if(netTime < netNS::NET_TIMER)
		return;
	netTime -= netNS::NET_TIMER;
	// check for inactive clients, called every NET_TIMER seconds
	checkNetworkTimeout(); 
}

// --- Check for network timeout ---

void MyLevel::checkNetworkTimeout()
{
	std::stringstream ss;
	for (int i=0; i<MAX_PLAYERS; i++)
	{
		if (player[i].getConnected())
		{
			player[i].incTimeout();
			// if communication timout 
			if (player[i].getTimeout() > netNS::MAX_ERRORS)
			{
				player[i].setActive(false);
				ss << "***** Player " << i << " disconnected. *****";
				AnnDebug() << ss.str();
			}
		}
	}
}

void MyLevel::doClientCommunication() 
{
	int playN;
	int size;
	prepareDataForClient(); 
	for (int i=0; i<MAX_PLAYERS; i++)
	{
		size = sizeof(toServerData);
		if( net.readData((char*) &toServerData, size, remoteIP) == netNS::NET_OK)
		{
			if(size > 0)
			{
				playN = toServerData.playerN;
				if (playN == 255)
				{
					//clientWantsTiJoin(); TODO
				}
				else
				{
					if (player[playN].getConnected())
					{
						if (player[playN].getActive())
							player[playN].setButtons(toServerData.buttons);
						size = sizeof(toClientData);
						// send player the latest game data
						net.sendData((char*) &toClientData, size, player[i].getNetIP());
						player[playN].setTimeout(0);
						player[playN].setCommWarnings(0);
					}
				}
			}
		}
		else    // No more incomming data ? break !!
		{
			break;
		}
	}
}

void MyLevel::prepareDataForClient() 
{
	for (int i=0; i<MAX_PLAYERS; i++)
	{
		toClientData.player[i].playerData = player[i].getNetData();
	}
}

//========================================================================
// Client is requesting to join game
//========================================================================

void MyLevel::clientWantToJoin()
{
	std::stringstream ss;
	int size;
	int status;
	connectResponse.number = 255;
	if (playerCount == 0)
	{
		roundOver = true;
		// score system goes here (TODO)
	}
	AnnDebug() << "Player requesting to join.";
	// find available player position to use
	for(int i=0; i<MAX_PLAYERS; i++) // search all player position
	{
		if (player[i].getConnected() == false) // if this position available
		{
			player[i].setConnected(true);
			player[i].setTimeout(0);
			player[i].setCommWarnings(0);
			player[i].setNetIP(remoteIP); // save player's IP
			player[i].setCommErrors(0); // clear old errors
			// send SERVER_ID adn player number to client
			strcpy_s(connectResponse.response, netNS::SERVER_ID);
			connectResponse.number = (UCHAR)i;
			size = sizeof(connectResponse);
			status = net.sendData((char*) &connectResponse, size, remoteIP);

		}
	}

}


void MyLevel::runLogic()
{
}