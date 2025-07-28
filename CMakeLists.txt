cmake_minimum_required(VERSION 3.20)
project(IceClimberClone VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-deprecated)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2 -DNDEBUG)
    else()
        add_compile_options(-O0 -g)
    endif()
endif()

# Platform-specific definitions
if(WIN32)
    add_compile_definitions(WINDOWS)
endif()

# Find packages
find_package(Threads REQUIRED)

# Try to find raylib using different methods
find_package(raylib CONFIG QUIET)
if(NOT raylib_FOUND)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(raylib QUIET raylib)
    endif()
endif()

# If still not found, use system linking (MSYS2 style)
if(NOT raylib_FOUND AND NOT raylib_LINK_LIBRARIES)
    message(STATUS "Using system raylib library")
    set(RAYLIB_LIBRARIES raylib)
    # Try to find raylib header
    find_path(RAYLIB_INCLUDE_DIRS raylib.h
        PATHS
        /usr/include
        /usr/local/include
        /mingw64/include
        C:/msys64/mingw64/include
    )
    if(RAYLIB_INCLUDE_DIRS)
        include_directories(${RAYLIB_INCLUDE_DIRS})
        message(STATUS "Found raylib headers at: ${RAYLIB_INCLUDE_DIRS}")
    endif()
else()
    set(RAYLIB_LIBRARIES raylib)
endif()

# Include directories
include_directories(src)
include_directories(third_party)
include_directories(.)

# Source files
set(SOURCES
    main.cpp
    src/collision/geometry/Rectangle.cpp
    src/sprite_rect_double_buffer.cpp
    src/position.cpp
    src/entity.cpp
    src/entity_factory.cpp
    src/entities/player.cpp
    src/entities/player_state_transitions.cpp
    src/entities/topi.cpp
    src/entities/topi_state_transitions.cpp
    src/entities/nitpicker.cpp
    src/entities/condor.cpp
    src/entities/game_over.cpp
    src/entities/nitpicker_state_transitions.cpp
    src/entities/ice.cpp
    src/entities/water.cpp
    src/entities/bonus_stage_text.cpp
    src/entities/bonus_stage_tile.cpp
    src/entities/vegetable.cpp
    src/entities/number.cpp
    src/entities/brick.cpp
    src/entities/cloud.cpp
    src/entities/side_wall.cpp
    src/state_machine.cpp
    src/game_manager.cpp
    src/sound_manager.cpp
    src/sprite.cpp
    src/entity_sprite_sheet_animation.cpp
    src/entity_sprite_sheet.cpp
    src/entity_data_manager.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${RAYLIB_LIBRARIES}
    Threads::Threads
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 gdi32 winmm)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreAudio"
        "-framework CoreVideo"
        "-framework CoreFoundation"
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE GL dl pthread m)
endif()

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/audio DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/levels DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/entities.dat DESTINATION ${CMAKE_BINARY_DIR})

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY audio levels textures DESTINATION bin)
install(FILES entities.dat DESTINATION bin)
