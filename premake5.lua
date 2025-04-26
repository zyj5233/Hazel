workspace "Hazel"		-- sln文件名
	architecture "x64"	

	configurations{
		"Debug",
		"Release",
		"Dist"
	}
		-- 启动项目
	startproject "Sandbox"

-- 组成输出目录:Debug-windows-x86_64（x86_64是标准64位架构）
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}		--创建一个空的lua表
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"		--把GLWF的路径填进空表
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"
IncludeDir["glm"] = "Hazel/vendor/glm"

include "Hazel/vendor/GLFW"		--把一个GLWF的premake导入这个premake
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"		--Hazel项目
	location "Hazel"--在sln所属文件夹下的Hazel文件夹
	kind "StaticLib"--dll动态库
	language "C++"
	cppdialect "C++17"
	staticruntime "on"	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 输出目录（bin/debug-windows-x86_64/Hazel）
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")-- 中间目录

	-- 预编译头 
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"
	
	-- 包含的所有h和cpp文件
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	-- 包含头文件，以便能够跨文件读取，就是Additional Include Directories中的配置
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",		--把之前表里GLFW的地址添加进搜索列表
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
 		"%{IncludeDir.glm}"
	}
	
	links 
 	{ 
 		"GLFW",
		"Glad",
		"ImGui",
 		"opengl32.lib"			--windows自带的opengl库
	}
	-- 如果是window系统
	filter "system:windows"
		-- On:代码生成的运行库选项是MTD,静态链接MSVCRT.lib库;
		-- Off:代码生成的运行库选项是MDD,动态链接MSVCRT.dll库;打包后的exe放到另一台电脑上若无这个dll会报错

		systemversion "latest"	-- windowSDK版本
		-- 预处理器定义
		defines{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	-- 不同配置下的预定义不同
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	-- 同样包含spdlog头文件
	includedirs{
		"Hazel/vendor/spdlog/include",
		"Hazel/src",
		"Hazel/vendor",
		"%{IncludeDir.glm}"
	}
	-- 引用hazel
	links
	{
		"Hazel"
	}

	filter "system:windows"

		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"              --过滤器，仅在debug配置使用，类似#ifdef HZ_PLATFORM_WINDOWS
		runtime "Debug"
		symbols "on"                    --开启用于调试

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"                   --开启用于编译器优化，提升性能

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"
