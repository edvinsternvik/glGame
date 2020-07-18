project "RedPhysics3d"
	kind "StaticLib"
	language "C++"
	systemversion "latest"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"RedPhysics3d/**.cpp",
		"RedPhysics3d/**.h"
	}

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"