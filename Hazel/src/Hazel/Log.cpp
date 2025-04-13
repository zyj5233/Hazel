#include "hzpch.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {
	// 由于这两属性是静态的，得在cpp里定义，不然报无法解析的外部符号错误
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	void Log::Init()
	{
		// 自定义日志格式：%^颜色起始点、%T时间戳(HH:MM:SS)、%n记录员的名字、%v实际的文本、%$颜色结束点
		spdlog::set_pattern("%^[%T] %n: %v%$");			//这是输出的格式
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");// 创建支持多线程（_mt）的安全控制台日志器
		s_CoreLogger->set_level(spdlog::level::trace);	//这个日志器的级别为最低，会记录高于它的级别，即所有级别的日志消息

		s_ClientLogger = spdlog::stdout_color_mt("APP");// APP 对应自定义格式的%n
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
