add_rules("mode.debug", "mode.release")
set_defaultmode("debug")

add_requires("fmt", "glfw", "sqlite3", "libsodium")
add_requires("imgui docking", {configs = {glfw_opengl3 = true}})

target("PassDepot")
    set_kind("binary")
    set_languages("c++20")
    add_files("Source/*.cpp")
    add_includedirs("Include")
    add_packages("fmt", "glfw", "sqlite3", "libsodium", "imgui")
