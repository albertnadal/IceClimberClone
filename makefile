CXX=g++
SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.5.sdk
CFLAGS=-std=c++17 -stdlib=libc++ -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops -Wno-deprecated -I/usr/local/include -I. -Isrc/ -Ithird_party -isysroot $(SDKROOT)
LDFLAGS=-Wl,-search_paths_first -Wl,-headerpad_max_install_names -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo -framework CoreFoundation -lraylib -Lthird_party/raylib/
EXEC=main

all: Rectangle.o sprite_rect_double_buffer.o position.o entity.o entity_factory.o player.o player_state_transitions.o topi.o topi_state_transitions.o nitpicker.o condor.o game_over.o nitpicker_state_transitions.o ice.o water.o bonus_stage_text.o bonus_stage_tile.o vegetable.o number.o brick.o cloud.o side_wall.o state_machine.o game_manager.o sound_manager.o sprite.o entity_sprite_sheet_animation.o entity_sprite_sheet.o entity_data_manager.o
	$(CXX) $(CFLAGS) $(LDFLAGS) main.cpp entity.o entity_factory.o player.o player_state_transitions.o topi.o topi_state_transitions.o nitpicker.o condor.o game_over.o nitpicker_state_transitions.o ice.o water.o bonus_stage_text.o bonus_stage_tile.o vegetable.o number.o brick.o cloud.o side_wall.o state_machine.o game_manager.o sound_manager.o sprite.o entity_data_manager.o entity_sprite_sheet.o entity_sprite_sheet_animation.o position.o sprite_rect_double_buffer.o Rectangle.o -o $(EXEC)

player.o: src/entities/player.cpp
	$(CXX) -c $(CFLAGS) src/entities/player.cpp

player_state_transitions.o: src/entities/player_state_transitions.cpp
	$(CXX) -c $(CFLAGS) src/entities/player_state_transitions.cpp

topi.o: src/entities/topi.cpp
	$(CXX) -c $(CFLAGS) src/entities/topi.cpp

topi_state_transitions.o: src/entities/topi_state_transitions.cpp
	$(CXX) -c $(CFLAGS) src/entities/topi_state_transitions.cpp

nitpicker.o: src/entities/nitpicker.cpp
	$(CXX) -c $(CFLAGS) src/entities/nitpicker.cpp

nitpicker_state_transitions.o: src/entities/nitpicker_state_transitions.cpp
	$(CXX) -c $(CFLAGS) src/entities/nitpicker_state_transitions.cpp

condor.o: src/entities/condor.cpp
	$(CXX) -c $(CFLAGS) src/entities/condor.cpp

game_over.o: src/entities/game_over.cpp
	$(CXX) -c $(CFLAGS) src/entities/game_over.cpp

ice.o: src/entities/ice.cpp
	$(CXX) -c $(CFLAGS) src/entities/ice.cpp

water.o: src/entities/water.cpp
	$(CXX) -c $(CFLAGS) src/entities/water.cpp

bonus_stage_text.o: src/entities/bonus_stage_text.cpp
	$(CXX) -c $(CFLAGS) src/entities/bonus_stage_text.cpp

bonus_stage_tile.o: src/entities/bonus_stage_tile.cpp
	$(CXX) -c $(CFLAGS) src/entities/bonus_stage_tile.cpp

vegetable.o: src/entities/vegetable.cpp
	$(CXX) -c $(CFLAGS) src/entities/vegetable.cpp

number.o: src/entities/number.cpp
	$(CXX) -c $(CFLAGS) src/entities/number.cpp

brick.o: src/entities/brick.cpp
	$(CXX) -c $(CFLAGS) src/entities/brick.cpp

cloud.o: src/entities/cloud.cpp
	$(CXX) -c $(CFLAGS) src/entities/cloud.cpp

side_wall.o: src/entities/side_wall.cpp
	$(CXX) -c $(CFLAGS) src/entities/side_wall.cpp

state_machine.o: src/state_machine.cpp
	$(CXX) -c $(CFLAGS) src/state_machine.cpp

entity_factory.o: src/entity_factory.cpp
	$(CXX) -c $(CFLAGS) src/entity_factory.cpp

entity_data_manager.o: src/entity_data_manager.cpp
	$(CXX) -c $(CFLAGS) src/entity_data_manager.cpp

game_manager.o: src/game_manager.cpp
	$(CXX) -c $(CFLAGS) src/game_manager.cpp

sound_manager.o: src/sound_manager.cpp
	$(CXX) -c $(CFLAGS) src/sound_manager.cpp

entity.o: src/entity.cpp
	$(CXX) -c $(CFLAGS) src/entity.cpp

entity_sprite_sheet.o: src/entity_sprite_sheet.cpp
	$(CXX) -c $(CFLAGS) src/entity_sprite_sheet.cpp

entity_sprite_sheet_animation.o: src/entity_sprite_sheet_animation.cpp
	$(CXX) -c $(CFLAGS) src/entity_sprite_sheet_animation.cpp

sprite.o: src/sprite.cpp
	$(CXX) -c $(CFLAGS) src/sprite.cpp

position.o: src/position.cpp
	$(CXX) -c $(CFLAGS) src/position.cpp

sprite_rect_double_buffer.o: src/sprite_rect_double_buffer.cpp
	$(CXX) -c $(CFLAGS) src/sprite_rect_double_buffer.cpp

Rectangle.o: src/collision/geometry/Rectangle.cpp
	$(CXX) -c $(CFLAGS) src/collision/geometry/Rectangle.cpp

clean:
	rm -f $(EXEC) *.o *.gch src/*.o src/*.gch third_party/collision/structures/*.gch third_party/AABB/*.gch
