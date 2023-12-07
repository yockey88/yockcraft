include "externals.lua"

workspace "yockcraft"
    startproject "runtime"
    architecture "x64"
    configurations {
        "Debug" ,
        "Release"
    }

    startproject "yockcraft"

    language "C++"
    cppdialect "C++latest"
    staticruntime "on"

    flags { "MultiProcessorCompile" }

    defines {
        "_CRT_SECURE_NO_WARNINGS" ,
        "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING" ,
    }

    filter "action:vs*"
        linkoptions { "/ignore:4099" }
        disablewarnings { "68" }

    filter "configurations:Debug"
        optimize "Off"
        symbols "On"

    filter { "system:windows" , "configurations:Debug" }
        sanitize { "Address" }
        flags { "NoRuntimeChecks"}
        defines { "NOMINMAX" }

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

    filter "system:windows"
        buildoptions { "/EHsc" , "/Zc:preprocessor" , "/Zc:__cplusplus" }

tdir = "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
odir = "%{wks.location}/bin_obj/%{cfg.buildcfg}/%{prj.name}"

group "External"
include("external/glad")
include("external/spdlog")
include("external/imgui")
include("external/msdf-atlas-gen")
group ""

group "Yockcraft"
include "./yockcraft"
group ""
