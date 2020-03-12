project "angelscript"
	kind "StaticLib"
	language "C++"
	cppdialect "c++17"
	systemversion "latest"

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

	configuration { "windows"}
		files {
			"source/as_callfunc_x64_msvc_asm.asm"
		}

	filter "configurations:Debug"
		defines "GL_GAME_EDITOR"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "GL_GAME_GAME"
		optimize "On"
		runtime "Release"