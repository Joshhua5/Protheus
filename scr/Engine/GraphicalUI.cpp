#include "GraphicalUI.h"

using namespace Pro; 

GraphicalUI::GraphicalUI()
{
}


GraphicalUI::~GraphicalUI()
{
}


std::stack<GUI::GUIEvent> GraphicalUI::update(std::vector<SDL_Event*> events){
	std::stack<GUI::GUIEvent> returned_events;
	GUI::GUIEvent* feedback;
	for each(auto event in events){
		for each(auto windows in window_contexts){
			feedback = windows->update(*event);
			if (feedback != nullptr){
				returned_events.push(*feedback);
				delete feedback;
			} 
		}
	}
	return returned_events;
}
 


void GraphicalUI::enableEditor(){

}

void GraphicalUI::initEditor(){
	static bool init = false;
	if (init == true)
		return;

	GUI::GUIContext* editor_context = new GUI::GUIContext();
	GUI::GUIWindow properties_window;

	SDL_Rect properties_rect;
	properties_rect.x = 20;
	properties_rect.y = 20;
	properties_rect.w = 200;
	properties_rect.h = 200; 

	properties_window.setPosition(properties_rect); 
	properties_window.setID(editor_context->getID("Properties"));
	 

	editor_context->attachWindow(properties_window,
		editor_context->getName(properties_window.getID()));
	
	editor_context->setContextName("Editor");

	inactive_window_contexts.push_back(editor_context);
	
	init = true;
}