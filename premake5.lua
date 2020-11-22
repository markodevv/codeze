workspace "codeze"

configurations {"Debug", "Release"}



project "codeze"

    kind "ConsoleApp"
	language "C"
	staticruntime "on"

	files {"code/**.c", "code/**.h"}

	targetdir "bin/"
	objdir "bin/"


	filter "system:windows"

	    links {"opengl32.lib"}

		defines {
		   "WINDOWS_PLATFORM"
		}


	filter "system:linux"

--		buildoptions {"-g", "-fPIC"}

--		linkoptions {"-ldl", "-lGL", "-pthread"}

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
		   
