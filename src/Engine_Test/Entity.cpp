#include "stdafx.h"
#include "CppUnitTest.h"

#include <Entity/Entity.h>


using namespace Pro;
using namespace ECS;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
 

namespace UnitTests
{
	TEST_CLASS(EntityTest) {


		struct Position : Component {
			static unsigned ID() {
				return 1;
			}
			
			Position() { }
			Position(int _x) {
				x = _x;
			}

			int x, y, z;
		};

		TEST_METHOD(NewInstance) {
			Entity entity("test entity"); 

			for(int i = 0; i < 32; ++i)
				entity.NewInstance();
			 
			Assert::AreEqual(32U, entity.Instances());
		} 

		TEST_METHOD(AddComponent) {
			Entity entity("test entity");
			entity.AddComponent<Position>([](void* comp) { static_cast<Position*>(comp)->x = 10; });
			  
			Assert::IsTrue(entity.Contains<Enabled>()); 
			Assert::IsTrue(entity.Contains<Position>());

			entity.Get
			 
			Assert::AreEqual(10, entity.Get<Position>()->x); 
		}

		TEST_METHOD(GetComponentIterator) {
			Entity entity("test entity");
			entity.AddComponent<Position>();

			for (int i = 0; i < 10; ++i) {
				//auto ref = entity.CreateWithRef();
				//ref.Initialize<Position>(i);
			}

			auto iterator = entity.GetComponentIterator<Position>();
			int expected = 0;
			while (iterator.HasNext())
				Assert::AreEqual(expected++, iterator.Read()->x); 
		} 
	};
}