#include "netPlayer.h"

NetPlayer::NetPlayer(void)
{
}


//=============================================================================
// Sets all player data from PlayerStc sent from server to client
//=============================================================================
void NetPlayer::setNetData(PlayerStc ss)
{
	auto engine(AnnEngine::Instance());
    setX(ss.X);
    setY(ss.Y);
	setZ(ss.Z);
    
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
    data.playerN = getPlayerN();
    //-----flags-----
    // bit0 active
    // bit1 engineOn
    // bit2 shieldOn
    data.flags = 0;
    if(getActive())
        data.flags |= 0x01;
  
    return data;
}

NetPlayer::~NetPlayer(void)
{
}
