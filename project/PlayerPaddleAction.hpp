#include <Annwvyn.h>
#include "FalconController.hpp"
using namespace Annwvyn;

class PlayerPaddleAction : LISTENER, public HapticCallback
{
public:
	enum controllState{CLASSIC, FALCON};
	PlayerPaddleAction(AnnGameObject* playerPaddle, AnnGameObject* playerPuck);
	void StickEvent(AnnStickEvent e);
	void KeyEvent(AnnKeyEvent e);
	void tick();
	void callback(FalconController* controller);


	void resetPuck();

	void setState(controllState newState);

private:
	AnnGameObject* paddle, *puck;
	float paddleSpeed;
	float deadzone;

	float falconFactor;

	AnnVect3 keyboardVelocity, stickVelocity, inputVelocity;
	controllState state;

};