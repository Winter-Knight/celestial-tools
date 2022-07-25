#ifndef PLAYER_H
#define PLAYER_H

#include <chrono>
#include "input.h"
#include "camera.hpp"
#include "terrain.h"

class Player {
public:
	void Init(Terrain * t, InputHandler * i);
	void handleJumps(std::chrono::duration<float> deltaTime);
	void maintainBounds();
	void updateCamera(std::chrono::duration<float> dt);
	celestial::Camera & getCamera() { return camera; }

private:
	celestial::Camera camera;
	Terrain * terrain;
	InputHandler * input;
};

#endif // PLAYER_H
