VERSION = 0.1
LIB = libCelestialWorlds.so
LIB0 = libCelestialWorlds.so.0
LIB00 = libCelestialWorlds.so.0.0
ALL_LINUX_LIBS = $(LIB) $(LIB0) $(LIB00)
ALL_LINUX_BINS = examples/terrainjumper/terrainjumper examples/terrainjumper/terrainjumper-glfw
ALL_LINUX_BINS += tools/explorer/celestial-explorer tools/starbox-creator/starbox-creator
ALL_WIN32_BINS = examples/terrainjumper/terrainjumper.exe examples/terrainjumper/terrainjumper-glfw.exe
ALL_WIN32_BINS += tools/explorer/celestial-explorer.exe tools/starbox-creator/starbox-creator.exe
DLL = CelestialWorlds.dll
ROOT_DIR= .
SOURCE_FILES= $(ROOT_DIR)/texture.cpp $(ROOT_DIR)/celestialworlds.cpp $(ROOT_DIR)/glprogram.cpp
SOURCE_FILES+= $(ROOT_DIR)/celestial.cpp $(ROOT_DIR)/skypanorama.cpp $(ROOT_DIR)/parser.cpp
SOURCE_FILES+= $(ROOT_DIR)/sphere.cpp $(ROOT_DIR)/glsphere.cpp $(ROOT_DIR)/framebuffer.cpp
SOURCE_FILES+= $(ROOT_DIR)/glplane.cpp $(ROOT_DIR)/lights.cpp $(ROOT_DIR)/resource-handler.cpp
HEADERS = $(shell ls $(ROOT_DIR)/*.h $(ROOT_DIR)/*.hpp)
TMP = /tmp
PREFIX = $(HOME)/tmp
DESTDIR =
MAKE = make OPT="$(OPT)" ADD_CXXFLAGS="$(ADD_CXXFLAGS)"


CXX = g++
OPT = -O0 -g
LIB_OPTIONS = -shared -Wl,-soname,$(LIB0)
CXXFLAGS = -Wall -Wno-unused-variable $(OPT) -fpic -I. $(ADD_CXXFLAGS)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCE_FILES))
LIBS = -lGL -lfreeimage

WIN_CXX = i686-w64-mingw32-g++
WIN_LIB_OPTIONS = -shared
WIN_CXXFLAGS = -Wall -Wno-unused-variable -Wno-comment $(OPT) -I$(HOME)/win32-depends/include/ $(ADD_CXXFLAGS)
WIN_OBJECTS = $(patsubst %.cpp,%.obj,$(SOURCE_FILES))
WIN_LIBS = -static-libstdc++ -static-libgcc -lws2_32 -L$(HOME)/win32-depends/lib/ -lglew32 -lopengl32 -lFreeImage

$(LIB): $(LIB0)
	-ln -s $< $@

$(LIB0): $(LIB00)
	-ln -s $< $@

$(LIB00): $(OBJECTS)
	$(CXX) -o $@ $^ $(LIB_OPTIONS) $(LIBS)
	chmod 644 $@

install: $(ALL_LINUX_LIBS)
	install -d $(DESTDIR)$(PREFIX)/lib
	cp -a $^ $(DESTDIR)$(PREFIX)/lib
	install -d $(DESTDIR)$(PREFIX)/include/celestialworlds
	install *.hpp $(DESTDIR)$(PREFIX)/include/celestialworlds
	install -d $(DESTDIR)$(PREFIX)/share/celestialworlds/shaders
	install shaders/* $(DESTDIR)$(PREFIX)/share/celestialworlds/shaders
	

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
	-rm $(ALL_LINUX_LIBS) *.so

win-clean:
	-rm $(DLL)
	-rm $(WIN_OBJECTS)

clean-all: clean win-clean
	cd examples/terrainjumper && make clean-all
	cd tools/explorer && make clean-all
	cd tools/starbox-creator && make clean-all

# List of packages:
#src linux-deb (libCelestialWorlds libCelestialWorlds-dev libCelestialWorlds-examples libCelestialWorlds-tools) linux-tar.xz win32-zip win32-installer

package-src: clean-all
	-rm -rf $(TMP)/celestial-tools-$(VERSION)
	-mkdir $(TMP)/celestial-tools-$(VERSION)
	cp -a * .gitignore $(TMP)/celestial-tools-$(VERSION)
	cd $(TMP) && tar -c celestial-tools-$(VERSION) | gzip -9 > celestial-tools-$(VERSION).tar.gz
	cp $(TMP)/celestial-tools-$(VERSION).tar.gz ..
	cd .. && tar -xf celestial-tools-$(VERSION).tar.gz

# Linux bin:

examples/terrainjumper/terrainjumper:
	cd examples/terrainjumper; rm *.o; $(MAKE)

examples/terrainjumper/terrainjumper-glfw: examples/terrainjumper/terrainjumper
	cd examples/terrainjumper; rm *.o; $(MAKE) terrainjumper-glfw

tools/explorer/celestial-explorer:
	cd tools/explorer && $(MAKE)

tools/starbox-creator/starbox-creator:
	cd tools/starbox-creator && $(MAKE)

all-linux-bins: $(ALL_LINUX_BINS)

install-linux-bins: install all-linux-bins
	install -d $(DESTDIR)$(PREFIX)/games
	install examples/terrainjumper/terrainjumper* $(DESTDIR)$(PREFIX)/games
	install -d $(DESTDIR)$(PREFIX)/bin
	install tools/explorer/celestial-explorer $(DESTDIR)$(PREFIX)/bin/celestial-explorer
	install tools/starbox-creator/starbox-creator $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)$(PREFIX)/share/celestialworlds
	cp -a examples/*.txt examples/images tools/starbox-creator/shaders $(DESTDIR)$(PREFIX)/share/celestialworlds

package-linux-bin: $(shell ls examples/*.txt) examples/images shaders tools/starbox-creator/shaders
	$(MAKE) clean-all
	make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"'
	cd examples/terrainjumper && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"'
	rm examples/terrainjumper/*.o
	cd examples/terrainjumper && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"' terrainjumper-glfw -f Makefile.glfw
	cd tools/explorer && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"'
	cd tools/starbox-creator && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"'
	-rm -rf $(TMP)/celestial-tools-bin-$(VERSION)
	-mkdir $(TMP)/celestial-tools-bin-$(VERSION)
	cp -a $(ALL_LINUX_BINS) $(ALL_LINUX_LIBS) $^ $(TMP)/celestial-tools-bin-$(VERSION)
	cd $(TMP) && tar -c celestial-tools-bin-$(VERSION) | gzip -9 > celestial-tools-bin-$(VERSION).tar.gz

package-win32-bin: $(shell ls examples/*.txt) examples/images shaders tools/starbox-creator/shaders $(shell ls $(HOME)/win32-depends/lib/*.dll)
	$(MAKE) clean-all
	make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"' $(DLL)
	cd examples/terrainjumper && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"' terrainjumper.exe
	rm examples/terrainjumper/*.obj
	cd examples/terrainjumper && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"' terrainjumper-glfw.exe -f Makefile.glfw
	cd tools/explorer && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"' celestial-explorer.exe
	cd tools/starbox-creator && make -j $(NUMJOBS) OPT="-s -O2" ADD_CXXFLAGS='-DDEFAULT_SYSTEM_FILE=\"sol.txt\"' starbox-creator.exe

	-rm -rf $(TMP)/celestial-tools-win32-$(VERSION)
	-mkdir $(TMP)/celestial-tools-win32-$(VERSION)
	cp -a $(DLL) $(ALL_WIN32_BINS) $^ $(TMP)/celestial-tools-win32-$(VERSION)
	cd $(TMP) && zip -r -o -9 celestial-tools-win32-$(VERSION).zip celestial-tools-win32-$(VERSION)
