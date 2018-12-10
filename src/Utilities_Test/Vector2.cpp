#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Pro;
using namespace Math;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(Vector2) {


		TEST_METHOD(CONSTRUCTOR) {
			Pro::Math::Vector2<int> x(1);
			Pro::Math::Vector2<int> y(1, 2);
			Pro::Math::Vector2<int> z({ 4, 8 });
			Pro::Math::Vector2<int> w(y);
			Pro::Math::Vector2<int> r(Pro::Math::Vector2<int>(2, 2));

			Assert::IsTrue(x.x == 1 && x.y == 1, L"", LINE_INFO());
			Assert::IsTrue(y.x == 1 && y.y == 2, L"", LINE_INFO());
			Assert::IsTrue(z.x == 4 && z.y == 8, L"", LINE_INFO());
			Assert::IsTrue(w.x == 1 && w.y == 2, L"", LINE_INFO());
			Assert::IsTrue(r.x == 2 && r.y == 2, L"", LINE_INFO());
		}

		TEST_METHOD(ADDITION) {
			Pro::Math::Vector2<int> x(1);
			Pro::Math::Vector2<int> y(1, 2);
			Pro::Math::Vector2<int> result1 = x + y;
			Pro::Math::Vector2<int> result2(x + y);

			Assert::IsTrue(result1.x == 2 && result1.y == 3, L"", LINE_INFO());
			Assert::IsTrue(result2.x == 2 && result2.y == 3, L"", LINE_INFO());

			result1 += x;

			Assert::IsTrue(result1.x == 3 && result1.y == 4, L"", LINE_INFO());
		}

		TEST_METHOD(SUBTRACTION) {
			Pro::Math::Vector2<int> x(1);
			Pro::Math::Vector2<int> y(1, 2);
			Pro::Math::Vector2<int> result1 = x - y;
			Pro::Math::Vector2<int> result2(x - y);

			Assert::IsTrue(result1.x == 0 && result1.y == -1, L"", LINE_INFO());
			Assert::IsTrue(result2.x == 0 && result2.y == -1, L"", LINE_INFO());

			result1 -= y;

			Assert::IsTrue(result1.x == -1 && result1.y == -3, L"", LINE_INFO());
		}

		TEST_METHOD(DIVISION) {
			Pro::Math::Vector2<int> x(4);
			Pro::Math::Vector2<int> y(1, 2);
			Pro::Math::Vector2<int> result1 = x / y;
			Pro::Math::Vector2<int> result2(x / y);

			Assert::IsTrue(result1.x == 4 && result1.y == 2, L"", LINE_INFO());
			Assert::IsTrue(result2.x == 4 && result2.y == 2, L"", LINE_INFO());

			result1 /= y;

			Assert::IsTrue(result1.x == 4 && result1.y == 1, L"", LINE_INFO());
		}

		TEST_METHOD(MULTIPLICATION) {
			Pro::Math::Vector2<int> x(1);
			Pro::Math::Vector2<int> y(1, 2);
			Pro::Math::Vector2<int> result1 = x * y;
			Pro::Math::Vector2<int> result2(x * y);

			Assert::IsTrue(result1.x == 1 && result1.y == 2, L"", LINE_INFO());
			Assert::IsTrue(result2.x == 1 && result2.y == 2, L"", LINE_INFO());

			result1 *= y;

			Assert::IsTrue(result1.x == 1 && result1.y == 4, L"", LINE_INFO());

		}

		TEST_METHOD(CAST) {
			Pro::Math::Vector2<int> x(1);
			auto cast = x.Cast<float>();
			Assert::IsTrue(cast.x == 1.f || cast.y == 1.f, L"", LINE_INFO());
		}

		TEST_METHOD(EQUALS) {
			Pro::Math::Vector2<int> x(1, 2);

			Assert::IsTrue(x == x, L"", LINE_INFO());
			Assert::IsTrue(x.Equals(1, 2), L"", LINE_INFO());
		}


		TEST_METHOD(CONTAINS) {
			Pro::Math::Vector2<int> z({ 4, 8 });

			Assert::IsTrue(z.Contains(6), L"", LINE_INFO());
			Assert::IsFalse(z.Contains(2), L"", LINE_INFO());
			Assert::IsFalse(z.Contains(10), L"", LINE_INFO());
		}

		TEST_METHOD(LENGTH) {
			Pro::Math::Vector2<int> x(4, 3);
			Assert::IsTrue(x.Length() == 5, L"", LINE_INFO());
		}

		TEST_METHOD(NORMALIZE) {
			Pro::Math::Vector2<int> x(1);
			auto cast = x.Cast<float>();
			Assert::IsTrue(cast.Normalize() != Math::Vector2<float>(0.707f), L"", LINE_INFO());
			Assert::IsTrue(cast.NormalizeThis() != Math::Vector2<float>(0.707f), L"", LINE_INFO());
		}
	};
}
