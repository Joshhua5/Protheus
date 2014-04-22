#include "GUI.h"

using namespace Pro;

GUI::GUI()
{
}


GUI::~GUI()
{
}


std::stack<GUIEvent> GUI::update(std::vector<SDL_Event*> events){
	std::stack<GUIEvent> returned_events;
	GUIEvent* feedback;
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
 


void GUI::enableEditor(){

}

void GUI::initEditor(){
	static bool init = false;
	if (init == true)
		return;

	GUIContext* editor_context = new GUIContext();
	GUIWindow properties_window;

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