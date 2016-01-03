#include <Annwvyn.h>
using namespace Annwvyn;

class PlayerPaddleAction : LISTENER
{
public:
	PlayerPaddleAction(AnnGameObject* playerPaddle);
	void StickEvent(AnnStickEvent e);
	void KeyEvent(AnnKeyEvent e);

private:
	AnnGameObject* paddle;
	float paddleSpeed;
	float deadzone;
};