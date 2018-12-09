CXX=g++
SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk
CFLAGS=-std=c++11 -stdlib=libc++ -Ofast -Wno-deprecated -I/usr/local/Cellar/glfw/3.2.1/include/ -I. -isysroot $(SDKROOT)
LDFLAGS=-Wl,-search_paths_first -Wl,-headerpad_max_install_names -framework OpenGL -framework Cocoa -lGLFW -L/usr/local/Cellar/glfw/3.2.1/lib/
EXEC=main

all: glad.o vec2.o scene_object_manager.o scene_object.o sprite.o sprite_texture.o object_frame.o object_action.o object_type.o scene_object_texture_manager.o
	$(CXX) $(CFLAGS) $(LDFLAGS) main.cpp scene_object_manager.o scene_object.o sprite.o sprite_texture.o scene_object_texture_manager.o object_type.o object_action.o object_frame.o vec2.o glad.o -o $(EXEC)

scene_object_texture_manager.o: scene_object_texture_manager.cpp
	$(CXX) -c $(CFLAGS) scene_object_texture_manager.cpp

scene_object_manager.o: scene_object_manager.cpp
	$(CXX) -c $(CFLAGS) scene_object_manager.cpp

object_type.o: object_type.cpp
	$(CXX) -c $(CFLAGS) object_type.cpp

object_action.o: object_action.cpp
	$(CXX) -c $(CFLAGS) object_action.cpp

object_frame.o: object_frame.cpp
	$(CXX) -c $(CFLAGS) object_frame.cpp

scene_object.o: scene_object.cpp
	$(CXX) -c $(CFLAGS) scene_object.cpp

sprite_texture.o: sprite_texture.cpp
	$(CXX) -c $(CFLAGS) sprite_texture.cpp

sprite.o: sprite.cpp
	$(CXX) -c $(CFLAGS) sprite.cpp

vec2.o: vec2.cpp
	$(CXX) -c $(CFLAGS) vec2.cpp

glad.o: glad/glad.c
	$(CXX) -c $(CFLAGS) glad/glad.c

clean:
	rm $(EXEC) *.o
