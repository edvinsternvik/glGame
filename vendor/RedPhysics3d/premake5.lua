project "RedPhysics3d"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"RedPhysics3d/**.cpp",
		"RedPhysics3d/**.h"
	}