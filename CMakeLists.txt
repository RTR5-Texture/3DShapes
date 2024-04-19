cmake_minimum_required(VERSION 3.2)

project(aakaar)

# Set string for platform specific subdirectories and settings
if(WIN32)
	set(AMC_PLATFORM_NAME "win" CACHE STRING "Target platform ('win', 'mac' or 'linux')")
elseif(APPLE)
	set(AMC_PLATFORM_NAME "mac" CACHE STRING "Target platform ('win', 'mac' or 'linux')")
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	set(AMC_PLATFORM_NAME "linux" CACHE STRING "Target platform ('win', 'mac' or 'linux')")
else()
	message(FATAL_ERROR "Unsupported platform")
endif()

# enable testing -- required for running unit tests
enable_testing()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	option(USE_PCH "Build using precompiled headers" ON)
else()
	option(USE_PCH "Build using precompiled headers" OFF)
endif()

# include the build config
include(cmake/BuildConfig.cmake)

# disable WIN32 GUI application by default
# applications that want to build as WIN32 application should use the WIN32 flag for add_executable()
set(CMAKE_WIN32_EXECUTABLE ON)

add_subdirectory(libraries)