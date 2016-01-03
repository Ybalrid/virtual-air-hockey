#include "stdafx.h"
#include "myLevel.hpp"

AnnGameObject* test = nullptr;

MyLevel::MyLevel() : AnnAbstractLevel(),
	playerPaddleActor(nullptr)
{ 
}

void MyLevel::load()
{
	//For having a lighter syntax :
	auto engine(AnnEngine::Instance());
	
	//Load Sinbad:
	/*auto Sinbad (engine->createGameObject("Sinbad.mesh"));
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
	levelLighting.push_back(light);*/

	auto table(addGameObject("table.mesh"));
	table->setPos(0, -1.5,0);
	table->setScale(0.85,0.85,0.85);
	table->setUpPhysics();

	
	auto puck(addGameObject("puck.mesh"));
	puck->setPos(0, -1.1, .8);
	puck->setUpPhysics(1, convexShape);
	test = puck;
	


	auto paddle(addGameObject("paddle.mesh"));
	paddle->setPos(0, -1.1, 1.05);
	paddle->setScale(0.25,0.25,0.25);
	paddle->setUpPhysics(1, convexShape);
	playerPaddleActor = new PlayerPaddleAction(paddle, puck);
	

	auto Ground (engine->createGameObject("room.mesh"));
	Ground->setPos(0,-2,0);
	Ground->setUpPhysics();
	//Add it to the level list
	levelContent.push_back(Ground);


	engine->setAmbiantLight(Ogre::ColourValue(0.4,0.4,0.4));
	engine->getPlayer()->setPosition(AnnVect3(0,-1,2.1));

	//Add light source directly over the table	
	auto pointLight(addLight());
	pointLight->setPosition(AnnVect3(0,1,0));

	engine->resetPlayerPhysics();

	engine->getEventManager()->addListener(playerPaddleActor);
}

void MyLevel::unload()
{
	AnnAbstractLevel::unload();
	AnnEngine::Instance()->getEventManager()->removeListener(playerPaddleActor);
	delete playerPaddleActor;
}

void MyLevel::runLogic()
{
	
}