project(aakaar VERSION 0.1 LANGUAGES CXX)

add_library(aakaar SHARED
	include/Aakaar.h
	include/detail/Api.h
	src/Aakaar.cpp
)

set_target_properties(aakaar
PROPERTIES
	DEFINE_SYMBOL "AMC_TEXTURE_EXPORT"
)

target_include_directories(aakaar
PUBLIC
	include
)

target_link_libraries(aakaar
PRIVATE
	gdi32.lib
	kernel32.lib
)

add_subdirectory(examples)