workspace "codeze"

toolset "clang"
configurations {"Debug", "Release"}

include "third_party/glfw/"
include "third_party/glad/"
include "third_party/tinyregex"
include "third_party/gc"

IncludeDir = {}
IncludeDir["glfw"] = "third_party/glfw/include"
IncludeDir["glad"] = "third_party/glad/include"
IncludeDir["freetype"] = "third_party/freetype/include"
IncludeDir["tinyregex"] = "third_party/tinyregex"
IncludeDir["gc"] = "third_party/gc/src"

project "codeze"

    kind "ConsoleApp"
	language "C"
	staticruntime "on"

	files {"src/**.c", "src/**.h"}

	includedirs {
	   "%{IncludeDir.glfw}",
	   "%{IncludeDir.freetype}",
	   "%{IncludeDir.tinyregex}",
	   "%{IncludeDir.gc}",
	   "%{IncludeDir.glad}"
	}

	targetdir "bin/"
	objdir "bin/"

	defines {
	   "GLFW_INCLUDE_NONE"
	}

	filter "system:windows"

	    links {"glad", "glfw", "freetype", "tinyregex", "gc", "opengl32.lib"}

		defines {
		   "WINDOWS_PLATFORM"
		}


	filter "system:linux"

	    buildoptions {"-g", "-fPIC"}

	    links {"glad", "glfw", "GL", "freetype", "gc", "dl", "tinyregex", "pthread", "m"}

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
		   
