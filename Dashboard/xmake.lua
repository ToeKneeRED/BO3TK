
target("Dashboard")
    set_kind("binary")
    set_languages("cxx23")

    add_rules("qt.widgetapp", "qt.moc")

    local gamePath = get_config("game") or ""
    print("gamePath: " .. gamePath)
    if gamePath ~= "" then
        add_defines("GAME_PATH=R\"(" .. gamePath .. ")\"")
    end

    local dllPath = get_config("dll") or ""
    print("dllPath: " .. dllPath)
    if dllPath ~= "" then
        add_defines("DLL_PATH=R\"(" .. dllPath .. ")\"")
    end

    add_packages("qt5widgets", "qt5gui", "qt5core", "qt5base")

    add_files("src/*.cpp","src/widgets/*.cpp", "src/widgets/*.h")
    add_headerfiles("include/*.h", "src/widgets/*.h")
    add_includedirs("include", "resources", "src/widgets")

    add_deps("Tools")