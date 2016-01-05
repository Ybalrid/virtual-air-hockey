#include <Annwvyn.h>
using namespace Annwvyn;

class PlayerPaddleAction : LISTENER
{
public:
	PlayerPaddleAction(AnnGameObject* playerPaddle, AnnGameObject* playerPuck);
	void StickEvent(AnnStickEvent e);
	void KeyEvent(AnnKeyEvent e);
	void tick();


	void resetPuck();

private:
	AnnGameObject* paddle, *puck;
	float paddleSpeed;
	float deadzone;

	AnnVect3 keyboradVelocity, inputVelocity;
};