#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Utilites_test
{
	TEST_CLASS(Vector2)
	{
	public:
		TEST_METHOD(Vector2_Constructors)
		{
			Pro::Vector2<float> x(0, 1);

		}

		TEST_METHOD(Vector2_Copies)
		{
			Pro::Vector2<float> x(1, 1);
			Pro::Vector2<float> y(0, 0);

			Pro::Vector2<float> z = y;
			Pro::Vector2<float> w = std::forward<Pro::Vector2<float>>(w);
			Pro::Vector2<float> h = std::forward<Pro::Vector2<float>>({ 1.f, 1.0f });

			Assert::AreEqual(z, y, L"Vector<float> w = y", LINE_INFO());
			Assert::AreEqual(w, w, L"Vector<float> z(x)", LINE_INFO());
			Assert::AreEqual(h, { 1.0f, 1.0f }, L"Vector<float> w = y", LINE_INFO());
		}

		TEST_METHOD(Vector2_Moves)
		{
			Pro::Vector2<float> x(1, 1);
			Pro::Vector2<float> y(std::move(x));
			Pro::Vector2<float> z = { 1.0f, 1.0f };
			Pro::Vector2<float> w = std::move(x);

			Assert::AreEqual(y, x, L"Vector<float> w = y", LINE_INFO());
			Assert::AreEqual(z, { 1.0f, 1.0f }, L"Vector<float> z(x)", LINE_INFO());
			Assert::AreEqual(w, x, L"Vector<float> w = y", LINE_INFO());
		}

		TEST_METHOD(Vector2_Equals)
		{
			Pro::Vector2<float> x(1, 0);
			Pro::Vector2<float> y(0, 1);
			Pro::Vector2<float> z(1, 1);

			Assert::AreEqual(x == y, false, L"Equals Error", LINE_INFO());
			Assert::AreEqual(y == x, false, L"Equals Error", LINE_INFO());
			Assert::AreEqual(x == z, false, L"Equals Error", LINE_INFO());
			Assert::AreEqual(z == x, false, L"Equals Error", LINE_INFO());
			Assert::AreEqual(y == z, false, L"Equals Error", LINE_INFO());
			Assert::AreEqual(z == y, false, L"Equals Error", LINE_INFO());
			Assert::AreEqual(x == x, true , L"Equals Error", LINE_INFO());
		}

		TEST_METHOD(Vector2_Contains)
		{
			Pro::Vector2<float> x(1, 0);
			Pro::Vector2<float> y(0, 1);
			Pro::Vector2<float> z(1, 1);
			 
		}
	};
}