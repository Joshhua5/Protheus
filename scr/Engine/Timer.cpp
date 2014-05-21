#include "Timer.h" 

using namespace Pro;
using namespace Util;
using namespace std::chrono;

#define getNS() high_resolution_clock::now().time_since_epoch().count()
#define period high_resolution_clock::period::den

void Timer::tick(){
	lastTick = currentTick;
	currentTick = getNS();
} 

double Timer::getTicksPerSec(){
	return static_cast<double>(period / (currentTick - lastTick));
}

double Timer::getTickDelta(){
	return static_cast<double>(currentTick - lastTick);
}

Timer::Timer(){ startTick = getNS(); }


Timer::~Timer(){} 