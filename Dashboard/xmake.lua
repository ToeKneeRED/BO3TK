target("Dashboard")
    set_kind("binary")
    set_languages("cxx23")

    add_rules("qt.widgetapp", "qt.moc")

    add_packages("qt5widgets", "qt5gui", "qt5core", "qt5base")

    add_files("src/*.cpp","src/widgets/*.cpp", "resources/*.qrc")
    -- need to add_files qt-derived headers for moc to process
    add_files("src/widgets/*.h")
    add_headerfiles("include/*.h", "src/widgets/*.h")
    add_includedirs("include", "src/widgets")

    add_deps("Tools")

    local game = get_config("game")
    if game then
        add_defines("GAME_PATH=\"" .. game .. "\"")
    end

    local dll = get_config("dll")
    if dll then
        add_defines("DLL_PATH=\"" .. dll .. "\"")
    end