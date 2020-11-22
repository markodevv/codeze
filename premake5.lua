workspace "codeze"

configurations {"Debug", "Release"}

include "third_party/glfw/"
include "third_party/glad/"

IncludeDir = {}
IncludeDir["glfw"] = "third_party/glfw/include"
IncludeDir["glad"] = "third_party/glad/include"

project "codeze"

    kind "ConsoleApp"
	language "C"
	staticruntime "on"

	files {"code/**.c", "code/**.h"}

	includedirs {
	   "%{IncludeDir.glfw}",
	   "%{IncludeDir.glad}"
	}

	targetdir "bin/"
	objdir "bin/"

	defines {
	   "GLFW_INCLUDE_NONE"
	}

	filter "system:windows"

	    links {"glad", "glfw", "opengl32.lib"}

		defines {
		   "WINDOWS_PLATFORM"
		}


	filter "system:linux"

	buildoptions {"-g", "-fPIC"}

	    links {"glad", "glfw"}
		linkoptions {"-ldl", "-lGL", "-pthread"}

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
		   
