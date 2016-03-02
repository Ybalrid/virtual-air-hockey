#include "stdafx.h"
#include "myLevel.hpp"
#include "net.h"

#include "NetworkClient.hpp"

AnnGameObject * localSinbad = nullptr;
AnnGameObject * distantSinbad = nullptr;

MyLevel::MyLevel() : AnnAbstractLevel()
{
}

void MyLevel::load()
{
	//For having a lighter syntax :
	auto engine(AnnEngine::Instance());
	
	//Load Sinbad:
	auto Sinbad (engine->createGameObject("Sinbad.mesh"));
	Sinbad->setScale(0.25,0.25,0.25);
	Sinbad->setUpPhysics(100, phyShapeType::boxShape);
	//Add it to the level list
	levelContent.push_back(Sinbad);
	localSinbad  = Sinbad;
	Sinbad = nullptr;


	Sinbad = (engine->createGameObject("Sinbad.mesh"));
	Sinbad->setScale(0.25,0.25,0.25);
	Sinbad->setPosition(5,0,0);
	Sinbad->setUpPhysics(100, phyShapeType::boxShape);
	//Add it to the level list
	levelContent.push_back(Sinbad);
	distantSinbad  = Sinbad;


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
	/*
	toClientData.gameState = 0;
	initializeServer(port);
	roundOver = true;*/

}


void MyLevel::runLogic()
{
	//Get the networking singleton : 
	auto network = NetworkWorker::getSingleton();
	if(!network) return;
	if(localSinbad)
	{
		localSinbad->setOrientation(AnnQuaternion::IDENTITY);
		AnnDebug() << "current position of sinbad is : " << localSinbad->getPosition();
		network->setLocalPositon(localSinbad->getPosition());
	}

	if(distantSinbad)
	{
		distantSinbad->setOrientation(AnnQuaternion::IDENTITY);
		distantSinbad->setPosition(network->getDistantPosition());
		AnnDebug() << "current position of sinbad is : " << network->getDistantPosition();
	}
}
