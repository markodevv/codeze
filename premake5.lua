workspace "codeze"

configurations {"Debug", "Release"}
architecture "x86_64"
toolset "clang"

include "third_party/glfw/"
include "third_party/glad/"

IncludeDir = {}
IncludeDir["glfw"] = "third_party/glfw/include"
IncludeDir["glad"] = "third_party/glad/include"
IncludeDir["freetype"] = "third_party/freetype/include"


project "codeze"

toolset "clang"
    kind "ConsoleApp"
	language "C++"
	staticruntime "on"

	files {
	   "src/*.cpp",
	   "src/*.h"
	}

	includedirs {
	   "%{IncludeDir.glfw}",
	   "%{IncludeDir.freetype}",
	   "%{IncludeDir.glad}"
	}

	targetdir "bin/"
	objdir "bin/"

	defines {
	   "GLFW_INCLUDE_NONE"
	}

	filter "system:windows"

	    links {"glad", "glfw", "freetype.lib", "opengl32.lib"}

		defines {
		   "WINDOWS_PLATFORM"
		}


	filter "system:linux"

	    buildoptions {"-g", "-fPIC"}

	    links {"glad", "glfw", "GL", "freetype", "dl", "pthread", "m"}

		defines {
		   "LINUX_PLATFORM"
		}


	filter "configurations:Debug"
		   defines {"DEBUG"}
		   optimize "Off"
		   symbols "On"
	
	filter "configurations:Release"
		   defines {"NDEBUG"}
		   optimize "On"
		   
