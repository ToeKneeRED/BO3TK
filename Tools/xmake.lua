target("Tools")
    set_kind("headeronly")

    add_packages("spdlog", {public = true, wchar = true})

    add_headerfiles("include/*.h", {public = true})
    add_includedirs("include", {public = true})