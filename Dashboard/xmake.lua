target("Dashboard")
    set_languages("cxx23")
    set_default(true)

    add_rules("qt.widgetapp", "qt.moc")
    add_packages("qt5widgets", "qt5gui", "qt5core", "qt5base")

    add_files("src/*.cpp","src/components/**.cpp", "resources/*.qrc")
    -- need to add_files qt-derived headers for moc to process
    add_files("src/components/**.h")
    add_headerfiles("include/*.h", "src/components/**.h")
    add_includedirs("include")
    add_recursive_includes("src/components")

    add_deps("Tools")
    add_linkorders("Tools", "BO3TK")

    local game = get_config("game")
    if game then
        add_defines("GAME_PATH=\"" .. game .. "\"")
    end

    local dll = get_config("dll")
    if dll then
        add_defines("DLL_PATH=\"" .. dll .. "\"")
    end

    after_build(function (target)
        os.exec("windeployqt %s", target:targetfile())
    end)