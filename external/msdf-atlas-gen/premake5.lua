project "msdf"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    targetdir(tdir)
    objdir(odir)

    files {
        "artery-font-format/**.h" ,
        "artery-font-format/**.hpp" ,
        "msdfgen/**.h" ,
        "msdfgen/**.hpp" ,
        "msdfgen/**.cpp" ,
        "msdf-atlas-gen/**.h" ,
        "msdf-atlas-gen/**.hpp" ,
        "msdf-atlas-gen/**.cpp" ,
    }

    includedirs {
        "." ,
        "artery-font-format" ,
        "msdfgen" ,
        "msdfgen/core" ,
        "msdfgen-ext" ,
        "msdfgen-atlas-gen" ,
    }

    defines {
        "MSDFGEN_USE_LODEPNG" ,
    }

    filter { "system:windows" }
        systemversion "latest"
 
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        symbols "off"
        optimize "on"
