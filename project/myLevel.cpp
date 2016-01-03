#include "stdafx.h"
#include "myLevel.hpp"

AnnGameObject* test = nullptr;

MyLevel::MyLevel() : AnnAbstractLevel()
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

	/*
	auto puck(addGameObject("puck.mesh"));
	puck->setPos(0, -3.5, 0);
	puck->setUpPhysics(1, cylinderShape);
	//test = puck;
	*/


	auto paddle(addGameObject("paddle.mesh"));
	paddle->setPos(0, 5, 0);
	paddle->setUpPhysics(1, convexShape);
	test = paddle;
	

	auto Ground (engine->createGameObject("Ground.mesh"));
	Ground->setPos(0,-2,0);
	Ground->setUpPhysics();
	//Add it to the level list
	levelContent.push_back(Ground);


	engine->setAmbiantLight(Ogre::ColourValue::White);

}

void MyLevel::runLogic()
{
	if(test)
	AnnDebug() << test->pos();
}