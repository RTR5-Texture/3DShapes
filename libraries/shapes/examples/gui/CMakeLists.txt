add_executable(AakaarClientGUI WIN32)

target_sources(AakaarClientGUI
PRIVATE
OGL.cpp
OGL.h
vmath.h
OGL.rc
initial.ico)

target_include_directories(AakaarClientGUI
PRIVATE
${CMAKE_SOURCE_DIR}\\external\\glew\\include\\
)

target_link_directories(AakaarClientGUI
PRIVATE
${CMAKE_SOURCE_DIR}\\external\\glew\\lib\\Release\\x64\\)


target_link_libraries(AakaarClientGUI PRIVATE aakaar gdi32.lib user32.lib glew32.lib)