workspace "glGame"
	architecture "x64"
	startproject "glGame"

	configurations { "Debug", "Release" }

include "vendor/GLEW"
include "vendor/GLFW"
include "vendor/imgui"
include "vendor/lua"
include "vendor/angelscript"
include "vendor/RedPhysics3d"

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
		"vendor/GLFW/include",
		"vendor/glm/include",
		"vendor",
		"vendor/angelscript/include",
		"vendor/angelscript/add_on",
		"vendor/RedPhysics3d"
	}

	links {
		"GLEW",
		"GLFW",
		"ImGui",
		"lua",
		"angelscript",
		"RedPhysics3d"
	}

	defines "GLEW_STATIC"

	filter "system:windows"
		links "opengl32.lib"

	filter "system:linux"
		linkoptions { "-lX11", "-ldl", "-lGL", "-lpthread" }

	filter "configurations:Debug"
		defines "GL_GAME_EDITOR"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "GL_GAME_GAME"
		optimize "On"
		runtime "Release"