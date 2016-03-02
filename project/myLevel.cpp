#include "stdafx.h"
#include "myLevel.hpp"

#include "NetworkServer.hpp"


MyLevel::MyLevel() : AnnAbstractLevel(),
	playerPaddleActor(nullptr)
{ 
}



void MyLevel::load()
{
	//For having a lighter syntax :
	auto engine(AnnEngine::Instance());
	
	

	auto table(addGameObject("table.mesh"));
	table->setPos(0, -1.5,0);
	table->setScale(0.85,0.85,0.85);
	table->setUpPhysics();
	table->getBody()->setFriction(0);

	
	auto puck(addGameObject("puck.mesh"));
	puck->setPos(0, -1.1, .8);
	puck->setUpPhysics(1, phyShapeType::sphereShape);
	puck->getBody()->setFriction(2);
	





	auto Ground (engine->createGameObject("room.mesh"));
	Ground->setPos(0,-2,0);
	Ground->setUpPhysics();
	//Add it to the level list
	levelContent.push_back(Ground);


	//engine->setAmbiantLight(Ogre::ColourValue(0.4,0.4,0.4));

	//Add light source directly over the table	
	auto pointLight(addLight());
	pointLight->setPosition(AnnVect3(0,1,0));


		paddle=(addGameObject("paddle.mesh"));
		paddle->setScale(0.25,0.25,0.25);

	auto network(NetworkWorker::getSingleton());
	if(network->getType() == SERVER)
	{
			engine->getPlayer()->setPosition(AnnVect3(0,-1,1.7));
			paddle->setPos(0, -1.1, 1.05);

	}
	else if(network->getType() == CLIENT)
	{
		auto player(engine->getPlayer());
		player->setOrientation(Ogre::Euler(Ogre::Degree(180)));
		player->setPosition(AnnVect3(0,-1,-1.7));
		paddle->setPos(0, -1.1, -1.05);
	}

	engine->resetPlayerPhysics();
	paddle->setUpPhysics(10, convexShape);
	playerPaddleActor = new PlayerPaddleAction(paddle, puck);
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
	paddle->setOrientation(AnnQuaternion::IDENTITY);
	btRigidBody* body;
	if(body = paddle->getBody()) body->activate();
	

}