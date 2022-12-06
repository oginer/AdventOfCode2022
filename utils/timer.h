#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;

public:
	void start()
	{
		m_start = std::chrono::steady_clock::now();
	}

	void finish() const
	{
		auto end = std::chrono::steady_clock::now();

		std::cout << std::endl << "Execution time: " << std::setprecision(2) << std::chrono::duration <double, std::milli>(end - m_start).count() << " ms" << std::endl;
	}

	Timer()
		:m_start(std::chrono::steady_clock::now())
	{}

	~Timer()
	{
		finish();
	}

};
