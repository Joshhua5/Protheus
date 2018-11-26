/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2018.
 -------------------------------------------------------------------------

 Description:

 A DataComponent is for data orientated components, you would use a data component
 for components that don't have many instances and contain a lot of references

 -------------------------------------------------------------------------
 History:
 - 24:11:2018 Waring J.

 *************************************************************************/

#pragma once
 
#include "ComponentData.h"

namespace Pro {
	namespace Entity {     
		template<typename DataStruct>
		class DataComponent {
		public:
			constexpr static unsigned FowlerNollVoHash(char const* s, size_t count) {
				return ((count ? FowlerNollVoHash(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
			} 

			constexpr virtual unsigned UUID() = 0;
			static virtual void Start(DataStruct* data) = 0;
			static virtual void Update(DataStruct* data) = 0;
		};
	}
}