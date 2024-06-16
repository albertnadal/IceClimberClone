cmake_minimum_required(VERSION 3.15)
project(rocket)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops -Wno-deprecated -I/usr/local/Cellar/glfw/3.3/include/ -I/usr/local/include -I. -Isrc/ -Ithird_party")
set(CMAKE_CXX_LINK_FLAGS "-Wl,-search_paths_first -Wl,-headerpad_max_install_names -framework OpenGL -framework Cocoa -lGLFW -L/usr/local/Cellar/glfw/3.3/lib/")

include_directories(src)
include_directories(src/items)
include_directories(third_party)
include_directories(third_party/AABB)
include_directories(third_party/collision)
include_directories(third_party/collision/algorithm)
include_directories(third_party/collision/geometry)
include_directories(third_party/collision/math)
include_directories(third_party/collision/structures)
include_directories(third_party/glad)
include_directories(third_party/KHR)
include_directories(third_party/MersenneTwister)

add_executable(rocket
        src/collision/algorithm/CollisionDetector.cpp
        src/collision/algorithm/CollisionDetector.h
        src/collision/algorithm/Penetration.h
        src/collision/geometry/Rectangle.cpp
        src/collision/geometry/Rectangle.h
        src/collision/math/Vector2Util.h
        src/collision/structures/mat2x2.hpp
        src/collision/structures/vec2.hpp
        src/collision/collision.h
        src/objects/brick.cpp
        src/objects/brick.h
        src/objects/player.cpp
        src/objects/player.h
        src/objects/side_wall.cpp
        src/objects/side_wall.h
        src/defines.h
        src/filesystem.h
        src/float_double_buffer.cpp
        src/float_double_buffer.h
        src/fvec2.cpp
        src/fvec2.h
        src/object_sprite_sheet.cpp
        src/object_sprite_sheet.h
        src/object_sprite_sheet_animation.cpp
        src/object_sprite_sheet_animation.h
        src/position.cpp
        src/position.h
        src/scene_object.cpp
        src/scene_object.h
        src/scene_object_data_manager.cpp
        src/scene_object_data_manager.h
        src/scene_object_factory.cpp
        src/scene_object_factory.h
        src/scene_object_manager.cpp
        src/scene_object_manager.h
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
        src/utils.h
        src/vec2.cpp
        src/vec2.h
        third_party/AABB/AABB.h
        third_party/glad/glad.cpp
        third_party/glad/glad.h
        third_party/KHR/khrplatform.h
        third_party/MersenneTwister/MersenneTwister.h
        main.cpp)

target_link_libraries(rocket glfw)