CXXFLAGS = -O2
HEADERS = ../sphere.h torus.h
LIBS = -lGL -lSDL2

showobj: torus.o sdl-gl-showobj.o $(HEADERS)
	g++ -o showobj torus.o sdl-gl-showobj.o $(LIBS)

sphere.o: ../sphere.cpp $(HEADERS)
	g++ $(CXXFLAGS) -c -o sphere.o ../sphere.cpp

torus.o: torus.cpp $(HEADERS)
	g++ $(CXXFLAGS) -c -o torus.o torus.cpp

sdl-gl-showobj.o: sdl-gl-showobj.cpp $(HEADERS)
	g++ $(CXXFLAGS) -c -o sdl-gl-showobj.o sdl-gl-showobj.cpp

clean:
	rm *.o showobj
