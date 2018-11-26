#pragma once
/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2018.
 -------------------------------------------------------------------------

 Description:

 A component is a collection of data, grouped together by an Entity and processed by a System
  

 *************************************************************************/

namespace Pro {
	namespace ECS {
		class Component { 
		protected:
			constexpr static unsigned FowlerNollVoHash(char const* s, size_t count) {
				return ((count ? FowlerNollVoHash(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
			}
		public:
			static unsigned ID();
		};
	}
}