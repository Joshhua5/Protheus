#include "GUIContext.h"

using namespace Pro;
GUIContext::GUIContext()
{
}


GUIContext::~GUIContext()
{
}

GUIEvent* GUIContext::update(SDL_Event event){ 
	for each(auto windows in windows){ 
		if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
			// Pass the event is the button click was within the window
			if (windows.second.isClickWithin(event.button.x, event.button.y))
				return windows.second.update(event);
	}
	return nullptr;
}
 
void GUIContext::attachWindow(GUIWindow window, const std::string& windowName){
	windows.insert({ windowName, window });
}
void GUIContext::detachWindow(const std::string& windowName){
	windows.erase(windowName);
}

GUIEvent GUIContext::getEvent(){
	GUIEvent out = gui_events.top();
	gui_events.pop();
	return out;
}
GUIEvent GUIContext::peekEvent(){
	return gui_events.top();
}
bool GUIContext::hasEvent(){
	return !gui_events.empty();
}

std::string GUIContext::getContextName(){
	return context_name;
}
void GUIContext::setContextName(const std::string& name){
	context_name = name;
}

ID GUIContext::getID(const std::string& name){
	return id_manager->getIDFromName(name);
}

void GUIContext::removeID(const std::string& name){
	id_manager->releaseID(name);
}
std::string GUIContext::getName(ID _id){
	return id_manager->getName(_id);
}