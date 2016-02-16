#include <Annwvyn.h>

using namespace Annwvyn;

class MenuInput : LISTENER
{
public:
	MenuInput();

	void MouseEvent(AnnMouseEvent e);

	int getX();
	int getY();

private:
	int X, Y;
};

class StartupMenu : LEVEL
{
public:
	StartupMenu();
	void load();
	void unload();
	void runLogic();

private:
	AnnAbstractLevel* GameLevel;
	AnnGameObject* pointer;
	MenuInput* menuInputListener;
	int mouseConvert;
};