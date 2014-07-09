/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Stores cameras with reference from their GUID's
also contains a active_camera which will be used for rendering
-------------------------------------------------------------------------
History:
- 17:05:2014 Waring J.
*************************************************************************/

#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include "..\GUIDLookup.h"
#include "..\GameObjects.h"

namespace Pro{
	using namespace GameObject;

	class CameraContainer
	{
		std::unordered_map<game_id, DataEntity*> stored_cameras;
		DataEntity* active_camera;
	public:
		CameraContainer(){ active_camera = nullptr; }
		~CameraContainer(){}

		DataEntity* getCamera(game_id);
		DataEntity* getActiveCamera();
		void setActiveCamera(game_id);
		void addCamera(DataEntity*);
		void destoryCamera(game_id);
	};
}