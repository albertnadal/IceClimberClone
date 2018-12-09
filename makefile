CXX=g++
SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk
CFLAGS=-std=c++11 -stdlib=libc++ -Ofast -Wno-deprecated -I/usr/local/Cellar/glfw/3.2.1/include/ -I. -isysroot $(SDKROOT)
LDFLAGS=-Wl,-search_paths_first -Wl,-headerpad_max_install_names -framework OpenGL -framework Cocoa -lGLFW -L/usr/local/Cellar/glfw/3.2.1/lib/
EXEC=main

all: glad.o vec2.o object_frame.o object_action.o object_type.o object_manager.o
	$(CXX) $(CFLAGS) $(LDFLAGS) main.cpp object_manager.o object_type.o object_action.o object_frame.o vec2.o glad.o -o $(EXEC)

object_manager.o: object_manager.cpp
	$(CXX) -c $(CFLAGS) object_manager.cpp

object_type.o: object_type.cpp
	$(CXX) -c $(CFLAGS) object_type.cpp

object_action.o: object_action.cpp
	$(CXX) -c $(CFLAGS) object_action.cpp

object_frame.o: object_frame.cpp
	$(CXX) -c $(CFLAGS) object_frame.cpp

vec2.o: vec2.cpp
	$(CXX) -c $(CFLAGS) vec2.cpp

glad.o: glad/glad.c
	$(CXX) -c $(CFLAGS) glad/glad.c

clean:
	rm $(EXEC) *.o
