#include "stdafx.h"
#include "CppUnitTest.h"

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

	};
}