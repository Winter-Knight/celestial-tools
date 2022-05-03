# Starbox creator creates the background skybox with just stars

BIN = starbox-creator
SOURCE_FILES= main.cpp ../window.cpp ../input.cpp ../sphere.cpp ../camera.cpp ../framebuffer.cpp world.cpp star.cpp optionswindow.cpp
HEADERS = $(shell ls *.h ../*.h)

CXX = g++
OPT = -O0 -g
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -I.. -I/usr/include/imgui
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lGLEW -lSDL2 -lSDL2_image -limgui -lstb

$(BIN): $(OBJECTS)
	$(CXX) -o $(BIN) $^ $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) -c -o $@ $(CXXFLAGS) $<

clean:
	-rm $(BIN)
	-rm $(OBJECTS)
