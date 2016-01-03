#ifndef MY_LEVEL
#define MY_LEVEL

#include <Annwvyn.h>
#include "PlayerPaddleAction.hpp"

using namespace Annwvyn;

//Each level you can create hinerits
//from AnnAbstractLevel
class MyLevel : public AnnAbstractLevel
{
public:
	MyLevel();
	void load();
	void unload();
	void runLogic();
private:
	PlayerPaddleAction* playerPaddleActor;
};

#endif //MY_LEVEL