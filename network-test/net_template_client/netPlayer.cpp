#include "netPlayer.h"

NetPlayer::NetPlayer(void)
{
}


//=============================================================================
// Sets all player data from PlayerStc sent from server to client
//=============================================================================
void NetPlayer::setNetData(PlayerStc ss)
{
	setNetPlayerPosition(ss.X, ss.Y, ss.Z);
    setActive((ss.flags & 0x01) == 0x01);
}

//=============================================================================
// Return current ship state in ShipStc
//=============================================================================
PlayerStc NetPlayer::getNetData()
{
    PlayerStc data;
    data.X = getX();
    data.Y = getY();
	data.Z = getZ();
    data.playerN = getPlayerN();
    //-----flags-----
    // bit0 active
   
    data.flags = 0;
    if(getActive())
        data.flags |= 0x01;
  
    return data;
}

float NetPlayer::getX() 
{
	return AnnEngine::Instance()->getPlayer()->getPosition().x;
}

float NetPlayer::getY()
{
	return AnnEngine::Instance()->getPlayer()->getPosition().y;
}

float NetPlayer::getZ()
{
	return AnnEngine::Instance()->getPlayer()->getPosition().z;
}

void NetPlayer::setNetPlayerPosition(float x, float y, float z)
{
	AnnEngine::Instance()->Instance()->getPlayer()->setPosition(AnnVect3(x, y, z));
}



NetPlayer::~NetPlayer(void)
{
}
