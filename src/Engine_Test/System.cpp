#include "stdafx.h"
#include "CppUnitTest.h"
#include <Entity\Components\Position.h>

using namespace Pro;
using namespace ECS;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

 
namespace Engine_Test
{
	TEST_CLASS(SystemTest) {
		 
		 class EnableSetSystem : System<Enabled> { 
		 public:    
		 	void Execute(Entity& entity) { 
		 		Reset(entity);
		 
		 		Enabled* enabled;
		 		while ((enabled = Next<Enabled>()) != nullptr) {
		 			enabled->enabled = true;
		 		}
		 	};
		 }; 

		 struct Velocity : Math::Vector4<float> { 
		 };

		TEST_METHOD(Initialize) {
			Entity entity("test entity");
			entity.AddComponent<Enabled>([](void* comp) {
				static_cast<Enabled*>(comp)->enabled = false;
			});

			for (int i = 0; i < 1024 * 100; ++i)
				entity.NewInstance();

			EnableSetSystem system;

			system.Execute(entity);
			 
			auto iterator = entity.Iterator<Enabled>().Get<Enabled>();
			 
			Enabled* value;
			while((value = iterator.Read()) != nullptr) 
				Assert::AreEqual(true, value->enabled); 
		}
		
		TEST_METHOD(LambdaInitialize) {
			Entity entity("test entity");
			entity.AddComponent<Enabled>([](void* comp) {
				static_cast<Enabled*>(comp)->enabled = false;
			});

			for (int i = 0; i < 1024 * 100; ++i)
				entity.NewInstance();

			System<Enabled> system([](System<Enabled>& system) { 
				Enabled* enabled;
				while ((enabled = system.Next<Enabled>()) != nullptr) {
					enabled->enabled = true;
				}
			});

			system.Execute(entity);

			auto iterator = entity.Iterator<Enabled>().Get<Enabled>();

			Enabled* value;
			while ((value = iterator.Read()) != nullptr)
				Assert::AreEqual(true, value->enabled);
		}

		TEST_METHOD(GravitySystem) {
			Entity entity("balls"); 
			entity.AddComponent<Position>([](Position* component) {
				component->Set(0, 0, 0, 1);
			});
			unsigned increment = 0;
			entity.AddComponent<Velocity>([&](Velocity* component) {
				component->Set(0, increment++ * 0.01 , 0, 0);
			});

			for (int i = 0; i < 1024 * 1024; ++i)
				entity.NewInstance();

			// A new version of the iterator could be 
			// to strip out the bitmask on reads which has proved to be a 
			// bottleneck. And provide a required Lifetime object which
			// contains if a entity is enabled or constructed.
			// If we transform the function that the system is constructed wth
			// to work on components as parameters instead of iterators
			// we this have a good way to use the internal Lifetime objects
			// without complicating the system function.
			System<Position, Velocity> ProcessPhysics([](System<Position, Velocity>& system) {
				Position* position;
				while ((position = system.Next<Position>()) != nullptr) {
					auto velocity = system.Next<Velocity>();
					 
					velocity->y -= 9.8f / 1000.f;
					*position += *velocity;

					if (position->y <= 0) {
						velocity->y *= -0.8f;
						position->y = 0.01f;
					}
				}
			}); 

			for (int i = 0; i < 100; ++i) {
				ProcessPhysics.Execute(entity);
			}
		}
	};
}