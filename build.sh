#!/bin/bash
SDKROOT="$(xcrun --sdk macosx --show-sdk-path)"
g++ -std=c++11 -stdlib=libc++ -Ofast glad/glad.c main.cpp -o main -isysroot ${SDKROOT} -Wl,-search_paths_first -Wl,-headerpad_max_install_names -framework OpenGL -framework Cocoa -Wno-deprecated -I/usr/local/Cellar/glfw/3.2.1/include/ -L/usr/local/Cellar/glfw/3.2.1/lib/ -lGLFW -I.
