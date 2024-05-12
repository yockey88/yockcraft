local runtime = {}
runtime.name = "yockcraft-runtime"
runtime.path = "./yockcraft-runtime"
runtime.kind = "ConsoleApp"
runtime.language = "C++"
runtime.cppdialect = "C++latest"

runtime.files = function()
  files { "./driver.cpp" }
end

runtime.include_dirs = function()
  includedirs {}
end

runtime.windows_configurations = function()
  files {}
  includedirs {}
  systemversion "latest"
  entrypoint "WinMainCRTStartup"
end

runtime.post_build_commands = function()
  filter { "system:windows" , "configurations:Debug" }
    postbuildcommands {
      '{COPY} "../external/mono/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"' ,
      '{COPY} "../external/mono/bin/mono-2.0-sgen.pdb" "%{cfg.targetdir}"' ,
      '{COPY} "../external/mono/bin/MonoPosixHelper.dll" "%{cfg.targetdir}"' ,
      '{COPY} "../external/mono/bin/MonoPosixHelper.pdb" "%{cfg.targetdir}"' ,
      '{COPY} "../external/SDL2/lib/Debug/SDL2d.dll" "%{cfg.targetdir}"' ,
    }

  filter { "system:windows" , "configurations:Release" }
    postbuildcommands {
      '{COPY} "../external/mono/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"' ,
      '{COPY} "../external/mono/bin/MonoPosixHelper.dll" "%{cfg.targetdir}"' ,
      '{COPY} "../external/SDL2/lib/Release/SDL2.dll" "%{cfg.targetdir}"' ,
    }
end

runtime.components = {}
runtime.components["yockcraft"] = { "%{wks.location}/yockcraft/src" }

AddProject(runtime)
