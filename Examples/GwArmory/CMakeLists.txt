
# TODO: remove those imgui files and use the updated ones directly

add_library(GwArmory SHARED
	main.cpp 
	ArmorsDatabase.h
	imgui_impl_dx9.cpp
	imgui_impl_dx9.h
	ImGuiAddons.cpp
	ImGuiAddons.h)
# target_include_directories(GwArmory "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(GwArmory gwca imgui)
