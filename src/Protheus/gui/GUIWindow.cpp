#include "GUIWindow.h"

using namespace Pro;
using namespace GUI;

GUIWindow::GUIWindow(const std::string& name) : GUIContainer(name){ 
	menu_bar = nullptr; 
	hasMenuBar = false; 
}

GUIWindow::GUIWindow() : GUIContainer(){ 
	menu_bar = nullptr; 
	hasMenuBar = false;
}

void GUIWindow::attachMenuBar(GUIMenuBar * component)
{
	menu_bar = component; 
	hasMenuBar = true;
}
