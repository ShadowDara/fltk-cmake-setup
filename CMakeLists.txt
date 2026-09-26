cmake_minimum_required(VERSION 3.15)

project(app LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/arch:AVX2)
endif()

# Disable IPO/LTO for Debug and Release
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE OFF)

# Use the dynamic MSVC runtime (/MD, /MDd)
set(CMAKE_MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
    CACHE STRING "" FORCE
)

# Build FLTK as shared libraries
set(FLTK_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# Build dependencies statically
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Disable unnecessary FLTK targets
set(FLTK_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(FLTK_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FLTK_BUILD_FLUID OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/fltk)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(app
    ${MY_SOURCES}
)


target_include_directories(app
    PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/fltk"
)


target_include_directories(app
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)


set_property(TARGET app PROPERTY
    MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)


target_link_libraries(app
    PRIVATE
        fltk-shared
)

if(MSVC)

    foreach(FLTK_TARGET
        fltk-shared
        fltk_png-shared
        fltk_jpeg-shared
        fltk_z-shared
    )

        if(TARGET ${FLTK_TARGET})

            add_custom_command(TARGET app POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:${FLTK_TARGET}>"
                    "$<TARGET_FILE_DIR:app>"

                COMMENT "Copying ${FLTK_TARGET} DLL"
            )

        endif()

    endforeach()

endif()
