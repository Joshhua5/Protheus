#include "GraphicalUI.h"

using namespace Pro;

GraphicalUI::GraphicalUI()
{
}

GraphicalUI::~GraphicalUI()
{
}

void GraphicalUI::update(std::vector<SDL_Event*> events){
	for each(auto event in events)
		for each(auto windows in window_contexts)
			windows->update(*event);
}

void GraphicalUI::enableEditor(){
}

void GraphicalUI::initEditor(){
	//static bool init = false;
	//if (init == true)
	//	return;

	//GUI::GUIContext* editor_context = new GUI::GUIContext();
	//GUI::GUIWindow properties_window;

	//SDL_Point properties_pos;
	//properties_pos.x = 20;
	//properties_pos.y = 20;
	//SDL_Point properties_dim;
	//properties_dim.x = 200;
	//properties_dim.y = 200;

	//properties_window.setPosition(properties_pos);
	//properties_window.setDimensions(properties_dim);
	//properties_window.setID(editor_context->getID("Properties"));
	//

	//editor_context->attachWindow(properties_window,
	//	editor_context->getName(properties_window.getID()));
	//
	//editor_context->setContextName("Editor");

	//inactive_window_contexts.push_back(editor_context);
	//
	//init = true;
}