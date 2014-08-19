#include "Timer.h"

using namespace Pro;
using namespace Util;
using namespace std::chrono;

#define getNS() duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count()
#define period high_resolution_clock::period::den

void Timer::tick(){
	lastTick = currentTick;
	// We reduce the accuracy of the timer by 1NS to prevent division by 0
	currentTick = getNS();
}

double Timer::getTicksPerSec(){
	return (double)high_resolution_clock_period / ((currentTick - lastTick) + 1);
}

unsigned long long Timer::getTickDelta(){
	return currentTick - lastTick;
}

Timer::Timer(){ 
	currentTick = getNS();
	tick();  
	high_resolution_clock_period = period;
}


int Timer::lGetTickDelta(lua_State* L){
	static const auto t = Util::luaP_touserdata<Timer>(L, 1);
	luaP_pushnumber(L, t->getTickDelta()); 
	return 1;
}

int Timer::lGetTicksPerSec(lua_State* L){
	static const auto t = Util::luaP_touserdata<Timer>(L, 1);
	luaP_pushnumber(L, t->getTicksPerSec());
	return 1;
}