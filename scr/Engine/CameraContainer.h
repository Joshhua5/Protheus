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

#include "GUIDLookup.h"
#include "Camera.h"

namespace Pro{ 
	class CameraContainer
	{
		std::unordered_map<uint32, Scene::Camera*> stored_cameras;
		Scene::Camera* active_camera;
	public:
		CameraContainer(){ active_camera = nullptr; }
		~CameraContainer(){}
		    
		Scene::Camera* getCamera(uint32);
		Scene::Camera* getActiveCamera();
		void setActiveCamera(uint32);
		void addCamera(Scene::Camera*);
		void destoryCamera(uint32);
	}; 
}