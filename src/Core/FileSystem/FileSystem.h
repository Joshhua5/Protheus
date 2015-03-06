/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2015.
-------------------------------------------------------------------------

Description:
Handles all File Operations for platform independance,
All path's are relative to the set RootDir which is relative
to the executable unless defined as an absolute path.
-------------------------------------------------------------------------
History:
- 27:05:2014: Waring J.
*************************************************************************/

#pragma once

#include "CFile.h"
#include <string>

namespace Pro{
	namespace Util{

		class FileSystem
		{ 
			std::string rootDir_;
		public:
			FileSystem(const std::string& root = "");
			~FileSystem();

			FileSystem& operator=(const FileSystem& rhs) {
				rootDir_ = std::string(rhs.rootDir_);
				return *this;
			}

			/*  Opens a file relative to the root directory */
			IO::CFile* OpenFile(const std::string& path);
			/*  Sets the root directory */
			void rootDir(const std::string& dir);
			/*  Returns a reference to the root directory */
			std::string& rootDir();

			Buffer GetFile(const std::string& path);
		};
	}
}
