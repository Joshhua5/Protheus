#pragma once
/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2018.
 -------------------------------------------------------------------------

 Description:

 A system is 
  

 *************************************************************************/

#include <ArrayList.h>

namespace Pro {
	namespace ECS {
		class System { 
			
			 
			void virtual Update() = 0;
		};
	}
}