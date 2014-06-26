/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Class to process the map data from the file, provides a translation layer
	between GameObject::Map and the Game File 
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/
#pragma once

#include "..\..\util\CBuffer.h"

namespace Pro{
	namespace IO{
		class GameFileScript
		{
			unsigned int scriptID;
			char* script;

		public:
			GameFileScript();
			~GameFileScript();

			char* getScript();
			void executeScript();

			void setScript(CBuffer buffer);
		};
	}
}