#pragma once
#include "GUIWindow.h"
#include "IDManager.h" 
#include <SDL_events.h>
#include <unordered_map>
#include <vector>
namespace Pro{
	namespace GUI{
		class GUIContext
		{
		private:
			std::unordered_map<ID, GUIWindow> windows;
			// a private ID manager for the GUI context
			IDManager* id_manager;

			GUIEntity* focusedEntity;
			std::string context_name;
		public:
			GUIContext();
			~GUIContext();

			void update(SDL_Event event);

			void attachWindow(GUIWindow window, const std::string& windowName);
			void detachWindow(const std::string& windowName);
			  
			std::string getContextName();
			void setContextName(const std::string& name);

			std::string getName(ID _id);
			ID getID(const std::string& name);
			void removeID(const std::string& name);

			// LUA Functions

			static int lAttachWindow(lua_State*);
			static int lDetachWindow(lua_State*);
			static int lGetContextName(lua_State*);
			static int lSetContextName(lua_State*);

			//static int 
		};
	}
}
