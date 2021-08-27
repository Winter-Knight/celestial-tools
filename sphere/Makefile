CXXFLAGS = -g -O0
HEADERS = $(shell ls *.h)
LIBS = -lGL -lSDL2

showobj: sphere.o sdl-gl-showobj.o $(HEADERS)
	g++ -o showobj sphere.o sdl-gl-showobj.o $(LIBS)

sphere.o: sphere.cpp $(HEADERS)
	g++ $(CXXFLAGS) -c -o sphere.o sphere.cpp

sdl-gl-showobj.o: sdl-gl-showobj.cpp $(HEADERS)
	g++ $(CXXFLAGS) -c -o sdl-gl-showobj.o sdl-gl-showobj.cpp

clean:
	rm *.o showobj
