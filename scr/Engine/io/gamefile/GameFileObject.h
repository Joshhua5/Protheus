/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

#include "GameFileBase.h"
#include "..\..\gameobject\Avatar.h"
#include "..\..\gameobject\Camera.h"
#include "..\..\gameobject\SpriteEntity.h"

namespace Pro{
	namespace IO{
		using namespace GameObject;
		class GameFileObject : 
			public GameFileBase
		{
			Entity* object = nullptr;
		public:
			GameFileObject();

			template<typename T>
			void pack(const string& name, T* obj){/* Template Specialization */ }

			void unpack(GameFileChunk& chunk);

			Entity* getObject();
		};
	}
} 