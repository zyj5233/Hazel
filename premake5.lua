workspace "Hazel"		-- sln�ļ���
	architecture "x64"	

	configurations{
		"Debug",
		"Release",
		"Dist"
	}
		-- ������Ŀ
	startproject "Sandbox"

-- ������Ŀ¼:Debug-windows-x86_64��x86_64�Ǳ�׼64λ�ܹ���
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}		--����һ���յ�lua��
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"		--��GLWF��·������ձ�
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"
IncludeDir["glm"] = "Hazel/vendor/glm"

include "Hazel/vendor/GLFW"		--��һ��GLWF��premake�������premake
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"		--Hazel��Ŀ
	location "Hazel"--��sln�����ļ����µ�Hazel�ļ���
	kind "StaticLib"--dll��̬��
	language "C++"
	cppdialect "C++17"
	staticruntime "on"	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- ���Ŀ¼��bin/debug-windows-x86_64/Hazel��
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")-- �м�Ŀ¼

	-- Ԥ����ͷ 
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"
	
	-- ����������h��cpp�ļ�
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
	
	-- ����ͷ�ļ����Ա��ܹ����ļ���ȡ������Additional Include Directories�е�����
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",		--��֮ǰ����GLFW�ĵ�ַ��ӽ������б�
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
 		"%{IncludeDir.glm}"
	}
	
	links 
 	{ 
 		"GLFW",
		"Glad",
		"ImGui",
 		"opengl32.lib"			--windows�Դ���opengl��
	}
	-- �����windowϵͳ
	filter "system:windows"
		-- On:�������ɵ����п�ѡ����MTD,��̬����MSVCRT.lib��;
		-- Off:�������ɵ����п�ѡ����MDD,��̬����MSVCRT.dll��;������exe�ŵ���һ̨�������������dll�ᱨ��

		systemversion "latest"	-- windowSDK�汾
		-- Ԥ����������
		defines{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	-- ��ͬ�����µ�Ԥ���岻ͬ
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
	-- ͬ������spdlogͷ�ļ�
	includedirs{
		"Hazel/vendor/spdlog/include",
		"Hazel/src",
		"Hazel/vendor",
		"%{IncludeDir.glm}"
	}
	-- ����hazel
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
		defines "HZ_DEBUG"              --������������debug����ʹ�ã�����#ifdef HZ_PLATFORM_WINDOWS
		runtime "Debug"
		symbols "on"                    --�������ڵ���

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"                   --�������ڱ������Ż�����������

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"
