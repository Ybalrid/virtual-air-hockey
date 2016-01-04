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
	keyboradVelocity = AnnVect3::ZERO;
	inputVelocity = AnnVect3::ZERO;
}

void PlayerPaddleAction::KeyEvent(AnnKeyEvent e)
{
	if(e.isPressed() && e.getKey() == KeyCode::space)
	{
		puck->setPos(0, -1.1, .8);
		puck->setOrientation(AnnQuaternion::IDENTITY);
	}
	/*
	if(e.isPressed()) switch (e.getKey())
	{
	default: break;
	case KeyCode::up:
		keyboradVelocity.z = -1;
		break;
	case KeyCode::down:
		keyboradVelocity.z = 1;
		break;
	case KeyCode::left:
		keyboradVelocity.x = -1;
		break;
	case KeyCode::right:
		keyboradVelocity.x = 1;
		break;
	}

	if(e.isReleased()) switch (e.getKey())
	{
	default: break;
	case KeyCode::up:
		keyboradVelocity.z = 0;
		break;
	case KeyCode::down:
		keyboradVelocity.z = 0;
		break;
	case KeyCode::left:
		keyboradVelocity.x = 0;
		break;
	case KeyCode::right:
		keyboradVelocity.x = 0;
		break;
	}

	if(!keyboradVelocity.isZeroLength())
		keyboradVelocity.normalise();

		AnnVect3 currentVelocity(paddle->getBody()->getLinearVelocity());
		keyboradVelocity.y = currentVelocity.y;
		keyboradVelocity*=paddleSpeed;


	 inputVelocity=(keyboradVelocity);
	*/

}

//If a gamepad is present, this method will be called at each frame:
void PlayerPaddleAction::StickEvent(AnnStickEvent e)
{
	inputVelocity = AnnVect3::ZERO;
	AnnStickAxis horiz = e.getAxis(0);
	AnnStickAxis vert = e.getAxis(1);

	AnnDebug() << "Player axis : " << horiz.getAbsValue() << " & "<< vert.getAbsValue(); 
	

	//Calculate the normal velocity vector the object should have thanks to the 2 dimentional analog input
	 inputVelocity= AnnVect3(vert.getAbsValue(), 0, horiz.getAbsValue());
	 AnnDebug() << "Raw stick value : " << inputVelocity;


	inputVelocity.x = trim(inputVelocity.x, deadzone);
	inputVelocity.z = trim(inputVelocity.z, deadzone);
	
	if(!inputVelocity.isZeroLength())
		inputVelocity.normalise();

	inputVelocity *= paddleSpeed;
	//AnnDebug() << "input velocity is : " << inputVelocity;
	
	//If button 0 (Xbox A) is currently pressed
	if(e.isPressed(0))
	{
		//Reset position/orientation of the puck
		puck->setPos(0, -1.1, .8);
		puck->setOrientation(AnnQuaternion::IDENTITY);
	}

	
}
void PlayerPaddleAction::tick()
{
		
	if((paddle->pos().x < -0.70 && inputVelocity.x < 0) 
		|| (paddle->pos().x > 0.70 && inputVelocity.x > 0)) inputVelocity.x = 0;

	if((paddle->pos().z > 1.15 && inputVelocity.z > 0) 
		|| (paddle->pos().z <= 0.1) && inputVelocity.z < 0) inputVelocity.z = 0;

	//lock the orientation upright
	paddle->setOrientation(AnnQuaternion::IDENTITY);

	//Prevent the body to be "put to sleep" by the physics engine
	paddle->getBody()->activate();
	AnnVect3 currentVelocity(paddle->getBody()->getLinearVelocity());
	if(currentVelocity.y <=0);
		inputVelocity.y = currentVelocity.y;


	paddle->setLinearSpeed(inputVelocity);
	puck->setOrientation(AnnQuaternion::IDENTITY);

	AnnDebug() << inputVelocity;
}