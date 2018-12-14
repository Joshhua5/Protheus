#include "stdafx.h"
#include "CppUnitTest.h"

#include <Entity/Entity.h>
#include <Entity/EntityIterator.h>


using namespace Pro;
using namespace ECS;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Engine_Test
{
	TEST_CLASS(EntityTest) {


		struct Position { 
			Position() { }
			Position(int _x) {
				x = _x;
			}

			int x, y, z;
		};

		struct Identifier {
			size_t ID;
		};

		TEST_METHOD(NewInstance) {
			Entity entity("test entity");

			for (int i = 0; i < 32; ++i)
				entity.NewInstance();

			Assert::AreEqual(32U, entity.Instances());
		}

		TEST_METHOD(NewInstanceWithComponent) {
			Entity entity("test entity");
			entity.AddComponent<Enabled>([](void* comp) { static_cast<Enabled*>(comp)->enabled = true; });

			for (int i = 0; i < 32; ++i)
				entity.NewInstance();

			Assert::AreEqual(32U, entity.Instances());
		}

		TEST_METHOD(NewInstanceWithDelayedComponent) {
			Entity entity("test entity");

			for (int i = 0; i < 32; ++i)
				entity.NewInstance();

			// Add a Component to a entity with instances, this should cause them to be filled in 
			entity.AddComponent<Position>([&](void* comp) {
				static int count = 0;
				static_cast<Position*>(comp)->x = count++;
			});

			Assert::AreEqual(32U, entity.Instances());

			auto iterator = entity.Iterator<Position>();
			int expected = 0;
			while (iterator.HasNext())
				Assert::AreEqual(expected++, iterator.Read()->x);
		}
	
		TEST_METHOD(AddComponent) {
			Entity entity("test entity");
			entity.AddComponent<Enabled>([](void* comp) { static_cast<Enabled*>(comp)->enabled = true; });
			entity.AddComponent<Position>([](void* comp) { static_cast<Position*>(comp)->x = 10; });

			Assert::IsTrue(entity.Contains<Enabled>());
			Assert::IsTrue(entity.Contains<Position>());

			entity.NewInstance();

			Assert::AreEqual(1U, entity.Instances());

			auto enabled_iterator = entity.Iterator<Enabled>();
			auto position_iterator = entity.Iterator<Position>();

			Assert::AreEqual(true, enabled_iterator.Read()->enabled);
			Assert::AreEqual(10, position_iterator.Read()->x);
		}

		TEST_METHOD(AddComponentComplex) {
			Entity entity("test entity");
			
			// Must be static since we refer to it from the component constructors.
			static int identifier = 0;
			
			// Constructors for the components are called in the order that they're added to the entity. 
			entity.AddComponent<Position>([&](void* comp) { 
				static_cast<Position*>(comp)->x = identifier;
			});

			entity.AddComponent<Identifier>([&](void* comp) { 
				static_cast<Identifier*>(comp)->ID = identifier++;
			});
			   
			for (int i = 0; i < 1024 * 100; ++i)
				entity.NewInstance();

			auto iterator = entity.Iterator<Position>();
			int expected = 0;
			while (iterator.HasNext())
				Assert::AreEqual(expected++, iterator.Read()->x);
		}
	
		TEST_METHOD(ComponentIterator) {
			Entity entity("test entity");
			entity.AddComponent<Position>([](void* comp) { static_cast<Position*>(comp)->x = 0;});

			for (int i = 0; i < 10; ++i)
				entity.NewInstance();
			 
			auto iterator = entity.Iterator<Position>();
			unsigned count = 0;
			while (iterator.HasNext()) {
				iterator.Read();
				count++;
			} 

			Assert::AreEqual(entity.Instances(), count);
		}
		
		TEST_METHOD(ComponentIteratorContent) {
			Entity entity("test entity");
			entity.AddComponent<Position>([](void* comp) {
				static int count = 0;
				static_cast<Position*>(comp)->x = count++;
			});

			for (int i = 0; i < 10; ++i)
				entity.NewInstance();

			auto iterator = entity.Iterator<Position>();
			int expected = 0;
			while (iterator.HasNext())
				Assert::AreEqual(expected++, iterator.Read()->x);
		}
		
		
		TEST_METHOD(LargeIteratorTest) {
			Entity entity("test entity");
			entity.AddComponent<Position>([](void* comp) {
				static int count = 0;
				static_cast<Position*>(comp)->x = count++;
				});

			for (int i = 0; i < 1024 * 100; ++i)
				entity.NewInstance();


			auto iterator = entity.Iterator<Position>();  
			int expected = 0;
			while (iterator.HasNext())
				Assert::AreEqual(expected++, iterator.Read()->x);
		}

		TEST_METHOD(LargeIteratorTest) {
			Entity entity("test entity");
			entity.AddComponent<Position>([](void* comp) {
				static int count = 0;
				static_cast<Position*>(comp)->x = count++;
			});

			for (int i = 0; i < 1024 * 100; ++i)
				entity.NewInstance();

			auto iterator = entity.Iterator<Position>();
			int expected = 0;
			while (iterator.HasNext())
				Assert::AreEqual(expected++, iterator.Read()->x); 
		}
	};
}