#pragma once

#include "..\component\CGUID.h"

namespace Pro{
	namespace GameObject{
		using namespace std;

		class Entity :
			public Component::CGUID{
		public:
			Entity(const string& name) : CGUID(name){}
			Entity() : CGUID(){}
		};
	}
}