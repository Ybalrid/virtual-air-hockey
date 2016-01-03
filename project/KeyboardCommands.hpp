#include <Annwvyn.h>

using namespace Annwvyn;

class KeyboardCommands : LISTENER
{
public:
	KeyboardCommands();
	void KeyEvent(AnnKeyEvent e);
};