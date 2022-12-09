#pragma once

#include <iomanip>
#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	bool started;

public:
	void start()
	{
		m_start = std::chrono::steady_clock::now();
		started = true;
	}

	void finish()
	{
		if (started)
		{
			auto end = std::chrono::steady_clock::now();
			std::cout << std::endl << "Execution time: " << std::setprecision(2) << std::chrono::duration <double, std::milli>(end - m_start).count() << " ms" << std::endl;
			started = false;
		}
	}

	void cancel()
	{
		started = false;
	}

	Timer(bool start = true)
		:m_start(std::chrono::steady_clock::now()), started(start)
	{}

	~Timer()
	{
		finish();
	}

};
