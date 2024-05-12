local msdf_atlas_gen = {}

msdf_atlas_gen.name = "msdf_atlas_gen"
msdf_atlas_gen.kind = "StaticLib"
msdf_atlas_gen.language = "C++"
msdf_atlas_gen.cppdialect = "C++latest"

msdf_atlas_gen.files = function() 
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
end

msdf_atlas_gen.include_dirs = function() 
  includedirs {
    "." ,
    "artery-font-format" ,
    "msdfgen" ,
    "msdfgen/core" ,
    "msdfgen-ext" ,
    "msdfgen-atlas-gen" ,
  }
end

msdf_atlas_gen.defines = function() 
  defines { "MSDFGEN_USE_LODEPNG" }
end

AddExternalProject(msdf_atlas_gen)