require("ymake")

local config = {}
config.wks_name = "yockcraft"
config.architecture = "x64"
config.start_project = "yockcraft"
config.cpp_dialect = "C++latest"
config.static_runtime = "on"
config.target_dir = "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
config.obj_dir = "%{wks.location}/bin_obj/%{cfg.buildcfg}/%{prj.name}"

config.build_configurations = {
  "Debug",
  "Release"
}

config.platforms = {
  "Windows",
}

config.groups = {
  ["yockcraft"] = { "./yockcraft" },
  ["runtime"] = { "./yockcraft-runtime" },
  ["examples"] = { "./examples" }
}

local choc = {}
choc.name = "choc"
choc.include_dir = "%{wks.location}/external/choc"

local entt = {}
entt.name = "entt"
entt.include_dir = "%{wks.location}/external/entt"

local glad = {}
glad.name = "glad"
glad.path = "./external/glad"
glad.include_dir = "%{wks.location}/external/glad/include"
glad.lib_name = "glad"
glad.lib_dir = "%{wks.location}/bin/Debug/glad"

local glm = {}
glm.name = "glm"
glm.include_dir = "%{wks.location}/external/glm"

local gtest = {}
gtest.name = "gtest"
gtest.path = "./external/gtest"
gtest.include_dir = "%{wks.location}/external/gtest/googletest/include/gtest"
gtest.lib_name = "gtest"
gtest.lib_dir = "%{wks.location}/bin/Debug/gtest"

local imgui = {}
imgui.name = "imgui"
imgui.path = "./external/imgui"
imgui.include_dir = "%{wks.location}/external/imgui"
imgui.lib_name = "imgui"
imgui.lib_dir = "%{wks.location}/bin/Debug/imgui"

local magic_enum = {}
magic_enum.name = "magic_enum"
magic_enum.include_dir = "%{wks.location}/external/magic_enum"

local sdl2 = {}
sdl2.name = "sdl2"
sdl2.include_dir = "%{wks.location}/external/SDL2/SDL2"
sdl2.lib_dir = "%{wks.location}/external/SDL2/lib/%{cfg.buildcfg}"
sdl2.lib_name = "SDL2"
sdl2.debug_lib_name = "SDL2d"
sdl2.configurations = { "Debug", "Release" }

local spdlog = {}
spdlog.name = "spdlog"
spdlog.path = "./external/spdlog"
spdlog.include_dir = "%{wks.location}/external/spdlog/include"
spdlog.lib_name = "spdlog"
spdlog.lib_dir = "%{wks.location}/bin/Debug/spdlog"

local msdf_atlas_gen = {}
msdf_atlas_gen.name = "msdf_atlas_gen"
msdf_atlas_gen.path = "./external/msdf-atlas-gen"
msdf_atlas_gen.include_dor = "%{wks.location}/external/msdf-atlas-gen"
msdf_atlas_gen.lib_name = "msdf_atlas_gen"
msdf_atlas_gen.lib_dir = "%{wks.location}/bin/Debug/msdf-atlas-gen"

local stb = {}
stb.name = "stb"
stb.include_dir = "%{wks.location}/external/stb"

AddDependency(choc)
AddDependency(entt)
AddDependency(glad)
AddDependency(glm)
AddDependency(gtest)
AddDependency(imgui)
AddDependency(magic_enum)
AddDependency(sdl2)
AddDependency(spdlog)
AddDependency(msdf_atlas_gen)
AddDependency(stb)

CppWorkspace(config)
