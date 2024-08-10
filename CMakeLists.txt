cmake_minimum_required(VERSION 3.15)
project(rocket)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++ -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops -Wno-deprecated -I/usr/local/Cellar/glfw/3.3/include/ -I/usr/local/include -I. -Isrc/ -Ithird_party")
set(CMAKE_CXX_LINK_FLAGS "-Wl,-search_paths_first -Wl,-headerpad_max_install_names -framework OpenGL -framework Cocoa -lGLFW -L/usr/local/Cellar/glfw/3.3/lib/")

include_directories(src)
include_directories(src/items)
include_directories(third_party)
include_directories(third_party/AABB)
include_directories(third_party/collision)
include_directories(third_party/collision/geometry)
include_directories(third_party/collision/math)
include_directories(third_party/collision/structures)
include_directories(third_party/glad)
include_directories(third_party/KHR)
include_directories(third_party/MersenneTwister)

add_executable(rocket
        src/collision/geometry/Rectangle.cpp
        src/collision/geometry/Rectangle.h
        src/collision/math/Vector2Util.h
        src/collision/structures/mat2x2.hpp
        src/collision/structures/vec2.hpp
        src/collision/collision.h
        src/entities/brick.cpp
        src/entities/brick.h
        src/entities/cloud.cpp
        src/entities/cloud.h
        src/entities/player_state_transitions.cpp
        src/entities/player.cpp
        src/entities/player.h
        src/entities/side_wall.cpp
        src/entities/side_wall.h
        src/entities/topi_state_transitions.cpp
        src/entities/topi.cpp
        src/entities/topi.h
        src/entities/ice.cpp
        src/entities/ice.h
        src/entities/water.cpp
        src/entities/water.h
        src/entities/bonus_stage_text.cpp
        src/entities/bonus_stage_text.h
        src/entities/bonus_stage_tile.cpp
        src/entities/bonus_stage_tile.h
        src/entities/vegetable.cpp
        src/entities/vegetable.h
        src/entities/number.cpp
        src/entities/number.h
        src/entities/nitpicker_state_transitions.cpp
        src/entities/nitpicker.cpp
        src/entities/nitpicker.h
        src/defines.h
        src/filesystem.h
        src/float_double_buffer.cpp
        src/float_double_buffer.h
        src/entity_sprite_sheet.cpp
        src/entity_sprite_sheet.h
        src/entity_sprite_sheet_animation.cpp
        src/entity_sprite_sheet_animation.h
        src/position.cpp
        src/position.h
        src/entity.cpp
        src/entity.h
        src/entity_data_manager.cpp
        src/entity_data_manager.h
        src/entity_factory.cpp
        src/entity_factory.h
        src/entity_manager.cpp
        src/entity_manager.h
        src/shader.h
        src/shader_m.h
        src/shader_s.h
        src/sprite.cpp
        src/sprite.h
        src/sprite_texture.cpp
        src/sprite_texture.h
        src/state_machine.cpp
        src/state_machine.h
        src/stb_image.h
        src/uint16_double_buffer.cpp
        src/uint16_double_buffer.h
        third_party/AABB/AABB.h
        third_party/MersenneTwister/MersenneTwister.h
        third_party/raylib/raylib.h
        main.cpp)

target_link_libraries(rocket glfw)