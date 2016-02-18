#include <Annwvyn.h>


#include "NetConfig.hpp"

using namespace Annwvyn;

class MenuInput : LISTENER
{
public:
	MenuInput();

	void MouseEvent(AnnMouseEvent e);
	void tick();

	int getX();
	int getY();


	bool getClicked();

private:
	int X, Y;
	bool lastButtonState, currentButtonState;
	bool clicked;
};

class StartupMenu : LEVEL
{
public:
	//Annwyvn level API:
	StartupMenu();
	void load();
	void unload();
	void runLogic();



private:
	AnnAbstractLevel* GameLevel;
	AnnGameObject* pointer;
	AnnGameObject * RedCube, * GreenCube;

	MenuInput* menuInputListener;
	int mouseConvert;

	NetConfig configuration;

	AnnGameObjectList clickable;

	void RayCastClick();
	bool isClickable(AnnGameObject* obj);
	void objectClicked(AnnGameObject* obj);
};