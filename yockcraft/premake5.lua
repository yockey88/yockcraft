local yockcraft = {}
yockcraft.name = "yockcraft"
yockcraft.path = "./yockcraft"
yockcraft.kind = "StaticLib"
yockcraft.language = "C++"
yockcraft.cppdialect = "C++latest"

yockcraft.files = function()
  files {
    "./src/**.cpp" ,
    "./src/**.hpp"
  }
end

yockcraft.include_dirs = function()
  includedirs { "./src/" }
end

yockcraft.windows_configurations = function()
  files {}
  includedirs {}
  systemversion "latest"
  entrypoint "WinMainCRTStartup"
end

AddProject(yockcraft)
