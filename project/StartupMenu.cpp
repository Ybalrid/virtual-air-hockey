#include "stdafx.h"
#include "StartupMenu.hpp"


MenuInput::MenuInput() : constructListener()
{
	auto textInputer(AnnEngine::Instance()->getEventManager()->getTextInputer());
	textInputer->startListening();
	clicked = false;
	lastButtonState = false;
	currentButtonState = false;
}

void MenuInput::MouseEvent(AnnMouseEvent e)
{
	//AnnDebug() << "Mouse Rel X : " << 
	(X = e.getAxis(MouseAxisId::X).getRelValue());
	//AnnDebug() << "Mouse Rel Y : " << 
	(Y = e.getAxis(MouseAxisId::Y).getRelValue());
	currentButtonState = e.getButtonState(MouseButtonId::Left);
	if(lastButtonState && !currentButtonState) clicked = true;
	else clicked = false;
	lastButtonState = currentButtonState;
}
bool MenuInput::getClicked()
{
	return clicked;
}

int MenuInput::getX(){return X;}
int MenuInput::getY(){return Y;}

void MenuInput::tick()
{
	//auto textInputer(AnnEngine::Instance()->getEventManager()->getTextInputer());
	//AnnDebug() << "Text content : " << textInputer->getInput();
}

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

	RedCube = addGameObject("RedCube.mesh");
	RedCube->setPos(0,0,8.5);
	clickable.push_back(RedCube);

	
	GreenCube = addGameObject("GreenCube.mesh");
	GreenCube -> setPos(0.5,0,8.5);
	clickable.push_back(GreenCube);

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

	if(menuInputListener->getClicked())
	{
		//AnnDebug() << "detectedClick";
		RayCastClick();
	}
}

void StartupMenu::RayCastClick()
{
	AnnVect3 pointerPos, posePos, rayOrig, rayDirection;
	//AnnDebug() << "pointer position : " << 
	(pointerPos = pointer->getPosition());
	//AnnDebug() << "PointOfViewPosition : " << 
	(posePos = AnnEngine::Instance()->getPoseFromOOR().position);

	rayOrig = posePos;
	rayDirection = pointerPos - posePos;
	//AnnDebug() << "Ray Origin " << rayOrig;
	//AnnDebug() << "Ray Direction " << rayDirection;

	Ogre::Ray clickRay(rayOrig, rayDirection);
	Ogre::RaySceneQuery* clickQuerry = AnnEngine::Instance()->getSceneManager()->createRayQuery(clickRay);
	clickQuerry->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result(clickQuerry->execute());

	AnnGameObject* clicked(nullptr);

	//AnnDebug() << "Ray result size : " << result.size();
	for(auto it(result.begin()); it != result.end(); it++)
		if(it->movable && it->movable->getMovableType() == "Entity")
		{
			clicked = AnnEngine::Instance()->getFromNode(it->movable->getParentSceneNode());
			
			//AnnDebug() << "clicked :" << clicked;
			//AnnDebug() << "testCube :" << testCube;

			if(isClickable(clicked))
			{
				AnnDebug() << "You have clicked on something you can click!";
				objectClicked(clicked);
			}
		}


}

bool StartupMenu::isClickable(AnnGameObject* obj)
{
	if(!obj) return false;

	for(auto clickableObject : clickable)
		if(clickableObject == obj) return true;
	return false;
}

void StartupMenu::objectClicked(AnnGameObject* obj)
{
	for(auto iter : clickable)
		iter->node()->showBoundingBox(false);
	obj->node()->showBoundingBox(true);
}