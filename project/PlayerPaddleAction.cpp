#include "stdafx.h"
#include "PlayerPaddleAction.hpp"

using namespace Annwvyn;

PlayerPaddleAction::PlayerPaddleAction(AnnGameObject* playerPaddle) : constructListener(), 
	paddle(playerPaddle)
{
	AnnDebug() << "PLAYER PADDLE ACTION CREATED";
}

void PlayerPaddleAction::KeyEvent(AnnKeyEvent e)
{
}

void PlayerPaddleAction::StickEvent(AnnStickEvent e)
{
	AnnStickAxis horiz = e.getAxis(0);
	AnnStickAxis vert = e.getAxis(1);

	AnnDebug() << "Player axis : " << horiz.getAbsValue() << " & "<< vert.getAbsValue(); 
}