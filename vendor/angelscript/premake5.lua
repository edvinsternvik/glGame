project "angelscript"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"source/*.h",
		"source/*.cpp",
		"source/*.hpp",
		"include/angelscript.h",
		"add_on/**.cpp",
		"add_on/**.h"
	}

	includedirs {
		"include"
	}

	configuration { "linux" }
		buildoptions { "-fno-strict-aliasing" }