#include <cstdio>
#include "world.h"
#include "window.h"

int main(int argc, char ** argv)
{
	const char * systemFile;

	if (argc < 2)
		systemFile = "flips.txt";
	else
		systemFile = argv[1];

	Window * window = new Window();
	InputHandler * input = new InputHandler();
	World * world = new World(window, input);
	world->Init(systemFile);
	world->Play();

	return 0;
}
