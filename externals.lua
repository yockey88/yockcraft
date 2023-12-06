include "premake/lua/ordered_pairs.lua"

function FirstToUpper(str)
    return (str:gsub("^%l", string.upper))
end

external = {
    --Example dependency ----
    -- Assimp = {
    --     include_dir = "%{wks.location}/external/assimp/include" ,
    --     debug_lib_name = "assimp-vc143-mtd" , -- ??? why wont this one link?
    --     lib_name = "assimp-vc143-mt" ,
    --     lib_dir = "%{wks.location}/external/assimp/lib/%{cfg.buildcfg}" ,
    --     configurations = "Debug" , "Release"
    -- } ,
}

function LinkDependency(table , debug , target)
    -- Setup library directory
    if table.lib_dir ~= nil then
    	libdirs { table.lib_dir }
    end

    -- Try linking
    local lib_name = nil
    if table.lib_name ~= nil then
    	lib_name = table.lib_name
    end

    if table.debug_lib_name ~= nil and debug and target == "Windows" then
        lib_name = table.debug_lib_name
    end

    if lib_name ~= nil then
    	links { lib_name }
    	return true
    end

    return false
end

function AddInclude(table)
    if table.include_dir ~= nil then
        externalincludedirs{ table.include_dir }
    end
end

function ProcessDependencies(configuration)
    local target = FirstToUpper(os.target())

    for key , lib_data in orderedPairs(external) do

        local matches_config = true

        if configuration ~= nil and lib_data.Configurations ~= nil then
            matches_config = string.find(lib_data.Configurations , configuration)
        end

        local is_debug = configuration == "Debug"

        if matches_config then
            local continue_link = true

            if lib_data[target] ~= nil then
                continue_link = not LinkDependency(lib_data[target] , is_debug , target)
                AddInclude(lib_data[target])
            end

            if continue_link then
                LinkDependency(lib_data , is_debug , target)
            end

            AddInclude(lib_data)
        end
    end
end

function IncludeDependencies(configuration)
    local target = FirstToUpper(os.target())

    for key , lib_data in orderedPairs(external) do

        local matches_config = true

        if configuration ~= nil and lib_data.Configurations ~= nil then
            matches_config = string.find(lib_data.Configurations , configuration)
        end

        if matches_config then
            AddInclude(lib_data)

            if lib_data[target] ~= nil then
                AddInclude(lib_data[target])
            end
        end

    end
end
