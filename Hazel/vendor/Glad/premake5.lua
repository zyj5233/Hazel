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
 	    "include"       -- 为了glad.c直接#include <glad/glad.h>，而不用#include <include/glad/glad.h>
 	}
     
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"                    --开启用于调试

	filter "configurations:Release"
		runtime "Release"
		optimize "on"                   --开启用于编译器优化，提升性能

