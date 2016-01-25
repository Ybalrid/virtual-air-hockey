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
	


	engine->resetPlayerPhysics();

}

int MyLevel::initializeServer(int port = ANVPORT)
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

	AnnEngine::Instance()->getPlayer()->setPosition(AnnVect3(0,0,2.1));

	AnnDebug() << "---- Server ----";
	net.getLocalIP(localIP);
	ss << "Server IP: " << localIP;
	ss.str("");
	ss << "Port: " << port;
	AnnDebug() << ss.str();
	return netNS::NET_OK;
}

void MyLevel::runLogic()
{
}