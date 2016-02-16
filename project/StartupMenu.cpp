#include "stdafx.h"
#include "StartupMenu.hpp"


MenuInput::MenuInput() : constructListener()
{

}

void MenuInput::MouseEvent(AnnMouseEvent e)
{
	AnnDebug() << "Mouse Rel X : " << (X = e.getAxis(MouseAxisId::X).getRelValue());
	AnnDebug() << "Mouse Rel Y : " << (Y = e.getAxis(MouseAxisId::Y).getRelValue());
}

int MenuInput::getX(){return X;}
int MenuInput::getY(){return Y;}

StartupMenu::StartupMenu() : constructLevel(),
	GameLevel(nullptr),
	menuInputListener(nullptr),
	mouseConvert(100)
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

	AnnEngine::Instance()->getEventManager()->addListener(menuInputListener = new MenuInput);
}

void StartupMenu::unload()
{
	AnnAbstractLevel::unload();
	AnnEngine::Instance()->getEventManager()->removeListener(menuInputListener);
	AnnEngine::Instance()->getPlayer()->ignorePhysics = false;
	if(GameLevel) AnnEngine::Instance()->getLevelManager()->jump(GameLevel);

}

void StartupMenu::runLogic()
{
	float x((float)menuInputListener->getX()/(float)mouseConvert);
	float y(-(float)menuInputListener->getY()/(float)mouseConvert);

	pointer->setPos(pointer->pos() + AnnVect3(x,y,0));
}