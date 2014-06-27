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
#include "..\..\component\CGUID.h"

namespace Pro{
	namespace IO{
		class GameFileScript :  
			public Component::CGUID
		{ 
			char* script;

		public:
			// Accepts a buffer pass to setScript
			GameFileScript(CBuffer);
			GameFileScript();
			~GameFileScript();

			// Returns a pointer to the script
			// is of type char* because the script may be
			// pre compiled
			char* getScript();

			// executes the loaded script, returns false if
			// script returns an error or isn't a valid script
			bool executeScript();

			// Attachs the contents of the buffer to the script
			// assuming it's a valid Lua executable
			void setScript(CBuffer buffer);
		};
	}
}