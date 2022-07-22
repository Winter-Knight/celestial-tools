BIN = terrainjumper
ROOT_DIR= ../..
SOURCE_FILES= terrainjumper.cpp terrain.cpp player.cpp
SOURCE_FILES+= $(ROOT_DIR)/sdl/window.cpp $(ROOT_DIR)/sdl/input.cpp
HEADERS = $(shell ls $(ROOT_DIR)/*.h $(ROOT_DIR)/*.hpp $(ROOT_DIR)/sdl/*.h)
LIBNAME = CelestialWorlds
LIB = libCelestialWorlds.so
DLL = $(LIBNAME).dll
MAKEFILE_GLFW = Makefile.glfw

CXX = g++
OPT = -O0 -g
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -I$(ROOT_DIR) -I$(ROOT_DIR)/sdl -I.
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lGLEW -lSDL2 -lfreeimage -L$(ROOT_DIR) -l$(LIBNAME)
MAKE = make

WIN_CXX = i686-w64-mingw32-g++
WIN_CXXFLAGS = -Wall -Wno-unused-variable -Wno-comment $(OPT) -DSDL_MAIN_HANDLED -I$(HOME)/win32-depends/include/ -I$(ROOT_DIR) -I$(ROOT_DIR)/sdl -I.
WIN_OBJECTS = $(patsubst %.cpp,%.obj,$(SOURCE_FILES))
WIN_LIBS = -static-libstdc++ -static-libgcc -L$(HOME)/win32-depends/lib/ -lglew32 -lopengl32 -lSDL2 -lFreeImage -L $(ROOT_DIR) -lCelestialWorlds

$(BIN): $(OBJECTS) $(ROOT_DIR)/$(LIB)
	$(CXX) -o $(BIN) $(OBJECTS) $(LIBS)

$(BIN)-glfw:
	$(MAKE) -f $(MAKEFILE_GLFW)

$(BIN)-glfw.exe:
	$(MAKE) -f $(MAKEFILE_GLFW) $@

terrainjumper.o: terrainjumper.cpp $(HEADERS) $(shell ls *.h)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

terrain.o: terrain.cpp $(HEADERS) $(shell ls *.h)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

player.o: player.cpp $(HEADERS) $(shell ls *.h)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

$(ROOT_DIR)/$(LIB): $(shell ls $(ROOT_DIR)/*.cpp) $(HEADERS)
	cd $(ROOT_DIR) && $(MAKE) $(LIB)

# By using -mwindows, you won't get a console popping up when you start your app from the
# GUI, but you also won't get stdout or stderr when you start your app from the console
$(BIN).exe: $(WIN_OBJECTS) $(ROOT_DIR)/$(DLL)
	$(WIN_CXX) -o $@ $(WIN_OBJECTS) $(WIN_LIBS)
	chmod 644 $@

%.obj: %.cpp $(HEADERS)
	$(WIN_CXX) -c -o $@ $(WIN_CXXFLAGS) $<

$(ROOT_DIR)/$(DLL): $(shell ls $(ROOT_DIR)/*.cpp) $(HEADERS)
	cd $(ROOT_DIR) && $(MAKE) $(DLL)

clean:
	-rm $(BIN)
	-rm $(OBJECTS)
	$(MAKE) -f $(MAKEFILE_GLFW) clean

win-clean:
	-rm $(BIN).exe
	-rm $(WIN_OBJECTS)
	$(MAKE) -f $(MAKEFILE_GLFW) win-clean

clean-all: clean win-clean

wine-copy: $(BIN).exe $(ROOT_DIR)/$(DLL) $(ROOT_DIR)/tools/explorer/explorer.exe
	cp $^ /mnt/wine/home/wk/celestial/
