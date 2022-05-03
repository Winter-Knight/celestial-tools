#include <cstdio>
#include "window.h"
#include "input.h"
#include "world.h"

int main(int argc, char ** argv)
{
	Window * window = new Window();
	InputHandler * input = new InputHandler();
	World * world = new World(window, input);
	world->Init();
	world->Play();

	return 0;
}
