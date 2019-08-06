#pragma once

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Utilities_Test
{
	using namespace Pro;
	using namespace Util;

	TEST_CLASS(ObjectPoolTest)
	{ 
		struct PoolObject {
			int data[32];
		};
		 

	public:

		TEST_METHOD(CreatePool) {
			ObjectPool<PoolObject> pool;
			Assert::AreNotEqual(0U, pool.BlockCount()); 
		}

		TEST_METHOD(CreateLargePool) {
			ObjectPool<PoolObject> pool(1024);
			Assert::AreNotEqual(0U, pool.BlockCount());
			Assert::AreEqual(1024U, pool.blockSize());
		}
		
		TEST_METHOD(CreateTinyPool) {
			ObjectPool<PoolObject> pool(2);
			Assert::AreNotEqual(0U, pool.BlockCount());
			Assert::AreEqual(2U, pool.blockSize());
		}

		TEST_METHOD(GetObject) {
			ObjectPool<PoolObject> pool(1024);
			Assert::AreNotEqual(0U, pool.BlockCount()); 
		}

		TEST_METHOD(GetConstructedObject) {

			ObjectPool<PoolObject> pool();


		}

		TEST_METHOD(MultipleObjectGet) { 
			ObjectPool<PoolObject> pool(16);
			auto obj1 = pool.Get();
			auto obj2 = pool.Get();
			auto obj3 = pool.Get();
			auto obj4 = pool.Get(); 

			Assert::IsFalse(obj1 == obj2, L"Same object returned from pool before Return");
			Assert::IsFalse(obj2 == obj3, L"Same object returned from pool before Return");
			Assert::IsFalse(obj3 == obj4, L"Same object returned from pool before Return"); 

			pool.Return(obj1);
			pool.Return(obj2);
			pool.Return(obj3);
			pool.Return(obj4);
		}

		TEST_METHOD(PoolExpansionAndReturn) { 
			ObjectPool<PoolObject> pool(16);
			std::vector<PoolObject*> leasedObjects;
			for (int i = 0; i < 128; i++)
				leasedObjects.push_back(pool.Get());
			 
			Assert::AreEqual(128U, pool.LeasedObjects());

			for (auto obj : leasedObjects)
				pool.Return(obj);
			 
			Assert::AreEqual(0U, pool.LeasedObjects()); 
		}

		TEST_METHOD(ObjectReuse) { 
			ObjectPool<PoolObject> pool(2); 

			auto obj1 = pool.Get();
			auto obj2 = pool.Get();

			pool.Return(obj1);
			pool.Return(obj2);

			auto obj3 = pool.Get();
			auto obj4 = pool.Get(); 

			Assert::IsTrue(obj1 == obj3 || obj2 == obj3, L"Failed to recycle object");
			Assert::IsTrue(obj1 == obj4 || obj2 == obj4, L"Failed to recycle object" );

			pool.Return(obj3);
			pool.Return(obj4);
		}

		TEST_METHOD(CreateTinyPoolLargeThrougput) 
		{
			ObjectPool<PoolObject> pool(4); 
			std::vector<PoolObject*> leasedObjects;
			unsigned leasedObjectCount = 512;

			for (int i = 0; i < leasedObjectCount; ++i)
				leasedObjects.push_back(pool.Get());

			for (int i = 0; i < 20; ++i) {
				// Get a random set to delete 
				int releaseSetSize = (rand() + 32) % 96;
				for (int i = 0; i < releaseSetSize; ++i) {
					int idx = rand() % leasedObjects.size();
					pool.Return(leasedObjects.at(idx));
					leasedObjects.erase(leasedObjects.begin() + idx);
				}

				// Request a random amount to get back
				int recycleCount = (rand() + 16) % 112;
				for (int i = 0; i < recycleCount; ++i)
					leasedObjects.push_back(pool.Get());

				leasedObjectCount += recycleCount - releaseSetSize;
			}

			// Detect duplicate objects which have incorrectly been served twice
			std::unordered_map<void*, int> duplicate_detection;
			for (auto object : leasedObjects) {
				Assert::AreEqual(0U, duplicate_detection.count(object), L"Duplicate objects found in leased list");
				duplicate_detection.insert({ object, 1 });
			}
			Assert::AreEqual(leasedObjectCount, pool.LeasedObjects(), L"Pool leased count doesn't match");
		}

		TEST_METHOD(LargeRandomObjectThroughput) {  
			auto ExtractRandomSet = [=](std::vector<PoolObject*>* leasedObjects, int size) -> std::vector<PoolObject*> 
			{
				// Get a random set to delete
				std::vector<PoolObject*> toRelease;
				return toRelease;
			}; 

			ObjectPool<PoolObject> pool; 
			std::vector<PoolObject*> leasedObjects;
			unsigned leasedObjectCount = 512;

			for (int i = 0; i < leasedObjectCount; ++i)
				leasedObjects.push_back(pool.Get());
			 
			for (int i = 0; i < 20; ++i) {
				// Get a random set to delete 
				int releaseSetSize = (rand() + 32) % 96;  
				for (int i = 0; i < releaseSetSize; ++i) {
					int idx = rand() % leasedObjects.size(); 
					pool.Return(leasedObjects.at(idx));
					leasedObjects.erase(leasedObjects.begin() + idx);  
				} 

				// Request a random amount to get back
				int recycleCount = (rand() + 16) % 112;
				for (int i = 0; i < recycleCount; ++i)
					leasedObjects.push_back(pool.Get());

				leasedObjectCount += recycleCount - releaseSetSize;
			}
			 
			// Detect duplicate objects which have incorrectly been served twice
			std::unordered_map<void*, int> duplicate_detection; 
			for (auto object : leasedObjects) {
				Assert::AreEqual(0U, duplicate_detection.count(object), L"Duplicate objects found in leased list");
				duplicate_detection.insert({ object, 1 }); 
			}
			Assert::AreEqual(leasedObjectCount, pool.LeasedObjects(), L"Pool leased count doesn't match");
		}
	};
}