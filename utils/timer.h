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

	std::string format(auto t)
	{
		std::string result;
		double val;

		val = std::chrono::duration <double>(t).count();
		if (val > 60)
		{
			unsigned ival = static_cast<unsigned>(val);
			if (ival % 60 == 0 || val > 30 * 60)
				result = std::format("{} m", ival / 60);
			else
				result = std::format("{} m, {} s", ival / 60, ival % 60);
		}
		else if (val > 1)
			result = std::format("{:.1f} s", val);
		else
		{
			val = std::chrono::duration <double, std::milli>(t).count();
			if (val > 0.2) result = std::format("{:.3g} ms", val);
			else
			{
				val = std::chrono::duration <double, std::micro>(t).count();
				result = std::format("{:.3g} us", val);
			}
		}

		return result;
	}

public:
	void start()
	{
		m_start = std::chrono::steady_clock::now();
		started = true;
		print = false;
	}

	void finish(bool force_print = false)
	{
		if (started)
		{
			m_end = std::chrono::steady_clock::now();
			if (name == "" || force_print)
			{
				std::cout << std::endl;
				std::cout << name << ((name == "") ? "Execution time: " : " execution time: ");
				std::cout << format(m_end - m_start) << std::endl << std::endl;
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
			std::cout << format(m_end - m_start) << std::endl;
		}
	}
};
