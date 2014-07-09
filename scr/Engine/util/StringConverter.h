/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Provides functions to convert string's to other datatypes
-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include "..\Math.h"
#include <string>

namespace Pro{
	namespace Util{
		inline bool stringToBoolean(const std::string& str){
			return (str == "true") ? true : false;
		}

		inline int stringToInt(const std::string& str){
			return atoi(&str[0]);
		}

		// String format of "x y z w"
		inline Math::Vector4 stringToVec4(const std::string& str){
			size_t pos[3];
			pos[0] = str.find(' ');
			pos[1] = str.find(' ', pos[0] + 1);
			pos[2] = str.find_last_of(' ');

			return Math::Vector4(
				atof(&str[0]),
				atof(&str[pos[0]]),
				atof(&str[pos[1]]),
				atof(&str[pos[2]]));
		}
	}
}
