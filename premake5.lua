workspace "glGame"
	architecture "x64"
	startproject "glGame"

	configurations { "Debug", "Release" }

include "vendor/GLEW"
include "vendor/GLFW"

project "glGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	systemversion "latest"

	targetdir "bin/%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.buildcfg}"

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"vendor/GLEW/include",
		"vendor/GLFW/include"
	}

	links {
		"GLEW",
		"GLFW",
		"opengl32.lib"
	}

	filter "system:linux"
		linkoptions { "-lX11", "-ldl", "-lGL", "-lpthread" }

	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		optimize "On"
		runtime "Release"