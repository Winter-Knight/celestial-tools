LIB=libCelestialWorlds.so.0
ROOT_DIR= .
SOURCE_FILES= $(ROOT_DIR)/texture.cpp $(ROOT_DIR)/groundplane.cpp
SOURCE_FILES+= $(ROOT_DIR)/celestial.cpp $(ROOT_DIR)/skypanorama.cpp $(ROOT_DIR)/parser.cpp
SOURCE_FILES+= $(ROOT_DIR)/sphere.cpp $(ROOT_DIR)/glsphere.cpp $(ROOT_DIR)/framebuffer.cpp
SOURCE_FILES+= $(ROOT_DIR)/glplane.cpp $(ROOT_DIR)/lights.cpp $(ROOT_DIR)/resource-handler.cpp
HEADERS = $(shell ls $(ROOT_DIR)/*.h $(ROOT_DIR)/*.hpp)

CXX = g++
OPT = -O0 -g
LIB_OPTIONS = -shared -Wl,-soname,$LIB
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -fpic -I.
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lfreeimage

WIN_NATIVE_CXX = g++
WIN_NATIVE_CXXFLAGS = -Wall -Wno-comment -O2 -DSDL_MAIN_HANDLED -DGLEW_STATIC -I../mbsoftworks-win32-depends/include/
WIN_NATIVE_OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
WIN_NATIVE_LIBS = -L../mbsoftworks-win32-depends/lib/ -lSDL2 -lSDL2_image -lglew32 -lopengl32 -lassimp -lentityx

WIN_CXX = i686-w64-mingw32-g++
WIN_CXXFLAGS = -Wall -Wno-comment -O2 -DSDL_MAIN_HANDLED -DGLEW_STATIC -I$(HOME)/mbsoftworks-win32-depends/include/
WIN_OBJECTS = $(patsubst %.cpp,%.obj,$(SOURCE_FILES))
WIN_LIBS = -L$(HOME)/mbsoftworks-win32-depends/lib/ -lSDL2 -lglew32 -lopengl32 -lFreeImage -lfreetype-6

$(LIB): $(LIB).0.0
	ln -s $< $@

$(LIB).0.0: $(OBJECTS)
	$(CXX) -o $@ $^ $(LIB_OPTIONS) $(LIBS)
	chmod 644 $@

$(BIN).exe: $(WIN_NATIVE_OBJECTS)
	$(WIN_NATIVE_CXX) -o $(BIN).exe $^ $(WIN_NATIVE_LIBS)

# By using -mwindows, you won't get a console popping up when you start your app from the
# GUI, but you also won't get stdout or stderr when you start your app from the console
win-gui: $(OBJECTS)
	$(CXX) -o $(BIN).exe $^ $(LIBS) -mwindows

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	-rm $(BIN)
	-rm $(OBJECTS)

win-clean:
	del $(BIN).exe
	del $(OBJECTS)
