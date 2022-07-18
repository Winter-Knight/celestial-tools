BIN = terrainjumper
ROOT_DIR= ../..
SOURCE_FILES= terrainjumper.cpp terrain.cpp player.cpp
SOURCE_FILES+= $(ROOT_DIR)/sdl/window.cpp $(ROOT_DIR)/sdl/input.cpp
SOURCE_FILES+= $(ROOT_DIR)/celestialworlds.cpp $(ROOT_DIR)/texture.cpp $(ROOT_DIR)/celestial.cpp
SOURCE_FILES+= $(ROOT_DIR)/skypanorama.cpp $(ROOT_DIR)/parser.cpp $(ROOT_DIR)/sphere.cpp $(ROOT_DIR)/glsphere.cpp
SOURCE_FILES+= $(ROOT_DIR)/framebuffer.cpp $(ROOT_DIR)/glplane.cpp $(ROOT_DIR)/lights.cpp $(ROOT_DIR)/resource-handler.cpp
HEADERS = $(shell ls $(ROOT_DIR)/*.h $(ROOT_DIR)/*.hpp $(ROOT_DIR)/sdl/*.h)

CXX = g++
OPT = -O0 -g
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -I$(ROOT_DIR) -I$(ROOT_DIR)/sdl -I.
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lGLEW -lSDL2 -lfreeimage -L. -lFastNoise

WIN_NATIVE_CXX = g++
WIN_NATIVE_CXXFLAGS = -Wall -Wno-comment -O2 -DSDL_MAIN_HANDLED -DGLEW_STATIC -I../mbsoftworks-win32-depends/include/
WIN_NATIVE_OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
WIN_NATIVE_LIBS = -L../mbsoftworks-win32-depends/lib/ -lSDL2 -lSDL2_image -lglew32 -lopengl32 -lassimp -lentityx

WIN_CXX = i686-w64-mingw32-g++
WIN_CXXFLAGS = -Wall -Wno-comment -O2 -DSDL_MAIN_HANDLED -DGLEW_STATIC -I$(HOME)/mbsoftworks-win32-depends/include/
WIN_OBJECTS = $(patsubst %.cpp,%.obj,$(SOURCE_FILES))
WIN_LIBS = -L$(HOME)/mbsoftworks-win32-depends/lib/ -lSDL2 -lglew32 -lopengl32 -lFreeImage -lfreetype-6

$(BIN): $(OBJECTS)
	$(CXX) -o $(BIN) $^ $(LIBS)

$(BIN).exe: $(WIN_NATIVE_OBJECTS)
	$(WIN_NATIVE_CXX) -o $(BIN).exe $^ $(WIN_NATIVE_LIBS)

# By using -mwindows, you won't get a console popping up when you start your app from the
# GUI, but you also won't get stdout or stderr when you start your app from the console
win-gui: $(OBJECTS)
	$(CXX) -o $(BIN).exe $^ $(LIBS) -mwindows

terrainjumper.o: terrainjumper.cpp $(HEADERS) $(shell ls *.h)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

terrain.o: terrain.cpp $(HEADERS) $(shell ls *.h)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

player.o: player.cpp $(HEADERS) $(shell ls *.h)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	-rm $(BIN)
	-rm $(OBJECTS)

win-clean:
	del $(BIN).exe
	del $(OBJECTS)
