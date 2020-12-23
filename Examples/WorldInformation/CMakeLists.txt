
add_library(WorldInformation SHARED 
	main.cpp
	imgui_impl_dx9.h
	imgui_impl_dx9.cpp)
target_link_libraries(WorldInformation gwca imgui)
