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
			 

			Complex* object1 = reinterpret_cast<Complex*>(linkedArray.Insert());
			*object1 = Complex(1, 2, 3, 4);

			Assert::AreNotEqual((void*)nullptr, (void*)object1);
			Assert::AreEqual(1, object1->x_);
			Assert::AreEqual(2, object1->y_);
			Assert::AreEqual(3, object1->z_);
			Assert::AreEqual(4, object1->w_);
			 
			Complex* object2 = linkedArray.Insert<Complex>(5, 6, 7, 8);
			 
			Assert::AreNotEqual((void*)nullptr, (void*)object2);
			Assert::AreEqual(5, object2->x_);
			Assert::AreEqual(6, object2->y_);
			Assert::AreEqual(7, object2->z_);
			Assert::AreEqual(8, object2->w_); 
		}

		TEST_METHOD(SimpleGet) {
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 64; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);

			Assert::AreEqual(64U, linkedArray.Count(), L"", LINE_INFO());
			 
			for (int i = 0; i < 64; ++i)
				Assert::AreEqual(i, linkedArray.At<Complex>(i)->x_, L"", LINE_INFO()); 
		}

		TEST_METHOD(SimpleDelete) { 
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 64; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);

			linkedArray.Delete(32);
			linkedArray.Delete(48);
			linkedArray.Delete(16);

			Assert::AreEqual(64U, linkedArray.Count(), L"", LINE_INFO());

			int offset = 0;
			for (int i = 0; i < 64 - 3; ++i) {
				if (i == 32 || i == 16 || i == 48)
					offset++;
				Assert::AreEqual(i + offset, linkedArray.At<Complex>(i)->x_, L"", LINE_INFO());  
			}
		}


		TEST_METHOD(MultiChunkInsertAndGet) {
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 256; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);

			Assert::AreEqual(256U, linkedArray.Count(), L"", LINE_INFO());

			for (int i = 0; i < 256; ++i)
				Assert::AreEqual(i, linkedArray.At<Complex>(i)->x_, L"", LINE_INFO());
		}  

		TEST_METHOD(FragmentedMultiChunkInsertAndGet) { 
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 64; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);
			 
			linkedArray.Delete(32);
			linkedArray.Delete(48);
			linkedArray.Delete(16); 

			for (int i = 64; i < 128; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);


			linkedArray.Delete(86);
			linkedArray.Delete(96);
			linkedArray.Delete(70);


			int offset = 0;
			for (int i = 0; i < 64 - 3; ++i) {
				if (i == 32 || i == 16 || i == 48 || i == 86 || i == 96 || i == 70)
					offset++;
				Assert::AreEqual(i + offset, linkedArray.At<Complex>(i)->x_, L"", LINE_INFO());
			} 
		}

		TEST_METHOD(Fragmented) { 
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);
			for (int i = 0; i < 64; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);


			for (int i = 0; i < 32; i++)
				linkedArray.Delete(i);

			Assert::AreEqual(0.5f, linkedArray.Fragmentation());
		}

		TEST_METHOD(Collapse) {
			LinkedArrayRaw linkedArray(sizeof(Complex), 64);

			for (int i = 0; i < 64; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);

			linkedArray.Delete(32);
			linkedArray.Delete(48);
			linkedArray.Delete(16);

			for (int i = 64; i < 128; ++i)
				linkedArray.Insert<Complex>(i, i, i, i);


			linkedArray.Delete(86);
			linkedArray.Delete(96);
			linkedArray.Delete(70);

			linkedArray.Collapse();

			Assert::AreEqual(1.0f, linkedArray.Fragmentation()); 
		} 
	};
}