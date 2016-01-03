#include "stdafx.h"
#include "PlayerPaddleAction.hpp"

using namespace Annwvyn;

PlayerPaddleAction::PlayerPaddleAction(AnnGameObject* playerPaddle) : constructListener(), 
	paddle(playerPaddle),
	paddleSpeed(3.f),
	deadzone(0.20f)
{
	AnnDebug() << "PLAYER PADDLE ACTION CREATED";
}

void PlayerPaddleAction::KeyEvent(AnnKeyEvent e)
{
}

//If a gamepad is present, this method will be called at each frame:
void PlayerPaddleAction::StickEvent(AnnStickEvent e)
{
	AnnStickAxis horiz = e.getAxis(0);
	AnnStickAxis vert = e.getAxis(1);

	//AnnDebug() << "Player axis : " << horiz.getAbsValue() << " & "<< vert.getAbsValue(); 
	
	AnnVect3 currentVelocity(paddle->getBody()->getLinearVelocity());

	//Calculate the normal velocity vector the object should have thanks to the 2 dimentional analog input
	AnnVect3 inputVelocity(horiz.getAbsValue(), 0, vert.getAbsValue());
	
	if((paddle->pos().x < -0.70 && inputVelocity.x < 0) 
		|| (paddle->pos().x > 0.70 && inputVelocity.x > 0)) inputVelocity.x = 0;

	if((paddle->pos().z > 1.15 && inputVelocity.z > 0) 
		|| (paddle->pos().z <= 0.1) && inputVelocity.z < 0) inputVelocity.z = 0;

	inputVelocity.x = trim(inputVelocity.x, deadzone);
	inputVelocity.y = trim(inputVelocity.y, deadzone);
	
	if(!inputVelocity.isZeroLength())
		inputVelocity.normalise();

	inputVelocity *= paddleSpeed;
	inputVelocity.y = currentVelocity.y;
	//AnnDebug() << "input velocity is : " << inputVelocity;

	paddle->setLinearSpeed(inputVelocity);

	//lock the orientation upright
	paddle->setOrientation(AnnQuaternion::IDENTITY);

	//Prevent the body to be "put to sleep" by the physics engine
	paddle->getBody()->activate();
	AnnDebug() << paddle->pos();
}