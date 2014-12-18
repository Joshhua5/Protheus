
#include <iostream>

class B {
public:
	B() {
		std::cout << "O noes";
	}
	  
	~B() {
		std::cout << "deleted D:";
	}

	B(B&&) {
		std::cout << "Opps I've done it again";
	}

	B operator=(B&&) {
		std::cout << "I WAS MOVED O NOES";
	}
};

class A : B { 
public:
	int c;
	A() { c = 1; }

	A(A&& d) {
		c = d.c;
	}

	A operator=(A&& d) { c = d.c; }

	~A() { }

};
 

int main() {
	{
		A a;
		A f(std::move(a));
		A b = std::move(a);
	}
	system("pause");
}