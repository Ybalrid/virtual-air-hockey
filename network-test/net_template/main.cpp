//Precompiled header are used in this project
#include "stdafx.h"

//Include Annwvyn Engine.
#include <Annwvyn.h>
//Every Annwvyn classes are in the Annwvyn namespace
using namespace Annwvyn; 

//Include our level/stages here
#include "myLevel.hpp"


#include "NetworkServer.hpp"
#include "NetworkClient.hpp"




const bool SERVER = true;

AnnMain() //The application entry point is "AnnMain()". return type int.
{
	//Initialize the engine
	AnnEngine::openConsole();
	new AnnEngine("A game using Annwvyn");
	NetConfig config;

	//just for test:
	config.IAmServer = false;
	config.useTCP = false;
	config.serverAddress = "127.0.0.1";

	NetworkWorker* nw;
	if(config.IAmServer)
	nw = new NetworkServer;
	else
	{
	nw = new NetworkClient;
	static_cast<NetworkClient*>(nw)->config = config;
	}

	nw->initialize(ANVPORT);

	
	//Load your ressources here
	AnnEngine::Instance()->initResources();
	AnnEngine::Instance()->oculusInit();
	//If the player has to folow the integrated physics scheme
	AnnEngine::Instance()->initPlayerPhysics();
	//Do the other initialization herttEventListener(); //Basic events

	//Intentiate and register our basic level
	AnnEngine::Instance()->getLevelManager()->addLevel(new MyLevel);
	//This will make the game load and start the 1st we have registered
	AnnEngine::Instance()->getLevelManager()->jumpToFirstLevel();

	//This will recenter the facing direction of the player
	//in the real world with the VR
	AnnEngine::Instance()->resetOculusOrientation();
	
	//The game is rendering here now:
	AnnEngine::Instance()->useDefaultEventListener();
	do
	{
		nw->update();
	}
	while(AnnEngine::Instance()->refresh());

	//destroy the engine
	delete AnnEngine::Instance();
	return EXIT_SUCCESS;
}

