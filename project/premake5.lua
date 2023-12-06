include "./postbuild.lua"

projectname = "project"

project (projectname)
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    staticruntime "on"

    targetdir(tdir)
    objdir(odir)

    files {
        "./src/**.cpp",
        "./include/**.hpp"
    }

    includedirs { "include/" }

    filter "system:windows"
        systemversion "latest"

    filter { "system:linux" }
        result , err = os.outputof("pkg-config --cflags --libs gtk+-3.0")
        linkoptions { result }

    filter "configurations:Debug"
        symbols "on"

        ProcessDependencies("Debug")

    filter "configurations:Release"
        optimize "on"
        symbols "off"

        ProcessDependencies("Release")

    RunPostBuildCommands()
