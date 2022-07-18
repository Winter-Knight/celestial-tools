# Starbox creator creates the background skybox with just stars

BIN = starbox-creator
ROOT_DIR= ../..
SOURCE_FILES= main.cpp world.cpp star.cpp optionswindow.cpp texturerect.cpp pngsaver.cpp
SOURCE_FILES+= $(ROOT_DIR)/sdl/window.cpp $(ROOT_DIR)/sdl/input.cpp $(ROOT_DIR)/sphere.cpp
SOURCE_FILES+= $(ROOT_DIR)/framebuffer.cpp $(ROOT_DIR)/resource-handler.cpp $(ROOT_DIR)/texture.cpp
SOURCE_FILES+= imguifilebrowser/ImGuiFileBrowser.cpp
HEADERS = $(shell ls $(ROOT_DIR)/*.h $(ROOT_DIR)/*.hpp $(ROOT_DIR)/sdl/*.h)

CXX = g++
OPT = -O0 -g
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -I$(ROOT_DIR) -I$(ROOT_DIR)/sdl -I. -I/usr/include/imgui -I/usr/include/libpng16
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lGLEW -lSDL2 -lfreeimage -limgui -lstb -lpng16

$(BIN): $(OBJECTS)
	$(CXX) -o $(BIN) $^ $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	-rm $(BIN)
	-rm $(OBJECTS)
