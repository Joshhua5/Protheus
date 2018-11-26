#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Pro;
using namespace Util;


volatile bool flag = true;
const unsigned PIPE_COUNT = 1000000;

TEST_CLASS(PIPE_TEST) {

	static bool creator(Pipe<unsigned>* q) {
		while (flag);
		for (unsigned i = 0; i < PIPE_COUNT; ++i)
			if (q->Push(i) == false)
				--i;
		return true;
	}

	static bool consumor(Pipe<unsigned>* q, Timer<std::chrono::microseconds>* timer) {
		while (flag);
		timer->Tick();
		for (unsigned i = 0; i < PIPE_COUNT; ++i) {
			unsigned value;
			if (q->TopPop(value) == false) {
				--i;
				continue;
			}
			if (value != i)
				Assert::IsTrue(false, (std::to_wstring(value) + L" expected " + std::to_wstring(i)).data(), LINE_INFO());
		}
		timer->Tick();
		return true;
	}

	TEST_METHOD(PERFORMANCE) {
		unsigned long long time = 0;
		Pipe<unsigned> q(PIPE_COUNT);
		flag = true;

		std::thread(&PIPE_TEST::creator, &q).detach();
		Util::Timer<std::chrono::microseconds> timer;
		auto con = std::thread(&PIPE_TEST::consumor, &q, &timer);
		flag = false;

		// The consumer will tick once finished to remove the .join overhead
		con.join();
		time = timer.GetTickDelta();
	}

	TEST_METHOD(EXPECTED_ORDER_TEST)
	{
		Pipe<unsigned> values(128);
		for (unsigned x = 0; x < 63; ++x) {
			values.Push(x);
		}

		for (unsigned x = 0; x < 10; ++x) {
			unsigned value;
			values.TopPop(value);
		}

		for (unsigned x = 0; x < 5; ++x)
			values.Push(x);

		for (unsigned x = 10; x < 20; ++x) {
			unsigned value;
			values.TopPop(value);
			Assert::IsTrue(x == value,
				(L"Unexpected value returned: " + std::to_wstring(x) + L", expected" + std::to_wstring(value)).data(),
				LINE_INFO());

		}
	}
};
