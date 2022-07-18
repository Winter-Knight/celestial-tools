#include <chrono>

inline long long nowMicro()
{
	namespace stdtime = std::chrono;
	stdtime::system_clock::time_point tp = stdtime::system_clock::now();
	stdtime::system_clock::duration   d  = tp.time_since_epoch();
	stdtime::microseconds             us = stdtime::duration_cast<stdtime::microseconds> (d);
	return us.count();
}

inline std::chrono::duration<float> getDelta()
{
	namespace stdtime = std::chrono;
	static stdtime::steady_clock::time_point lastTime = stdtime::steady_clock::now();
	stdtime::steady_clock::time_point now = stdtime::steady_clock::now();
	stdtime::duration<float> d = now - lastTime;
	lastTime = now;
	return d;
}
