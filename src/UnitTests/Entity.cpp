#include "stdafx.h"
#include "CppUnitTest.h"

#include <Entity/Entity.h>


using namespace Pro;
using namespace ECS;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
 

namespace UnitTests
{
	TEST_CLASS(ENTITY_TEST) {


		struct Position : Component {
			static unsigned ID() {
				return 1;
			}

			float x, y, z;
		};

		TEST_METHOD(CreateEntity) {
			Entity entity("test entity");
		}


		TEST_METHOD(AddComponent) {
			Entity entity("test entity");
			entity.AddComponent<Position>();
		}
	};
}