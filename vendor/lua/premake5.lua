project "lua"
	kind "StaticLib"
	language "C"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"src/*.h",
		"src/*.c",
		"src/*.hpp"
	}