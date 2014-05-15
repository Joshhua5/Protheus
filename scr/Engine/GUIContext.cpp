#include "GUIContext.h"

using namespace Pro;
using namespace GUI;

GUIContext::GUIContext()
{
}


GUIContext::~GUIContext()
{
}

void GUIContext::update(SDL_Event event){ 
	for each(auto windows in windows)
		if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
			if (windows.second.isClickWithin(event.button.x, event.button.y))
				windows.second.update(event);
}
 
void GUIContext::attachWindow(GUIWindow window, const std::string& windowName){
	windows.insert({ windowName, window });
}
void GUIContext::detachWindow(const std::string& windowName){
	windows.erase(windowName);
}

std::string GUIContext::getContextName(){
	return context_name;
}
void GUIContext::setContextName(const std::string& name){
	context_name = name;
}

Pro::ID GUIContext::getID(const std::string& name){
	return id_manager->getIDFromName(name);
}

void GUIContext::removeID(const std::string& name){
	id_manager->releaseID(name);
}
std::string GUIContext::getName(Pro::ID _id){
	return id_manager->getName(_id);
}