file(GLOB app_src_glob *.cpp;*.cc;*.h;*.inl)

find_package(SDL2 REQUIRED)
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})

set(app_src ${app_src_glob})
source_group("" FILES ${app_src})

set(all_src ${app_src} ${pbjson_src} ${src_glew})

add_executable(spacetyper WIN32 MACOSX_BUNDLE ${all_src})
set_target_properties(spacetyper
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/dist"
                      DEBUG_OUTPUT_NAME "spacetyper-debug"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/dist"
                      RELEASE_OUTPUT_NAME "spacetyper"
                      )

target_link_libraries(spacetyper
                      core
                      gui
                      render
                      ${OPENGL_LIBRARIES}
                      ${SDL2_LIBRARY}
                      )

if(CMAKE_COMPILER_IS_GNUCC)
  set_property(TARGET spacetyper APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif()

