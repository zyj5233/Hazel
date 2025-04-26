project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
 
    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }
 
 	includedirs
 	{
 	    "include"       -- Ϊ��glad.cֱ��#include <glad/glad.h>��������#include <include/glad/glad.h>
 	}
     
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"                    --�������ڵ���

	filter "configurations:Release"
		runtime "Release"
		optimize "on"                   --�������ڱ������Ż�����������

