-- Build modes
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")

-- Echo compiler warnings
set_warnings("allextra")
set_policy("build.warning", true)

-- Automatically build before run
set_policy("run.autobuild", true)

-- CCache
set_policy("build.ccache", false)

-- Address sanitizer
set_policy("build.sanitizer.address", false)

-- Link-time optimization
set_policy("build.optimization.lto", false)

-- Dependencies
add_requires("fmt", "glfw", "sqlite3", "libsodium")
add_requires("imgui docking", {configs = {glfw_opengl3 = true}})

-- Executable
target("PassDepot")
    set_kind("binary")
    set_languages("cxx20")
    add_files("Source/*.cpp")
    add_includedirs("Include")
    add_packages("fmt", "glfw", "sqlite3", "libsodium", "imgui")
