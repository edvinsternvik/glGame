project "angelscript"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"source/*.h",
		"source/*.cpp",
		"source/*.hpp",
		"include/angelscript.h"
	}

	configuration { "linux" }
		buildoptions { "-fno-strict-aliasing" }