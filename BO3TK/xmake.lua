target("BO3TK")
    set_kind("shared")

    set_pcxxheader("include/pch.h")
    -- add_includedirs("include", {public = true})
    add_recursive_includes("include")

    add_files("src/**.cpp")
    add_headerfiles("include/**.h")
    add_extrafiles("src/*.asm")

    add_packages("minhook", "xbyak", "qt5base", "qt5core", "qt5gui", "qt5widgets", "imgui")

    add_deps("Tools")

    after_link(function (target)
        local file = target:targetfile()
        if file and os.isfile(file) then
            local dest = path.join(os.projectdir(), "common\\")
            os.cp(file, dest)
            cprint("${bright green}[%s] %s → %s${clear}", target:name(), file, dest)
        else
            cprint("${bright red}[%s] %s not found${clear}", target:name(), file or "nil")
        end
    end)