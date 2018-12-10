#include "stdafx.h"
#include "CppUnitTest.h"
  
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Pro;
using namespace Util;

namespace Utilities_Test
{
	TEST_CLASS(LinkedArrayRawTest) {

		struct Complex {
			int x_, y_, z_, w_;

			Complex(int x, int y, int z, int w) {
				x_ = x;
				y_ = y;
				z_ = z;
				w_ = w;
			}
		};



		static const unsigned QUEUE_COUNT = 1000000;
		  
		TEST_METHOD(Creation) { 
			LinkedArrayRaw linkedArray(sizeof(Complex));
		}
		 
		TEST_METHOD(SimpleInsert) {
			LinkedArrayRaw linkedArray(sizeof(Complex));
			 

			Complex* object1 = reinterpret_cast<Complex*>(linkedArray.Append());
			*object1 = Complex(1, 2, 3, 4);

			Assert::AreNotEqual((void*)nullptr, (void*)object1);
			Assert::AreEqual(1, object1->x_);
			Assert::AreEqual(2, object1->y_);
			Assert::AreEqual(3, object1->z_);
			Assert::AreEqual(4, object1->w_);
			 
			Complex* object2 = linkedArray.Append<Complex>(5, 6, 7, 8);
			 
			Assert::AreNotEqual((void*)nullptr, (void*)object2);
			Assert::AreEqual(5, object2->x_);
			Assert::AreEqual(6, object2->y_);
			Assert::AreEqual(7, object2->z_);
			Assert::AreEqual(8, object2->w_); 
		}

		TEST_METHOD(SimpleGet) {
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 64; ++i)
				linkedArray.Append<Complex>(i, i, i, i);

			Assert::AreEqual(64U, linkedArray.Count(), L"", LINE_INFO());
			 
			for (int i = 0; i < 64; ++i)
				Assert::AreEqual(i, linkedArray.At<Complex>(i)->x_, L"", LINE_INFO()); 
		}

		TEST_METHOD(SimpleDelete) { 
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);
			vector<unsigned> deleted_set = { 2, 16, 32, 48 };

			for (int i = 0; i < 64; ++i)
				linkedArray.Append<Complex>(i, i, i, i);

			for (auto index : deleted_set)
				linkedArray.DeleteAt(index);

			Assert::AreEqual(64 - deleted_set.size(), linkedArray.Count(), L"", LINE_INFO());
			    
			int offset = 0;
			for (int i = 0; i < static_cast<int>(64 - deleted_set.size()); ++i) {
				auto it = std::find(deleted_set.begin(), deleted_set.end(), i + offset);
				if(it != deleted_set.end())
					offset++;

				Assert::AreEqual(i + offset, linkedArray.At<Complex>(i)->x_, L"", LINE_INFO());  
			}
		}


		TEST_METHOD(MultiChunkInsertAndGet) {
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 256; ++i)
				linkedArray.Append<Complex>(i, i, i, i);

			Assert::AreEqual(256U, linkedArray.Count(), L"", LINE_INFO());

			for (int i = 0; i < 256; ++i)
				Assert::AreEqual(i, linkedArray.At<Complex>(i)->x_, L"", LINE_INFO());
		}  

		TEST_METHOD(FragmentedMultiChunkInsertAndGet) { 
			// Test data
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);
			vector<int> added_set_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			vector<int> added_set_2 = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
			
			vector<int> removed_index_set_1 = { 2, 4, 6, 8 };
			vector<int> removed_index_set_2 = { 2, 4, 6, 8 }; 

			// Work on the linked array
			for (auto i : added_set_1)
				linkedArray.Append<Complex>(i, i, i, i); 
			for (auto i : removed_index_set_1)
				linkedArray.DeleteAt(i);
			 
			for (auto i : added_set_2)
				linkedArray.Append<Complex>(i, i, i, i); 
			for (auto i : removed_index_set_2)
				linkedArray.DeleteAt(i);
			  
			// Create the data sets used for validation 
			vector<int> expected_set; 
			expected_set.insert(expected_set.end(), added_set_1.begin(), added_set_1.end());
			for (auto remove_index : removed_index_set_1) 
				if (remove_index < expected_set.size())
					expected_set.erase(expected_set.begin() + remove_index);

			expected_set.insert(expected_set.end(), added_set_2.begin(), added_set_2.end());
			for (auto remove_index : removed_index_set_2)
				if (remove_index < expected_set.size())
					expected_set.erase(expected_set.begin() + remove_index);
			  
			// Validate the data
			for(int i = 0; i < expected_set.size(); ++i)
				Assert::AreEqual(expected_set[i], linkedArray.At<Complex>(i)->x_, L"", LINE_INFO());
			
			Assert::AreEqual(expected_set.size(), linkedArray.Count(), L"", LINE_INFO());
		}

		TEST_METHOD(Fragmented) { 
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 64; ++i)
				linkedArray.Append<Complex>(i, i, i, i);
			 
			for (int i = 0; i < 32; i++)
				linkedArray.DeleteAt(i);

			Assert::AreEqual(0.5f, linkedArray.Fragmentation());
		}

		TEST_METHOD(Collapse) {
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);
			vector<int> removed_index_set_1 = { 32, 48, 16 };
			vector<int> removed_index_set_2 = { 86, 96, 70 };


			for (int i = 0; i < 64; ++i)
				linkedArray.Append<Complex>(i, i, i, i); 
			for (auto i : removed_index_set_1)
				linkedArray.DeleteAt(i); 

			for (int i = 64; i < 128; ++i)
				linkedArray.Append<Complex>(i, i, i, i); 
			for (auto i : removed_index_set_2)
				linkedArray.DeleteAt(i); 

			linkedArray.Collapse();

			Assert::AreEqual(1.0f, linkedArray.Fragmentation());  
		} 
		 
	};
}