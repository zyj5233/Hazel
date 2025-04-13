workspace "Hazel"		-- sln�ļ���
	architecture "x64"	

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

-- ������Ŀ¼:Debug-windows-x86_64��x86_64�Ǳ�׼64λ�ܹ���
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}		--����һ���յ�lua��
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"		--��GLWF��·������ձ�
 
include "Hazel/vendor/GLFW"		--��һ��GLWF��premake�������premake

project "Hazel"		--Hazel��Ŀ
	location "Hazel"--��sln�����ļ����µ�Hazel�ļ���
	kind "SharedLib"--dll��̬��
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- ���Ŀ¼��bin/debug-windows-x86_64/Hazel��
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")-- �м�Ŀ¼

	-- Ԥ����ͷ 
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"
	
	-- ����������h��cpp�ļ�
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	-- ����ͷ�ļ����Ա��ܹ����ļ���ȡ������Additional Include Directories�е�����
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"		--��֮ǰ����GLFW�ĵ�ַ��ӽ������б�
	}
	
	links 
 	{ 
 		"GLFW",
 		"opengl32.lib"			--windows�Դ���opengl��
	}
	-- �����windowϵͳ
	filter "system:windows"
		cppdialect "C++17"
		-- On:�������ɵ����п�ѡ����MTD,��̬����MSVCRT.lib��;
		-- Off:�������ɵ����п�ѡ����MDD,��̬����MSVCRT.dll��;������exe�ŵ���һ̨�������������dll�ᱨ��
		staticruntime "On"	
		systemversion "latest"	-- windowSDK�汾
		-- Ԥ����������
		defines{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL"
		}
		-- ����ú��ƶ�Hazel.dll�ļ���Sandbox�ļ����£�%{cfg.buildtarget.relpath}��dll�����·��������location����..�ǻص���һ��Ŀ¼���½���bin
		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	-- ��ͬ�����µ�Ԥ���岻ͬ
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	-- ͬ������spdlogͷ�ļ�
	includedirs{
		"Hazel/vendor/spdlog/include",
		"Hazel/src"
	}
	-- ����hazel
	links
	{
		"Hazel"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"              --������������debug����ʹ�ã�����#ifdef HZ_PLATFORM_WINDOWS
		symbols "On"                    --�������ڵ���

	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"                   --�������ڱ������Ż�����������

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"
