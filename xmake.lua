set_project("Black Ops III Tool Kit")
set_version("0.0.1")

set_languages("c11", "cxx23")
set_policy("build.ccache", false)
set_policy("package.requires_lock", false)
add_defines("_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING")

if(is_plat("windows")) then
    set_arch("x64")

    add_syslinks("kernel32")
    add_syslinks("user32")
    add_syslinks("Gdi32")
    add_syslinks("Shell32")

    add_ldflags("-subsystem:windows", {force = true})
end

add_rules("mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

add_requires("spdlog v1.13.0", "minhook v1.3.3", "qt5base", "qt5core", "qt5gui", "qt5widgets", "imgui")

add_requireconfs("spdlog", {configs = {wchar = true}})
add_requireconfs("imgui", {configs = {dx11 = true, win32 = true}})

if is_mode("debug") then
    add_defines("DEBUG")
end

add_includedirs("Tools/include", {public = true})
add_files("common/**.cpp")
add_headerfiles("common/**.h")
add_filegroups("common", {rootdir = "../common"})

function add_recursive_includes(dir)
    add_includedirs(dir)
    local subdirs = os.dirs(dir .. "/*")
    for _, subdir in ipairs(subdirs) do
        add_recursive_includes(subdir)
    end
end
add_recursive_includes("common")

includes("Tools", "BO3TK", "Dashboard")

option("dll")
    set_showmenu(true)
    set_default("BO3TK.dll")
    set_description("Path to dll to inject")
option_end()
option("game")
    set_showmenu(true)
    set_default("BlackOps3.exe")
    set_description("Path to BlackOps3.exe")
option_end()