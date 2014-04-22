#include "Timer.h" 

namespace Pro{

	using namespace std::chrono;

#define getNS() high_resolution_clock::now().time_since_epoch().count()
#define period high_resolution_clock::period::den

	double Timer::getDelta(){
		return  (double) currentTick - (double) lastTick;
	}
	double Timer::getUpdateDelta(){
		return (double) period / (double) (startUpdateTick - endUpdateTick);
	}
	double Timer::getRenderDelta(){
		return (double) period / (double) (startRenderTick - endRenderTick);
	}

	void Timer::tick(){
		lastTick = currentTick;
		currentTick = getNS();
	}
	void Timer::startUpdate(){
		startUpdateTick = getNS();
	}
	void Timer::endUpdate(){
		endUpdateTick = getNS();
	}
	void Timer::startRender(){
		startRenderTick = getNS();
	}
	void Timer::endRender(){
		endRenderTick = getNS();
	}

	unsigned int Timer::getFPS(){
		return (unsigned int) (period / (currentTick - lastTick));
	}

	Timer::Timer()
	{
	}


	Timer::~Timer()
	{
	}

}