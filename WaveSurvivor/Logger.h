#pragma once
#include <iostream>
#include <sstream>

class Logger
{
public:
	enum LogLevel {
		DEBUG,
		ERROR,
		PRINT
	};

	template <typename... Args>
	static void Log(LogLevel logLevel, const Args&... args);
	
	template<typename ...Args>
	static void Debug(const Args & ...args);

private:
	template<typename T, typename... Args>
	static void LogHelper(std::ostringstream& oss, const T& first, const Args&... args);

	template<typename T>
	static void LogHelper(std::ostringstream& oss, const T& last);
};

template<typename ...Args>
inline void Logger::Log(LogLevel logLevel, const Args & ...args)
{
	std::ostringstream oss;
	LogHelper(oss, args...);
	switch (logLevel) {
	case DEBUG:
		std::cout << "DEBUG: " << oss.str() << std::endl;
		break;
	case ERROR:
		std::cerr << "ERROR: " << oss.str() << std::endl;
		break;
	case PRINT:
		std::cout << oss.str() << std::endl;
		break;
	default:
		std::cerr << "Log level not implemented!" << std::endl;
	}
}

template<typename ...Args>
inline void Logger::Debug(const Args & ...args)
{
	std::ostringstream oss;
	LogHelper(oss, args...);
	std::cout << "DEBUG: " << oss.str() << std::endl;
}

template<typename T, typename ...Args>
inline void Logger::LogHelper(std::ostringstream& oss, const T& first, const Args & ...args)
{
	oss << first << " ";
	LogHelper(oss, args...);
}

template<typename T>
inline void Logger::LogHelper(std::ostringstream& oss, const T& last)
{
	oss << last;
}