//Precompiled header are used in this project
#include "stdafx.h"

//Include Annwvyn Engine.
#include <Annwvyn.h>
//Every Annwvyn classes are in the Annwvyn namespace
using namespace Annwvyn; 

//Include our level/stages here
#include "myLevel.hpp"

#include "KeyboardCommands.hpp"

#include "FalconController.hpp"

#include "NetworkServer.hpp"
#include "NetworkClient.hpp"

DWORD WINAPI netUpdateLoop(LPVOID nwAddr)
{
	auto nw = (NetworkWorker*)nwAddr;
	while(true) nw->update();
}

AnnMain() //The application entry point is "AnnMain()". return type int.
{
	//Initialize the engine
	AnnEngine::openConsole();
	new AnnEngine("A game using Annwvyn");
	
	//Open a steam to AnnDebug and give it to the FalconClass
	std::ostream* out(new AnnDebug());
	
	//Initialise FalconController
	FalconController* Falcon = new FalconController(*out);
	Falcon->startUpdateThread();

	NetConfig config;
	config.IAmServer = false;
	config.port = ANVPORT;
	config.useTCP = false;
	config.serverAddress = "192.168.1.22";

	NetworkWorker* nw;
	if(config.IAmServer) nw = new NetworkServer;
	else nw = new NetworkClient;
	nw->config = config;
	nw->initialize(config.port);

	//Load your ressources here

	AnnEngine::Instance()->loadDir("media/table");
	AnnEngine::Instance()->loadDir("media/puck");
	AnnEngine::Instance()->loadDir("media/paddle");
	AnnEngine::Instance()->loadDir("media/room");
	AnnEngine::Instance()->getAudioEngine()->loadSndFile("media/contact.wav");

	AnnEngine::Instance()->initResources();
	AnnEngine::Instance()->oculusInit();
	AnnEngine::Instance()->setDebugPhysicState(false);

	//If the player has to folow the integrated physics scheme
	AnnEngine::Instance()->initPlayerPhysics();
	//AnnEngine::Instance()->useDefaultEventListener();
	AnnEngine::Instance()->getEventManager()->addListener(new KeyboardCommands);	

	//Do the other initialization herttEventListener(); //Basic events

	//Intentiate and register our basic level
	AnnEngine::Instance()->getLevelManager()->addLevel(new MyLevel);
	//This will make the game load and start the 1st we have registered
	AnnEngine::Instance()->getLevelManager()->jumpToFirstLevel();

	//This will recenter the facing direction of the player
	//in the real world with the VR
	AnnEngine::Instance()->resetOculusOrientation();
	
	//start the network loop:
	CreateThread(NULL, 0, netUpdateLoop, nw, NULL, NULL);

	//The game is rendering here now
	AnnEngine::Instance()->startGameplayLoop();


	//destroy the engine
	//delete AnnEngine::Instance();
	delete Falcon;
	exit(EXIT_SUCCESS);
}

