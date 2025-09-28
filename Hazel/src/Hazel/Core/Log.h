#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Hazel {
	class HAZEL_API Log {
	public:
		static void Init();// 初始化
		// 向外提供获取日志对象
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }//正常写代码都这样操作，成员变量自己隐藏，封装函数对外使用
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }//静态成员函数无需创建实例直接调用，因为日志是全局基础设施
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;// Hazel项目的日志对象
		static std::shared_ptr<spdlog::logger> s_ClientLogger;// Sandbox项目的日志对象
	};
}

// Core log macros
// ...是接受函数参数包，__VA_ARGS__转发函数参数包
#define HZ_CORE_TRACE(...)    ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)	//...表示可以传递任意数量参数，__VA_ARGS__把所有传入的参数传给trace方法
#define HZ_CORE_INFO(...)     ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)     ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)    ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define HZ_CORE_FATAL(...)    ::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__) 

// Client log macros
#define HZ_TRACE(...)	      ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)	      ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)	      ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)	      ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
//#define HZ_FATAL(...)	      ::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__)
