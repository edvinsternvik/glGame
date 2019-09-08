project "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"./**.cpp",
		"./**.h"
	}

	includedirs {
		"../GLFW/include/",
		"../GLEW/include"
	}