#pragma once

#include <chrono>

namespace Time {


	using clock = std::chrono::high_resolution_clock;

	using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	//Durations measure timespans -> min, hr, ms
	using floatSeconds = std::chrono::duration<float>;


	class ScopeTimer {
	public:
		ScopeTimer(float* output) : StartTime(clock::now()), _RetVal(output) {}
		~ScopeTimer() {
			EndTime = Time::clock::now();
			Time::floatSeconds duration = EndTime - StartTime;
			*_RetVal = duration.count();
		}
	private:
		Time::timePoint StartTime;
		Time::timePoint EndTime;
		float* _RetVal;
	};
}