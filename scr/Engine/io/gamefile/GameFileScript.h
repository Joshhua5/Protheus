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

#include "GameFileChunk.h"
#include "..\..\component\CGUID.h"
#include "GameFileBase.h"

namespace Pro{
	namespace IO{
		class GameFileScript :
			public Component::CGUID,
			public GameFileBase
		{
			char* m_script;
		public:
			// Accepts a buffer pass to setScript
			GameFileScript(GameFileChunk);
			GameFileScript(){}

			// Returns a pointer to the script
			// is of type char* because the script may be
			// pre compiled
			char* getScript();

			// Attachs the contents of the buffer to the script
			// assuming it's a valid Lua executable
			// the CBuffer is dereferenced
			void unpack(GameFileChunk& script);

			// Packs a script into a chunk
			void pack(const string& script, const string& name); 
		};
	}
}