#include "netPlayer.h"

NetPlayer::NetPlayer(void)
{
}

PlayerStc NetPlayer::getNetData() 
{
	auto engine(AnnEngine::Instance());
	PlayerStc data;
	data.X = (float)engine->getPlayer()->getPosition();
}

NetPlayer::~NetPlayer(void)
{
}
