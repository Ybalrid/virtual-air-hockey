#include "stdafx.h"
#include "StartupMenu.hpp"

StartupMenu::StartupMenu() : constructLevel(),
	GameLevel(nullptr)
{
}

void StartupMenu::load()
{
	AnnEngine::Instance()->getPlayer()->ignorePhysics = true;
	auto directionalLight = addLight();
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDirection(AnnVect3(-1,-1,-0.5));
	pointer = addGameObject("pointer.mesh");
	pointer->setPos(0,0,9);
}

void StartupMenu::unload()
{
	AnnAbstractLevel::unload();
	AnnEngine::Instance()->getPlayer()->ignorePhysics = false;
	if(GameLevel) AnnEngine::Instance()->getLevelManager()->jump(GameLevel);

}

void StartupMenu::runLogic()
{
}