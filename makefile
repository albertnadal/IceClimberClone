CXX=g++
SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.5.sdk
CFLAGS=-std=c++17 -stdlib=libc++ -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops -Wno-deprecated -I/usr/local/include -I. -Isrc/ -Ithird_party -isysroot $(SDKROOT)
LDFLAGS=-Wl,-search_paths_first -Wl,-headerpad_max_install_names -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo -framework CoreFoundation -lraylib -Lthird_party/raylib/
EXEC=main

all: Rectangle.o CollisionDetector.o sprite_rect_double_buffer.o position.o scene_object.o scene_object_factory.o player.o brick.o side_wall.o state_machine.o scene_object_manager.o sprite.o object_sprite_sheet_animation.o object_sprite_sheet.o scene_object_data_manager.o
	$(CXX) $(CFLAGS) $(LDFLAGS) main.cpp scene_object.o scene_object_factory.o player.o brick.o side_wall.o state_machine.o scene_object_manager.o sprite.o scene_object_data_manager.o object_sprite_sheet.o object_sprite_sheet_animation.o position.o sprite_rect_double_buffer.o Rectangle.o CollisionDetector.o -o $(EXEC)

player.o: src/objects/player.cpp
	$(CXX) -c $(CFLAGS) src/objects/player.cpp

brick.o: src/objects/brick.cpp
	$(CXX) -c $(CFLAGS) src/objects/brick.cpp

side_wall.o: src/objects/side_wall.cpp
	$(CXX) -c $(CFLAGS) src/objects/side_wall.cpp

state_machine.o: src/state_machine.cpp
	$(CXX) -c $(CFLAGS) src/state_machine.cpp

scene_object_factory.o: src/scene_object_factory.cpp
	$(CXX) -c $(CFLAGS) src/scene_object_factory.cpp

scene_object_data_manager.o: src/scene_object_data_manager.cpp
	$(CXX) -c $(CFLAGS) src/scene_object_data_manager.cpp

scene_object_manager.o: src/scene_object_manager.cpp
	$(CXX) -c $(CFLAGS) src/scene_object_manager.cpp

scene_object.o: src/scene_object.cpp
	$(CXX) -c $(CFLAGS) src/scene_object.cpp

object_sprite_sheet.o: src/object_sprite_sheet.cpp
	$(CXX) -c $(CFLAGS) src/object_sprite_sheet.cpp

object_sprite_sheet_animation.o: src/object_sprite_sheet_animation.cpp
	$(CXX) -c $(CFLAGS) src/object_sprite_sheet_animation.cpp

sprite.o: src/sprite.cpp
	$(CXX) -c $(CFLAGS) src/sprite.cpp

position.o: src/position.cpp
	$(CXX) -c $(CFLAGS) src/position.cpp

sprite_rect_double_buffer.o: src/sprite_rect_double_buffer.cpp
	$(CXX) -c $(CFLAGS) src/sprite_rect_double_buffer.cpp

Rectangle.o: src/collision/geometry/Rectangle.cpp
	$(CXX) -c $(CFLAGS) src/collision/geometry/Rectangle.cpp

CollisionDetector.o: src/collision/algorithm/CollisionDetector.cpp
	$(CXX) -c $(CFLAGS) src/collision/algorithm/CollisionDetector.cpp

clean:
	rm -f $(EXEC) *.o *.gch src/*.o src/*.gch third_party/collision/structures/*.gch third_party/AABB/*.gch
