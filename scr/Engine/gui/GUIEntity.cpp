

#include "GUIEntity.h"

namespace Pro{
	namespace GUI{
		 
		GUIEntity::GUIEntity(const std::string& name) :
			CGUID(name),
			LuaCallback(){}

		GUIEntity::GUIEntity() :
			CGUID(),
			LuaCallback(){} 

		GUIEntity::~GUIEntity() { }
		 
		GUIEntity* GUIEntity::getParent(){
			return parent;
		}
		void GUIEntity::setParent(GUIEntity* _parent){
			parent = _parent;
		} 
		 
		bool GUIEntity::isClickWithin(Math::Vector2& v){
			return Math::Vector4(position, dimensions).contains(v); 
		}

		inline GUIEntity* getPointer(lua_State* L){
			return *(static_cast<GUIEntity**>(lua_touserdata(L, 1)));
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
	}
}
