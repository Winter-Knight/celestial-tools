LIB = libCelestialWorlds.so.0
DLL = CelestialWorlds.dll
ROOT_DIR= .
SOURCE_FILES= $(ROOT_DIR)/texture.cpp $(ROOT_DIR)/celestialworlds.cpp
SOURCE_FILES+= $(ROOT_DIR)/celestial.cpp $(ROOT_DIR)/skypanorama.cpp $(ROOT_DIR)/parser.cpp
SOURCE_FILES+= $(ROOT_DIR)/sphere.cpp $(ROOT_DIR)/glsphere.cpp $(ROOT_DIR)/framebuffer.cpp
SOURCE_FILES+= $(ROOT_DIR)/glplane.cpp $(ROOT_DIR)/lights.cpp $(ROOT_DIR)/resource-handler.cpp
HEADERS = $(shell ls $(ROOT_DIR)/*.h $(ROOT_DIR)/*.hpp)

CXX = g++
OPT = -O0 -g
LIB_OPTIONS = -shared -Wl,-soname,$(LIB)
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -fpic -I.
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lfreeimage

WIN_CXX = i686-w64-mingw32-g++
WIN_LIB_OPTIONS = -shared
WIN_CXXFLAGS = -Wall -Wno-unused-variable -Wno-comment $(OPT) -I$(HOME)/win32-depends/include/
WIN_OBJECTS = $(patsubst %.cpp,%.obj,$(SOURCE_FILES))
WIN_LIBS = -static-libstdc++ -static-libgcc -lws2_32 -L$(HOME)/win32-depends/lib/ -lglew32 -lopengl32 -lFreeImage

libCelestialWorlds.so: $(LIB)
	-ln -s $< $@

$(LIB): $(LIB).0.0
	-ln -s $< $@

$(LIB).0.0: $(OBJECTS)
	$(CXX) -o $@ $^ $(LIB_OPTIONS) $(LIBS)
	chmod 644 $@

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

# By using -mwindows, you won't get a console popping up when you start your app from the
# GUI, but you also won't get stdout or stderr when you start your app from the console
$(DLL): $(WIN_OBJECTS)
	$(WIN_CXX) -o $@ $^ $(WIN_LIB_OPTIONS) $(WIN_LIBS)
	chmod 644 $@

%.obj: %.cpp $(HEADERS)
	$(WIN_CXX) -c -o $@ $(WIN_CXXFLAGS) $<

clean:
	-rm $(OBJECTS)
	-rm *.0 *.so

win-clean:
	-rm $(DLL)
	-rm $(WIN_OBJECTS)

clean-all: clean win-clean
	cd examples/terrainjumper && make clean-all
	cd tools/explorer && make clean-all
	cd tools/starbox-creator && make clean-all
