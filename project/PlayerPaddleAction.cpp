#include "stdafx.h"
#include "PlayerPaddleAction.hpp"

#include "FalconController.hpp"
#include "NetworkServer.hpp"

using namespace Annwvyn;

PlayerPaddleAction::PlayerPaddleAction(AnnGameObject* playerPaddle, AnnGameObject* tablePuck) : constructListener(), 
	paddle(playerPaddle),
	puck(tablePuck),
	paddleSpeed(3.f),
	deadzone(0.20f),
	falconFactor(15),
	state(CLASSIC)
{
	AnnDebug() << "PLAYER PADDLE ACTION CREATED";
	keyboardVelocity = AnnVect3::ZERO;
	inputVelocity = AnnVect3::ZERO;
	stickVelocity = AnnVect3::ZERO;

	paddle->testCollisionWith(puck);

	FalconController::getSingleton()->setCallback(this);
}

void PlayerPaddleAction::KeyEvent(AnnKeyEvent e)
{
	if(e.isPressed() && e.getKey() == KeyCode::space)
		resetPuck();
	if(e.isPressed() && e.getKey() == KeyCode::q)
	{
		AnnDebug() << "Classic controls";
		state = CLASSIC;
	}
	if(e.isPressed() && e.getKey() == KeyCode::a)
	{
		AnnDebug() << "Falcon controls";
		state = FALCON;
	}


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

	AnnVect3 currentVelocity(paddle->getBody()->getLinearVelocity());

	switch(state)
	{
	case CLASSIC:
		if(stickVelocity.isZeroLength() && !keyboardVelocity.isZeroLength())
			inputVelocity = keyboardVelocity;
		else
			inputVelocity = stickVelocity;

		//Addapt controls for other side of the table
		if(NetworkWorker::getSingleton()->getType() == CLIENT)
		{
			inputVelocity.z *= -1;
			inputVelocity.x *= -1;
		}

		//Prevent the paddle to be out of the table
		if((paddle->pos().x < -0.70 && inputVelocity.x < 0) 
			|| (paddle->pos().x > 0.70 && inputVelocity.x > 0)) inputVelocity.x = 0;

		if(NetworkWorker::getSingleton()->getType() == CLIENT)
		{
			if((paddle->pos().z < -1.15 && inputVelocity.z < 0) 
				|| (paddle->pos().z >= -0.1) && inputVelocity.z > 0) inputVelocity.z = 0;
			//AnnDebug() << paddle->getPosition();
		}
		else
		{
			if((paddle->pos().z > 1.15 && inputVelocity.z > 0) 
				|| (paddle->pos().z <= 0.1) && inputVelocity.z < 0) inputVelocity.z = 0;
		}
		//lock the orientation upright
		paddle->setOrientation(AnnQuaternion::IDENTITY);

		//Prevent the body to be "put to sleep" by the physics engine
		paddle->getBody()->activate();
		if(currentVelocity.y <=0)
			inputVelocity.y = currentVelocity.y;



		paddle->setLinearSpeed(inputVelocity);
		break;

	case FALCON:

		//AnnDebug() << "controll with falcon";	
		FalconController* falcon (FalconController::getSingleton());
		if(falcon->getButtonState(FalconController::FalconGripButton::PRINCIPAL))
			resetPuck();
		//AnnDebug() << falcon->getPosition();
		AnnVect3 start(0, -1.1, 1.05);
		AnnVect3 falconPosition(falcon->getPosition());

		if(NetworkWorker::getSingleton()->getType() == CLIENT) 
		{
			start.z*=-1;
			falconPosition.x*=-1;
			falconPosition.z*=-1;
		}


		AnnVect3 position = (falconFactor*falconPosition + start);
		if(position.y < -1.084) position.y = -1.084;
		paddle->setPos(position);
		paddle->setOrientation(AnnQuaternion::IDENTITY);
		paddle->getBody()->setGravity(AnnVect3(0,0,0).getBtVector());

		if(paddle->collideWith(puck))
		{
			float instantForce = 3;
			AnnVect3 direction = paddle->pos() - puck->pos();
			direction.y = 0; //only planar
			direction.normalise();
			AnnVect3 returnForce = direction * instantForce;
				if(NetworkWorker::getSingleton()->getType() == SERVER) 
					returnForce *= -1;
			puck->getBody()->applyCentralImpulse(returnForce.getBtVector());
			puck->playSound("media/contact.wav");
		}

		break;
	}

	puck->setOrientation(AnnQuaternion::IDENTITY);

}

//haptics
void PlayerPaddleAction::callback(FalconController* controller)
{
	//AnnDebug() << "callback" << endl;
	AnnVect3 position = (falconFactor*controller->getPosition() + AnnVect3(0, -1.1, 1.05));

	FalconController::FalconVect3 force = {0, 0, 0};

	if(position.y < -1.084)
	{
		force[0] = 0;
		force[1] = (-1.084 - position.y) * 50;
		force[2] = 0;
	}

	if(paddle->collideWith(puck))
	{
		float instantForce = 5;

		//Calculate a translation vector between the paddle and puck
		AnnVect3 direction = paddle->pos() - puck->pos();
		direction.y = 0; //only planar
		//Normalize it
		if(!direction.isZeroLength())
			direction.normalise();
		AnnVect3 returnForce = direction * instantForce;

		force[0] += returnForce.x;
		force[1] += returnForce.y;
		force[2] += returnForce.z;
	}
	controller->setForce(force);
	AnnDebug() << AnnVect3(force[0], force[1], force[2]);
}


void PlayerPaddleAction::resetPuck()
{
	puck->setPos(0, -1.1, .8);
	puck->setOrientation(AnnQuaternion::IDENTITY);
	puck->setLinearSpeed(AnnVect3::ZERO);
}