set(sources
    External/ImGui/Source/imgui.cpp
    External/ImGui/Source/imgui_demo.cpp
    External/ImGui/Source/imgui_draw.cpp
    External/ImGui/Source/imgui_widgets.cpp
    External/ImGui/Source/imgui_tables.cpp
    External/ImGui/Source/imgui_impl_opengl3.cpp
    External/ImGui/Source/imgui_impl_glfw.cpp
    Source/PassDepot.cpp
    Source/PDWindow.cpp
    Source/PDGUI.cpp
    Source/PDDatabase.cpp
)

set(exe_sources
		Source/main.cpp
		${sources}
)
