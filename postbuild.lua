function RunPostBuildCommands()
    filter { "system:windows" , "configurations:Debug" }
        postbuildcommands {
            '{COPY} "../external/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"' ,
            '{COPY} "../external/mono/bin/Debug/mono-2.0-sgen.pdb" "%{cfg.targetdir}"' ,
            '{COPY} "../external/mono/bin/Debug/MonoPosixHelper.dll" "%{cfg.targetdir}"' ,
            '{COPY} "../external/mono/bin/Debug/MonoPosixHelper.pdb" "%{cfg.targetdir}"' ,
            '{COPY} "../external/SDL2/lib/Debug/SDL2d.dll" "%{cfg.targetdir}"' ,
        }

    filter { "system:windows" , "configurations:Release" }
        postbuildcommands {
            '{COPY} "../external/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"' ,
            '{COPY} "../external/mono/bin/Release/MonoPosixHelper.dll" "%{cfg.targetdir}"' ,
            '{COPY} "../external/SDL2/lib/SDL2.dll" "%{cfg.targetdir}"' ,
        }

end
