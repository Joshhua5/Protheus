#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Utilities_Test
{
	using namespace Pro;
	using namespace Util;

	TEST_CLASS(LinkedListTest) {

		struct Complex {
			int x_, y_, z_, w_;
			Complex() {}
			Complex(int x, int y, int z, int w) {
				x_ = x;
				y_ = y;
				z_ = z;
				w_ = w;
			}

			int sum() { return x_ + y_ + z_ + w_; }


		}; 

public:
		
		TEST_METHOD(Creation) {
			LinkedList<Complex> list;

			Assert::AreEqual(static_cast<void*>(nullptr), (void*)list.GetStartIterator());
			Assert::AreEqual(static_cast<void*>(nullptr), (void*)list.GetEndIterator());

			Assert::AreEqual(0U, list.size());
			Assert::IsTrue(list.empty());
		}

		TEST_METHOD(SimpleInsert) {
			LinkedList<Complex> list;
			 
			list.PushBack(Complex(1, 2, 3, 4));
			auto end_iterator = list.GetEndIterator();
			 
			Assert::AreEqual(1 + 2 + 3 + 4, end_iterator->_ptr.sum()); 

			list.PushFront(Complex(5, 6, 7, 8));

			auto front_iterator = list.GetStartIterator();
			 
			Assert::AreEqual(5 + 6 + 7 + 8, front_iterator->_ptr.sum());
			Assert::AreEqual(1 + 2 + 3 + 4, front_iterator->_next->_ptr.sum()); 

			Assert::AreEqual(2U, list.size()); 
			Assert::IsFalse(list.empty()); 
		}

		TEST_METHOD(SimpleGet) {
			LinkedList<Complex> list;

			for (int i = 0; i < 64; ++i)
				list.PushBack(Complex(i, i, i, i));

			Assert::AreEqual(64U, list.size());

			for (int i = 0; i < 64; ++i) 
				Assert::AreEqual(i, list.At(i).x_);
		}

		TEST_METHOD(SimpleIteratorGet) {
			LinkedList<Complex> list;

			for (int i = 0; i < 64; ++i)
				list.PushBack(Complex(i, i, i, i));

			Assert::AreEqual(64U, list.size());

			auto iterator = list.GetStartIterator();

			for (int i = 0; i < 64; ++i) { 
				Assert::AreEqual(i, iterator->_ptr.x_);
				iterator = iterator->_next;
			}
		}
		 
		TEST_METHOD(DeleteOnSingle){ 
			LinkedList<Complex> list;

			list.PushBack(Complex(1, 1, 1, 1));

			Assert::AreEqual(1U, list.size());

			Complex removedObject;
			list.Remove(0, &removedObject);
				
			Assert::AreEqual(0U, list.size());
			Assert::AreEqual(1, removedObject.w_); 
		}
		
		TEST_METHOD(PopOnSingle) {
			LinkedList<Complex> list;

			list.PushBack(Complex(1, 1, 1, 1));

			Assert::AreEqual(1U, list.size());

			Complex removedObject = list.PopFront();

			Assert::AreEqual(0U, list.size());
			Assert::AreEqual(1, removedObject.w_);
		}

		TEST_METHOD(RemoveSingle){ 
			LinkedList<Complex*> list;

			Complex* object = new Complex(1, 1, 1, 1);

			list.PushBack(object);

			Assert::AreEqual(1U, list.size());

			Assert::IsTrue(list.RemoveObject(object));

			Assert::AreEqual(0U, list.size());
			Assert::AreEqual(1, object->w_);

			delete object;		
		}

		TEST_METHOD(DeleteOnMultiple){}
		TEST_METHOD(PopOnMultiple){}
		TEST_METHOD(RemoveMultiple) {}
		TEST_METHOD(RandomInsertAndDelete){}
		TEST_METHOD(ThreadedInsertAndDelete){}
		TEST_METHOD(ProducerConsumer){}
		TEST_METHOD(RandomSeek){}
		TEST_METHOD(RandomSeekWithThreadedInsertDelete) {}	

		/*

		TEST_METHOD(SimpleDelete) {
			LinkedList<Complex> linkedArray(64);
			vector<unsigned> deleted_set = { 2, 16, 32, 48 };

			for (int i = 0; i < 64; ++i)
				linkedArray.Emplace(i, i, i, i);

			for (auto index : deleted_set)
				linkedArray.DeleteAt(index);

			Assert::AreEqual(64 - deleted_set.size(), linkedArray.Count(), L"", LINE_INFO());

			int offset = 0;
			for (int i = 0; i < static_cast<int>(64 - deleted_set.size()); ++i) {
				auto it = std::find(deleted_set.begin(), deleted_set.end(), i + offset);
				if (it != deleted_set.end())
					offset++;

				Assert::AreEqual(i + offset, linkedArray.At(i)->x_, L"", LINE_INFO());
			}
		}


		TEST_METHOD(MultiChunkInsertAndGet) {
			LinkedList<Complex> linkedArray(64);

			for (int i = 0; i < 256; ++i)
				linkedArray.Emplace(i, i, i, i);

			Assert::AreEqual(256U, linkedArray.Count(), L"", LINE_INFO());

			for (int i = 0; i < 256; ++i)
				Assert::AreEqual(i, linkedArray.At(i)->x_, L"", LINE_INFO());
		}

		TEST_METHOD(FragmentedMultiChunkInsertAndGet) {
			// Test data
			LinkedList<Complex> linkedArray(64);
			vector<int> added_set_1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			vector<int> added_set_2 = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

			vector<int> removed_index_set_1 = { 2, 4, 6, 8 };
			vector<int> removed_index_set_2 = { 2, 4, 6, 8 };

			// Work on the linked array
			for (auto i : added_set_1)
				linkedArray.Emplace(i, i, i, i);
			for (auto i : removed_index_set_1)
				linkedArray.DeleteAt(i);

			for (auto i : added_set_2)
				linkedArray.Emplace(i, i, i, i);
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
			for (int i = 0; i < expected_set.size(); ++i)
				Assert::AreEqual(expected_set[i], linkedArray.At(i)->x_, L"", LINE_INFO());

			Assert::AreEqual(expected_set.size(), linkedArray.Count(), L"", LINE_INFO());
		}

		TEST_METHOD(Fragmented) {
			LinkedList<Complex> linkedArray(64);

			for (int i = 0; i < 64; ++i)
				linkedArray.Emplace(i, i, i, i);

			for (int i = 0; i < 32; i++)
				linkedArray.DeleteAt(i);

			Assert::AreEqual(0.5f, linkedArray.Fragmentation());
		}

		TEST_METHOD(Collapse) {
			LinkedArray<Complex> linkedArray(64);
			vector<int> removed_index_set_1 = { 32, 48, 16 };
			vector<int> removed_index_set_2 = { 86, 96, 70 };


			for (int i = 0; i < 64; ++i)
				linkedArray.Emplace(i, i, i, i);
			for (auto i : removed_index_set_1)
				linkedArray.DeleteAt(i);

			for (int i = 64; i < 128; ++i)
				linkedArray.Emplace(i, i, i, i);
			for (auto i : removed_index_set_2)
				linkedArray.DeleteAt(i);

			linkedArray.Collapse();

			Assert::AreEqual(1.0f, linkedArray.Fragmentation());
		}
		*/

	};
}