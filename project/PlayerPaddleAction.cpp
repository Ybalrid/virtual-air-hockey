#include "stdafx.h"
#include "PlayerPaddleAction.hpp"

using namespace Annwvyn;

PlayerPaddleAction::PlayerPaddleAction(AnnGameObject* playerPaddle, AnnGameObject* tablePuck) : constructListener(), 
	paddle(playerPaddle),
	puck(tablePuck),
	paddleSpeed(3.f),
	deadzone(0.20f)
{
	AnnDebug() << "PLAYER PADDLE ACTION CREATED";
	keyboardVelocity = AnnVect3::ZERO;
	inputVelocity = AnnVect3::ZERO;
	stickVelocity = AnnVect3::ZERO;
}

void PlayerPaddleAction::KeyEvent(AnnKeyEvent e)
{
	if(e.isPressed() && e.getKey() == KeyCode::space)
		resetPuck();

	if(e.isPressed()) switch (e.getKey())
	{
	default: break;
	case KeyCode::up:
		keyboardVelocity.z = -1;
		break;
	case KeyCode::down:
		keyboardVelocity.z = 1;
		break;
	case KeyCode::left:
		keyboardVelocity.x = -1;
		break;
	case KeyCode::right:
		keyboardVelocity.x = 1;
		break;
	}

	if(e.isReleased()) switch (e.getKey())
	{
	default: break;
	case KeyCode::up:
		keyboardVelocity.z = 0;
		break;
	case KeyCode::down:
		keyboardVelocity.z = 0;
		break;
	case KeyCode::left:
		keyboardVelocity.x = 0;
		break;
	case KeyCode::right:
		keyboardVelocity.x = 0;
		break;
	}
	

	if(!keyboardVelocity.isZeroLength())
		keyboardVelocity.normalise();
	keyboardVelocity *= paddleSpeed;
}

//If a gamepad is present, this method will be called at each frame:
void PlayerPaddleAction::StickEvent(AnnStickEvent e)
{
	stickVelocity = AnnVect3::ZERO;
	AnnStickAxis horiz = e.getAxis(0);
	AnnStickAxis vert = e.getAxis(1);	

	//Calculate the normal velocity vector the object should have thanks to the 2 dimentional analog input
	stickVelocity= AnnVect3(vert.getAbsValue(), 0, horiz.getAbsValue());

	stickVelocity.x = trim(stickVelocity.x, deadzone);
	stickVelocity.z = trim(stickVelocity.z, deadzone);
	
	if(!stickVelocity.isZeroLength())
		stickVelocity.normalise();

	stickVelocity *= paddleSpeed;
	
	//If button 0 (Xbox A) is currently pressed
	if(e.isPressed(0)) resetPuck();//Reset position/orientation of the puck	
}
void PlayerPaddleAction::tick()
{
	if(stickVelocity.isZeroLength() && !keyboardVelocity.isZeroLength())
		inputVelocity = keyboardVelocity;
	else
		inputVelocity = stickVelocity;

	//Prevent the paddle to be out of the table
	if((paddle->pos().x < -0.70 && inputVelocity.x < 0) 
		|| (paddle->pos().x > 0.70 && inputVelocity.x > 0)) inputVelocity.x = 0;

	if((paddle->pos().z > 1.15 && inputVelocity.z > 0) 
		|| (paddle->pos().z <= 0.1) && inputVelocity.z < 0) inputVelocity.z = 0;

	//lock the orientation upright
	paddle->setOrientation(AnnQuaternion::IDENTITY);

	//Prevent the body to be "put to sleep" by the physics engine
	paddle->getBody()->activate();
	AnnVect3 currentVelocity(paddle->getBody()->getLinearVelocity());
	if(currentVelocity.y <=0)
		inputVelocity.y = currentVelocity.y;

	paddle->setLinearSpeed(inputVelocity);
	puck->setOrientation(AnnQuaternion::IDENTITY);
}


void PlayerPaddleAction::resetPuck()
{
	puck->setPos(0, -1.1, .8);
	puck->setOrientation(AnnQuaternion::IDENTITY);
	puck->setLinearSpeed(AnnVect3::ZERO);
}