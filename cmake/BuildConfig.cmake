include(CheckCXXCompilerFlag)
include(CheckCXXSymbolExists)

# set c++ standard to 17
set(CMAKE_CXX_STANDARD 17)

# disallow GNU extensions where-ever relevant
# Compiler extensions are compiler-specific behaviors that allow you to write programs that are incompatible with the C++ standard
set(CMAKE_CXX_EXTENSIONS FALSE)

# see: https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
# Use -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Non-windows platforms do not export symbols by default.
# Symbols from a shared library must be explicitly marked for export, as on Windows
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# see: https://cmake.org/cmake/help/latest/variable/CMAKE_COLOR_DIAGNOSTICS.html
# for colourful output
if(NOT DEFINED CMAKE_COLOR_DIAGNOSTICS)
	set(CMAKE_COLOR_DIAGNOSTICS TRUE)
endif()

# treat warning as error - for strict quality control of the code
#if(NOT DEFINED CMAKE_COMPILE_WARNING_AS_ERROR)
#	set(CMAKE_COMPILE_WARNING_AS_ERROR TRUE)
#endif()

if(MSVC)
	# see: https://learn.microsoft.com/en-us/cpp/build/reference/linker-options?view=msvc-170
	add_link_options(
		"LINKER:/WX"	# treat linker warnings as error
		"LINKER:/NXCOMPAT"	# Marks an executable as verified to be compatible with the Windows Data Execution Prevention feature.
	)

	# see: https://learn.microsoft.com/en-us/windows/win32/secbp/control-flow-guard
	# enable control flow guard checks
	add_compile_options("/guard:cf")
	add_link_options("LINKER:/GUARD:CF")

	# debug symbol generation
	#if(NOT DEFINED CMAKE_MSVC_DEBUG_INFORMATION_FORMAT)
	#	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "ProgramDatabase")
	#endif()

else()
	# non-windows platform settings

	# see: https://www.redhat.com/en/blog/enhance-application-security-fortifysource
	add_compile_definitions("_FORTIFY_SOURCE=2")

	if(NOT APPLE)
		# for security purpose
		# see: https://www.redhat.com/en/blog/hardening-elf-binaries-using-relocation-read-only-relro
		add_link_options(
			"LINKER: -z,relro"
			"LINKER:-z,noexecstack"
		)
	endif()

	add_compile_options(
		# detect use of undefined macros
		"-Wundef"
		
		# don't allow implicit fall through in switch statements (break visarla tar developer la orad)
		"-Wimplicit-fallthrough"
	)

	# turn of debug symbol generation
	#add_compile_options("$<$<NOT:$<CONFIG:Debug>>:-g>")

	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		add_compile_options(
			# warn if APPLE apis used are not supported on the platform
			"-Wunguarded-availability"

			"-Wreduntant-parens"

			"-Wuninitialized"

			# warn if loop has an implicit cast that create a copy
			"-Wrange-loop-construct"

			# dont allow public functions to be defined in cpp files with no corresponding declaration
			"-Wmissing-prototypes"
		)
	endif()

	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		add_compile_options(
			"-Wmissing-declarations"
			"-Wno-error=maybe-uninitialized"
			"-Wno-error=unused-result"
		)
	endif()
endif()

if(WIN32)
	add_compile_definitions(
		"NOMINMAX"
		"UNICODE"
		"_UNICODE"
		"WIN32_LEAN_AND_MEAN"
	)
endif()