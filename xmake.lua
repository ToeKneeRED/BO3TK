set_project("Black Ops III Toe Knee")
set_version("0.0.1")

set_languages("c11", "cxx23")
set_policy("build.ccache", false)
set_policy("package.requires_lock", false)

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

if(is_plat("windows")) then
    add_cxflags("/bigobj")
    set_arch("x64")

    -- :(
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

includes("Tools")
includes("BO3TK")
includes("Dashboard")

set_defaultmode("releasedbg")

function copy_to_common(operation)
    return function(target)
        local operations = target:extraconf("rules", "copy_to_common", "operations") or {}
        if table.contains(operations, operation) then
            local destination = path.join(os.projectdir(), "common")
            os.cp(target:targetfile(), destination)
            cprint("${bright green}[%s:%s] Copied %s to %s${clear}",
                target:name(), operation, target:filename(), destination)
        end
    end
end

rule("copy_to_common") -- ex. add_rules("copy_to_common", {"operations", {"after_build"}})
    on_build(copy_to_common("on_build"))
    before_build(copy_to_common("before_build"))
    after_build(copy_to_common("after_build"))
    before_clean(copy_to_common("before_clean"))
    after_clean(copy_to_common("after_clean"))
rule_end()
