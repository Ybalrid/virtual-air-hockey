#include "stdafx.h"
#include "StartupMenu.hpp"

StartupMenu::StartupMenu() : constructLevel()
{
}

void StartupMenu::load()
{
	AnnEngine::Instance()->getPlayer()->ignorePhysics = true;
}

void StartupMenu::unload()
{
	AnnAbstractLevel::unload();
	AnnEngine::Instance()->getPlayer()->ignorePhysics = false;

}

void StartupMenu::runLogic()
{
}