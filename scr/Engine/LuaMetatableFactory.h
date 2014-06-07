/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Creates all the global's which are accessible through Lua.
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once
#include "lua\lua.hpp" 

namespace Pro{
	namespace Lua{
		class LuaMetatableFactory{
			//  Purpose of this function is to reduce the dupilicated function defitions between metatables	 
			template<typename T> vector<luaL_Reg> createMetatable(){
				vector<luaL_Reg> out;
				T object;
				if (dynamic_cast<Component::ActiveState*>(&object)){
					out.push_back({ "activate", &T::lActivate });
					out.push_back({ "deactivate", &T::lDeactivate });
					out.push_back({ "isActive", &T::lIsActive });
				}
				if (dynamic_cast<Component::Area*>(&object)){
					out.push_back({ "getDimensions", &T::lGetDimensions });
					out.push_back({ "setDimensions", &T::lSetDimensions });
				}
				if (dynamic_cast<Component::CGUID*>(&object)){
					out.push_back({ "getGUID", &T::lGetGUID });
				}
				if (dynamic_cast<Component::CScriptable*>(&object)){
					out.push_back({ "update", &T::lUpdate });
					out.push_back({ "attachFunction", &T::lAttachFunction });
				}
				if (dynamic_cast<Component::LuaCallback*>(&object)){
					out.push_back({ "bindCallback", &T::lBindCallback });
				}
				if (dynamic_cast<Component::Name*>(&object)){
					// No Lua Functions
				}
				if (dynamic_cast<Component::Position*>(&object)){
					out.push_back({ "getPosition", &T::lGetPosition });
					out.push_back({ "setPosition", &T::lSetPosition });
				}
				if (dynamic_cast<GameObject::Entity*>(&object)){
					// No Lua Functions
				}
				if (dynamic_cast<GameObject::CScriptable*>(&object)){
					out.push_back({ "checkForItem", &T::lCheckForItem });
					out.push_back({ "giveItem", &T::lGiveItem });
					out.push_back({ "inventorySize", &T::lInventorySize });
					out.push_back({ "takeItem", &T::lTakeItem });
				}
				if (dynamic_cast<GameObject::Item*>(&object)){
					// No Lua Functions
				}
				if (dynamic_cast<GameObject::SpriteEntity*>(&object)){
					out.push_back({ "setSprite", &T::lSetSprite });
					out.push_back({ "getSprite", &T::lGetSprite });
				}
				if (dynamic_cast<GameObject::AnimatedEntity*>(&object)){
					out.push_back({ "activateAnimation", &T::lActivateAnimation });
					out.push_back({ "addAnimation", &T::lAddAnimation });
				}
				if (dynamic_cast<GameObject::Avatar*>(&object)){
					// No Lua Functions
				}
				if (dynamic_cast<Asset::AnimatedSprite*>(&object)){
					// No Lua Functions
				}
				if (dynamic_cast<Asset::Sprite*>(&object)){
					// No Lua Functions
				}
				if (dynamic_cast<Scene::Camera*>(&object)){
					out.push_back({ "move", &T::lMove });
				}
				if (dynamic_cast<GUI::GUIButton*>(&object)){
					// No Lua Functions
				}
				if (dynamic_cast<GUI::GUIEntity*>(&object)){
					out.push_back({ "getParent", &T::lGetParent });
					out.push_back({ "setParent", &T::lSetParent });
				}
				if (dynamic_cast<GUI::GUIContext*>(&object)){
					out.push_back({ "attachWindow", &T::lAttachWindow });
					out.push_back({ "detachWindow", &T::lDetachWindow });
					out.push_back({ "getContextName", &T::lGetContextName });
				}
				if (dynamic_cast<GUI::GUIContainer*>(&object)){
					out.push_back({ "addComponent", &T::lAddComponent });
				}
				return out;
			}

			template<typename T> void defineMetatable(lua_State* L){

				auto fields = createMetatable<T>();
				luaL_newmetatable(L, &T::lGetMetatable()[0]);

				for each(auto field in fields){
					lua_pushcfunction(L, field.func);
					lua_setfield(L, -2, field.name);
				}

				lua_pushstring(L, "__index");
				lua_pushvalue(L, -2);
				lua_settable(L, -3);
			}
		
		public:
			// Produces all metatables for Lua's integration
			void defineMetatables(lua_State* L){

				defineMetatable<GameObject::Avatar>(L);
				defineMetatable<Scene::Camera>(L);
				defineMetatable<GUI::GUIButton>(L);
				defineMetatable<GUI::GUIContext>(L);
				defineMetatable<GUI::GUIContext>(L);
				defineMetatable<GUI::GUIWindow>(L);
			}
			 
		};
	}
}