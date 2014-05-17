#include "GUIEntity.h"

namespace Pro{
	namespace GUI{
		 
		GUIEntity::GUIEntity(const std::string& name) : CGUID(name){
			lua_callback = "\0";
			lua_state = nullptr; 
		}

		GUIEntity::GUIEntity()
		{
			lua_callback = "\0";
			lua_state = nullptr;
		}


		GUIEntity::~GUIEntity()
		{
		}


		SDL_Point GUIEntity::getPosition(){
			return position;
		}
		void GUIEntity::setPosition(SDL_Point _position){
			position = _position;
		}

		SDL_Point GUIEntity::getDimensions(){
			return dimensions;
		}

		void GUIEntity::setDimensions(SDL_Point _d){
			dimensions = _d;
		}

		GUIEntity* GUIEntity::getParent(){
			return parent;
		}
		void GUIEntity::setParent(GUIEntity* _parent){
			parent = _parent;
		} 

		bool GUIEntity::enabled(){
			return entity_enabled;
		}
		void GUIEntity::enabled(bool _bool){
			entity_enabled = _bool;
		}

		bool GUIEntity::isClickWithin(int x, int y){
			if (position.x < x && position.x + dimensions.x > x)
				if (position.y < y && position.y + dimensions.y > y)
					return true;
			return false;
		}
		bool GUIEntity::isClickWithin(SDL_MouseButtonEvent _mouse){
			int x = _mouse.x;
			int y = _mouse.y;
			return isClickWithin(x, y);
		}
		  
		std::string* GUIEntity::getCallback(){
			return &lua_callback;
		}
		void GUIEntity::setCallback(lua_State* L, const std::string& func, unsigned char args, unsigned char returns){
			lua_state = L;
			lua_callback = func;
			lua_arguments = args;
			lua_return = returns;
		}
		void GUIEntity::callback(){
			if (lua_state == nullptr)
				return;
			lua_getglobal(lua_state, &lua_callback[0]);
			lua_call(lua_state, 0, 0);
			return;
		} 
		inline GUIEntity* getPointer(lua_State* L){
			return *(static_cast<GUIEntity**>(lua_touserdata(L, 1)));
		}

		int  GUIEntity::lBindCallback(lua_State* L){
			GUIEntity* p = getPointer(L);
			p->setCallback(L,
				lua_tostring(L, 2),
				static_cast<unsigned char>(lua_tonumber(L, 3)),
				static_cast<unsigned char>(lua_tonumber(L, 4)));
			return 0;
		}
		int  GUIEntity::lGetPosition(lua_State* L){
			GUIEntity* p = getPointer(L);
			SDL_Point r = p->getPosition();
			lua_pushnumber(L, r.x);
			lua_pushnumber(L, r.y);
			return 2;
		}
		int  GUIEntity::lSetPosition(lua_State* L){
			GUIEntity* p = getPointer(L);
			SDL_Point r;
			r.x = static_cast<int>(lua_tonumber(L, 2));
			r.y = static_cast<int>(lua_tonumber(L, 3));
			p->setPosition(r);
			return 0;
		} 
		int  GUIEntity::lGetDimensions(lua_State* L){
			GUIEntity* p = getPointer(L);
			SDL_Point r = p->getDimensions();
			lua_pushnumber(L, r.x);
			lua_pushnumber(L, r.y);
			return 2;
		}
		int  GUIEntity::lSetDimensions(lua_State* L){
			GUIEntity* p = getPointer(L);
			SDL_Point r;
			r.x = static_cast<int>(lua_tonumber(L, 2));
			r.y = static_cast<int>(lua_tonumber(L, 3));
			p->setDimensions(r);
			return 0;
		}
		int  GUIEntity::lGetParent(lua_State* L){
			GUIEntity* p = getPointer(L);
			GUIEntity** o = static_cast<GUIEntity**>(lua_newuserdata(L, sizeof(GUIEntity*)));
			*o = p->getParent();
			luaL_getmetatable(L, "gui_entity_metatable");
			lua_setmetatable(L, -2);
			p->getParent();
			return 1;
		}
		int  GUIEntity::lSetParent(lua_State* L){
			GUIEntity* p = getPointer(L);
			p->setParent(static_cast<GUIEntity*>(lua_touserdata(L, 2)));
			return 0;
		} 
		int  GUIEntity::lEnable(lua_State* L){
			GUIEntity* p = getPointer(L);
			p->enabled(true);
			return 0;
		}
		int  GUIEntity::lDisable(lua_State* L){
			GUIEntity* p = getPointer(L);
			p->enabled(false);
			return 0;
		}
		int  GUIEntity::lIsEnabled(lua_State* L){
			GUIEntity* p = getPointer(L);
			lua_pushboolean(L, p->enabled());
			return 1;
		}  
		int GUIEntity::lGetName(lua_State* L){
			GUIEntity* p = getPointer(L);
			lua_pushstring(L, &(*GUIDLookup::getName(p->getGUID()))[0] );
			return 1;
		}
	}
}