#pragma once
#include "CScriptable.h"
using namespace std;

using namespace Pro;
using namespace Component;

CScriptable::CScriptable(){
	luaFunction = "";
}
CScriptable::~CScriptable(){

}

// Calls the assigned Lua Script
void CScriptable::update(){
	
}

// Assigns a function's name to the object
void CScriptable::attachFunction(const string& function){
	luaFunction = function;
}
