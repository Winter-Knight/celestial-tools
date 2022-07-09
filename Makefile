BIN = celestial
SOURCE_FILES= main.cpp window.cpp input.cpp texture.cpp world.cpp camera.cpp celestial.cpp skybox.cpp skypanorama.cpp parser.cpp
SOURCE_FILES+= sphere.cpp glsphere.cpp gltesssphere.cpp framebuffer.cpp groundplane.cpp glplane.cpp lights.cpp
HEADERS = $(shell ls *.h)

CXX = g++
OPT = -O0 -g
CXXFLAGS = -Wall -Wno-unused-variable $(OPT)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lGLEW -lSDL2 -lSDL2_image

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

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

libfont/%.o:
	cd $(FONT_DIR) && make

clean:
	-rm $(BIN)
	-rm $(OBJECTS)

win-clean:
	del $(BIN).exe
	del $(OBJECTS)
