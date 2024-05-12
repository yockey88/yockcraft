local terrain_gen_ex = {}
terrain_gen_ex.name = "terrain-gen-example"
terrain_gen_ex.path = "./examples/terrain"
terrain_gen_ex.kind = "ConsoleApp"
terrain_gen_ex.language = "C++"
terrain_gen_ex.cppdialect = "C++latest"

terrain_gen_ex.files = function()
  files { "./terrain/**.cpp" }
end

terrain_gen_ex.include_dirs = function()
  includedirs { "./terrain" }
end

terrain_gen_ex.windows_configurations = function()
  files {}
  includedirs {}
  systemversion "latest"
  entrypoint "WinMainCRTStartup"
end

terrain_gen_ex.components = {}
terrain_gen_ex.components["yockcraft"] = { "%{wks.location}/yockcraft/src" }

AddProject(terrain_gen_ex)
