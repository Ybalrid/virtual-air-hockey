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
	static int step =0;
	static float waitTime; // Seconds we have waited for response server

	int size;
	int newPort;
	std::string str;
	std::stringstream ss;
	if(clientConnected) // if connected
	{
		tryToConnect = false;
		AnnDebug() << "Currently connected";
		return;
	}
	switch(step)
	{
	case 0:
		AnnDebug() << "----- VR AirHockey -----";
		AnnDebug() << "Enter IP adress or name of server";
		// now I have to wait for Arthur to bring a solution to I/O stuff here !!
		// console -> input
		// console -> show()
		step = 1;
		break;
	case 1:
		// here I need to get the user input stuf ( str = console->getInput(); )
		if(str == "")
			return;
		strcpy_s(remoteIP, str.c_str());
		// console->clearInput(); clear input text
		AnnDebug() << "Enter port number, 0 selects default: ";
		step = 2;
		break;
	case 2:
		// same here I've to get the input stuff ( str = console->getInput(); )
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
		// console->clearInput
		break;
	}
}












void MyLevel::runLogic()
{
	communicate(frameTime);
}