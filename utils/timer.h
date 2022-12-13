#pragma once

#include <iomanip>
#include <chrono>
#include <string>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_end;

	bool started{ false };
	bool print{ false };
	std::string name;

public:
	void start()
	{
		m_start = std::chrono::steady_clock::now();
		started = true;
		print = false;
	}

	void finish()
	{
		if (started)
		{
			m_end = std::chrono::steady_clock::now();
			if (name == "")
			{
				std::cout << std::endl;
				std::cout << "Execution time: ";
				std::cout << std::setprecision(2) << std::chrono::duration <double, std::milli>(m_end - m_start).count() << " ms" << std::endl << std::endl;
				print = false;
			}
			else print = true;
			started = false;
		}
	}

	void cancel()
	{
		started = false;
		print = false;
	}

	Timer(const std::string& str, bool start_now = true)
		:m_start(std::chrono::steady_clock::now()), name(str)
	{
		if (start_now) start();
	}

	Timer(const char* str, bool start_now = true) : Timer(std::string{ str }, start_now) {}

	Timer(bool start_now = true) : Timer("", start_now) {}

	~Timer()
	{
		finish();

		if (print)
		{
			std::cout << std::endl;
			std::cout << name << " execution time: ";
			std::cout << std::setprecision(2) << std::chrono::duration <double, std::milli>(m_end - m_start).count() << " ms" << std::endl;
		}
	}
};
