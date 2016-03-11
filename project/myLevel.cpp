#include "stdafx.h"
#include "myLevel.hpp"

#include "NetworkServer.hpp"
#include "NetworkClient.hpp"


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

	
	 puck =(addGameObject("puck.mesh"));
	puck->setPos(0, -1.1, .8);
	puck->setUpPhysics(1, phyShapeType::sphereShape);
	puck->getBody()->setFriction(0);
	puck->getBody()->setRollingFriction(0);
	puck->getBody()->setDamping(0,0);
	puck->getBody()->setRestitution(0);
	





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
		opponantPaddle=(addGameObject("paddle.mesh"));
		opponantPaddle->setScale(0.25,0.25,0.25);

	auto network(NetworkWorker::getSingleton());
	if(network->getType() == SERVER)
	{
			engine->getPlayer()->setPosition(AnnVect3(0,-1,1.7));
			paddle->setPos(0, -1.1, 1.05);
			opponantPaddle->setPos(0, -1.1, -1.05);

	}
	else if(network->getType() == CLIENT)
	{
		auto player(engine->getPlayer());
		player->setOrientation(Ogre::Euler(Ogre::Degree(180)));
		player->setPosition(AnnVect3(0,-1,-1.7));
		paddle->setPos(0, -1.1, -1.05);
		opponantPaddle->setPos(0, -1.1, 1.05);

	}

	engine->resetPlayerPhysics();
	paddle->setUpPhysics(10, convexShape);
	opponantPaddle->setUpPhysics(10, convexShape);

	playerPaddleActor = new PlayerPaddleAction(paddle, opponantPaddle, puck);
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
	opponantPaddle->setOrientation(AnnQuaternion::IDENTITY);
	btRigidBody* body;
	if(body = paddle->getBody()) body->activate();
	if(body = opponantPaddle->getBody()) body->activate();

	auto network(NetworkWorker::getSingleton());
	if(!network) return;
	network->setLocalPositon(paddle->getPosition());
	opponantPaddle->setPosition(network->getDistantPosition());
	if(network->getType() == SERVER)
		static_cast<NetworkServer*>(network)->setRefPuckPosition(puck->getPosition());
	else if(network->getType() == CLIENT)
		puck->setPosition(static_cast<NetworkClient*>(network)->getRefPuckPosition());


	if(network->getType() == SERVER)
	{
		AnnDebug() << puck->getPosition();
		if(puck->getPosition().z < -1.355)
			playerPaddleActor->resetPuck();
		if(puck->getPosition().z > 1.355)
			playerPaddleActor->resetPuck();			
	}
}