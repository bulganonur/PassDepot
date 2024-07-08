set(sources
    External/ImGui/Include/imgui.cpp
    External/ImGui/Include/imgui_demo.cpp
    External/ImGui/Include/imgui_draw.cpp
    External/ImGui/Include/imgui_widgets.cpp
    External/ImGui/Include/imgui_tables.cpp
    External/ImGui/Include/imgui_impl_opengl3.cpp
    External/ImGui/Include/imgui_impl_glfw.cpp
)

set(exe_sources
		Source/PassDepot.cpp
		${sources}
)
