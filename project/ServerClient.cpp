#include "stdafx.h"
#include "ServerClient.h"

ServerClient::ServerClient(void)
{
}

PlayerStc ServerClient::getNetData() 
{
	auto engine(AnnEngine::Instance());
	PlayerStc data;
	data.X = engine->getPlayer()->getPosition().x;
	data.Y = engine->getPlayer()->getPosition().y;
	data.Z = engine->getPlayer()->getPosition().z;
	//data.playerN = getPlayerN();
	//-----flags-----
	// Bit0 active
	data.flags = 0;
	if(getActive())
		data.flags |= 0x01;
	return data;
}

ServerClient::~ServerClient(void)
{
}
