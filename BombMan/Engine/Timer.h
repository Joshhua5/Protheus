#pragma once
#include <chrono>
namespace Pro{

	class Timer
	{
	private:
		long long lastTick;
		long long currentTick;
		long long startUpdateTick;
		long long endUpdateTick;
		long long startRenderTick;
		long long endRenderTick;
		double high_resolution_clock_period;
	public:
		Timer();
		~Timer();

		double getDelta();
		double getUpdateDelta();
		double getRenderDelta();
		unsigned int getFPS();
		void tick();
		void startUpdate();
		void endUpdate();
		void startRender();
		void endRender();
	};

}