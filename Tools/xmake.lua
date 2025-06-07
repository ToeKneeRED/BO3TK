set_languages("cxx23")

target("Tools")
    set_kind("static")

    add_packages("spdlog", {public = true, wchar = true})

    add_headerfiles("include/*.h")
    add_includedirs("include")
    add_files("include/*.asm")

    add_rules("copy_to_common", {operations = {"after_build"}})