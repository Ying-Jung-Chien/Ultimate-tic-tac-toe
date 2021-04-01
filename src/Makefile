CXX = g++-8
CXXFLAG = -std=c++17 -O2 -Wall -Werror -Wextra -pedantic-errors -fPIC
GMAEINCLUDE = ./
BUILDDIR = ./build

all: gamerunner AI

AI:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -shared AITemplate/Porting.cpp -o ${BUILDDIR}/AI.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a1.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a2.so

ALLEGRO_LIBRARIES := allegro-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5
ALLEGRO_FLAGS := $(shell pkg-config --cflags --libs $(ALLEGRO_LIBRARIES))
gamerunner:    
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -pthread main.cpp -o ${BUILDDIR}/gamerunner -ldl $(ALLEGRO_FLAGS)

.PHONY: clean
clean:
	-rm gamerunner a1.so
