project "angelscript"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"source/src/*.h",
		"source/src/*.c",
		"source/src/*.hpp",
		"include/angelscript.h"
	}

	configuration { "linux" }
		buildoptions { "-fno-strict-aliasing" }