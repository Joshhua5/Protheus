// Utilities_Test.cpp : Defines the entry point for the console application.
//  

#include <Vector2.h>
#include <Error.h>

using namespace Pro;

int main()
{
	Vector2<float> x(0, 1); 
	Vector2<float> y(0, 0);

	Vector2<float> z = y;
	Vector2<float> w = std::forward<Vector2<float>>(z);
	Vector2<float> h = std::forward<Vector2<float>>({ 1.f, 1.0f });
	 
	if (z != y)
		error.reportError("Vector<float> w = y");
	
	if (w != w)
		error.reportError("comparison against self");

	if (h != Vector2<float>(1.0f, 1.0f))
		error.reportError("Error in comparison");

	 	 
	Vector2<float> moveConst(std::move(x));
	Vector2<float> initalizerListEquals = { 1.0f, 1.0f };
	Vector2<float> moveEquals = std::move(x);

	if (moveConst != x)
		error.reportError("Move Constructor");

	if (initalizerListEquals != h)
		error.reportError("Initalizer Constructor");

	if (moveEquals != x)
		error.reportError("Move Operator");



	//Assert::AreEqual(y, x, L"Vector<float> w = y", LINE_INFO());
	//Assert::AreEqual(z, { 1.0f, 1.0f }, L"Vector<float> z(x)", LINE_INFO());
	//Assert::AreEqual(w, x, L"Vector<float> w = y", LINE_INFO());
	//Vector2<float> x(1, 0);
	//Vector2<float> y(0, 1);
	//Vector2<float> z(1, 1);

	//Assert::AreEqual(x == y, false, L"Equals Error", LINE_INFO());
	//Assert::AreEqual(y == x, false, L"Equals Error", LINE_INFO());
	//Assert::AreEqual(x == z, false, L"Equals Error", LINE_INFO());
	//Assert::AreEqual(z == x, false, L"Equals Error", LINE_INFO());
	//Assert::AreEqual(y == z, false, L"Equals Error", LINE_INFO());
	//Assert::AreEqual(z == y, false, L"Equals Error", LINE_INFO());

	//Pro::Vector2<float> x(1, 0);
	//Pro::Vector2<float> y(0, 1);
	//Pro::Vector2<float> z(1, 1);

}

