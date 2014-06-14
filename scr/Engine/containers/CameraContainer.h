/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Stores entities with reference from their GUID's

-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

#pragma once

#include <unordered_map> 
#include <iostream>
#include <string>
#include "..\GUIDLookup.h"
#include "..\gameobject\Camera.h"

namespace Pro{ 
	using namespace GameObject;

	class CameraContainer
	{
		std::unordered_map<uint32, Camera*> stored_cameras;
		Camera* active_camera;
	public:
		CameraContainer(){ active_camera = nullptr; }
		~CameraContainer(){}
		    
		Camera* getCamera(uint32);
		Camera* getActiveCamera();
		void setActiveCamera(uint32);
		void addCamera(Camera*);
		void destoryCamera(uint32);
	}; 
}