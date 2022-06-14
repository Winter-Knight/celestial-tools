# Starbox creator creates the background skybox with just stars

BIN = starbox-creator
SOURCE_FILES= main.cpp ../window.cpp ../input.cpp ../sphere.cpp ../camera.cpp ../framebuffer.cpp world.cpp star.cpp optionswindow.cpp
SOURCE_FILES+= pngsaver.cpp imguifilebrowser/ImGuiFileBrowser.cpp
HEADERS = $(shell ls *.h ../*.h imguifilebrowser/*.h)

CXX = g++
OPT = -O0 -g
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -I.. -I/usr/include/imgui -I/usr/include/libpng16
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lGLEW -lSDL2 -lSDL2_image -limgui -lstb -lpng16

$(BIN): $(OBJECTS)
	$(CXX) -o $(BIN) $^ $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	-rm $(BIN)
	-rm $(OBJECTS)
