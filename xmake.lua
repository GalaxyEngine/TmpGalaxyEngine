add_rules("mode.release", "mode.debug")
add_rules("plugin.vsxmake.autoupdate")

if is_plat("windows") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
end

add_requires("imgui v1.89.9-docking", { configs = { glfw_opengl3 = true } })
add_requires("glad")
add_requires("stb")
--add_requires("rttr")

-- enable features
add_defines("ENABLE_MULTITHREAD")
--add_defines("ENABLE_REFLECTION")

set_languages("c++20")

set_rundir("GalaxyCore")
set_targetdir("GalaxyCore")

-- diable warnings
add_cxflags("/wd4251", {tools = "cl"}) -- class needs to have dll-interface to be used by clients of class
add_defines("_CRT_SECURE_NO_WARNINGS")
add_cxflags("-Wall")            -- Enable all commonly used warning flags

target("GalaxyEngine")
    set_symbols("debug")
    set_kind("shared")

    -- set include dirs
    add_includedirs("GalaxyEngine/include")

    add_defines("GALAXY_EXPORTS")
    
    if (is_plat("windows", "msvc")) then 
        add_cxflags("/permissive")
    end

    -- Includes --
    add_headerfiles("GalaxyEngine/include/**.h");
    add_headerfiles("GalaxyEngine/include/**.inl");
    add_files("GalaxyEngine/src/**.cpp")
    set_pcxxheader("GalaxyEngine/include/pch.h")
    
    -- Packages --
    add_packages("glfw")
    add_packages("imgui")
    add_packages("glad")
    add_packages("stb")

    if is_plat("mingw") then
        -- for file dialog in Windows mingw
        add_links("comdlg32")
    end
    --add_packages("rttr")
target_end()

target("GalaxyCore")
    set_default(true)
    set_kind("binary")
    add_deps("GalaxyEngine")
    add_files("GalaxyCore/**.cpp")
    add_includedirs("GalaxyEngine/include")
target_end()