/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	A GameFile class which has the base of GameFileBase, all write to their
	own buffers, once their buffer has been populated, the GameFileLoader
	will act as an api to mask the packing and unpacking
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/
#pragma once

#include "..\CFile.h"
#include <string>
#include "GameFileChunk.h"
#include "GameFileIndex.h"
#include "GameFileImage.h"
#include "GameFileMap.h"
#include "GameFileScript.h"
#include "GameFileRaw.h"
#include "GameFileAudio.h"
#include <unordered_map>

namespace Pro{
	namespace IO{

		using namespace std;
 

		class GameFileLoader
		{   
			unordered_map<string, unsigned int> image_index;
			unordered_map<string, unsigned int> script_index;
			unordered_map<string, unsigned int> audio_index;
			unordered_map<string, unsigned int> object_index;
			unordered_map<string, unsigned int> map_index;
			unordered_map<string, unsigned int> raw_index; 

			CFile* file;
		public:
			GameFileLoader(const string& file_path){ load(file_path); } 
			void load(const string& file_path);

			const char* getScript(const string& name);
			const char* getConfigScript();

			game_id getImage(lua_State*, const string& name);
			CBuffer getAudio(const string& name);  
			Map* getMap(const string& name);
			CBuffer getRaw(const string& name);
			
			template<typename T>
			T* getObject(const string& name){

			}
		};
	}
}
/*
Header{ 
Image Index Offset
Script Index Offset
Audio Index Offset
Object Index Offset
Map Index
Raw Index Offset
}

Data{
ChunkID
ChunkType
ChunkSize
}

*/