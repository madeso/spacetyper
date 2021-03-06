cmake_minimum_required(VERSION 3.1.0)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(spacetyper)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/osx-bundle.plist")

if(${APPLE})
  # disabling http://stackoverflow.com/questions/32626171/xcode-7-how-to-suppress-warning-overrides-a-member-function-but-is-not-marked
  # that apple-clang spews out for wx headers
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif()

# compiler flags
if(${MSVC})
	add_definitions(/D _CRT_SECURE_NO_WARNINGS)

	# character set to Not Set=ASCII/SBCS
  # http://public.kitware.com/Bug/view.php?id=12189
	add_definitions(/D _SBCS)
endif()

# force visual studio and the like to only have debug and release builds
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
endif()

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(euphoria)

include_directories(euphoria)
add_subdirectory(spacetyper)
