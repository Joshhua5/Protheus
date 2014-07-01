/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
	Data hiding of the game file header
-------------------------------------------------------------------------
History:
- 25:06:2014: Waring J.
*************************************************************************/

#pragma once

namespace Pro{
	namespace IO{
		class GameFileHeader
		{
		public:
			// contains the ID of the script which will execute
			// this will define the resources to be loaded and
			// which scene will be loaded at first
			unsigned int configScriptID;
			// a offset in the file(in bytes) to the index
			// which will store all images and their details
			unsigned int imageIndexOffset;
			// a offset in the file to the index of scripts
			// which will store and load all scripts in the game
			unsigned int scriptIndexOffset;
			// position of the index that stores all
			// audio files and their ID
			unsigned int audioIndexOffset;
			// position to a index which defines all tiles used in the map
			unsigned int mapTileIndexOffset;
			// position to a index that stores all objects
			unsigned int objectIndexOffset;
			// position to the index that stores all the maps
			unsigned int mapIndexOffset;
			// provides a index to raw data, which have no specified 
			// type
			unsigned int rawIndexOffset; 
		};

	}
}