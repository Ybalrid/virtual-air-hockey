#include "stdafx.h"
#include "KeyboardCommands.hpp"

using namespace Annwvyn;

KeyboardCommands::KeyboardCommands() : constructListener()
{
}

void KeyboardCommands::KeyEvent(AnnKeyEvent e)
{
	if(e.isPressed())
	{
		switch (e.getKey())
		{
		default:
			break;
		case KeyCode::f12:
			AnnEngine::Instance()->resetOculusOrientation();
			break;
		case KeyCode::grave:
			AnnEngine::Instance()->toogleOnScreenConsole();
			break;
		case KeyCode::tab:
			AnnEngine::Instance()->toogleOculusPerfHUD();
			break;
		case KeyCode::f1:
			OgreOculusRender::showRawView();
			break;
		case KeyCode::f2:
			OgreOculusRender::showMirrorView();
			break;
		case KeyCode::f3:
			OgreOculusRender::showMonscopicView();
			break;
		}
	}
}