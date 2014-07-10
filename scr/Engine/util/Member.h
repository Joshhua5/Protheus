#pragma once
#include <string>
#include "Member.h"
#include <vector>

namespace Pro{
	namespace Serializer{
		using namespace std;

		struct Member{
			union{
				// used in writing
				unsigned offset;
				// used in reading
				void* data;
			};
			unsigned size;
			string name;
		};
	}
}