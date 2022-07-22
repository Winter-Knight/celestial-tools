#include "player.h"
#include "compat.h"

void Player::Init(Terrain * t, InputHandler * i)
{
	terrain = t;
	input = i;
	camera.pos = glm::vec3(Terrain::terrainWidth / 2.0f, 275.0f, Terrain::terrainWidth / 2.0f);
	camera.dir = glm::vec3(1.0f, 0.0f, 0.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.CalculateView();
	
	camera.fov = glm::radians(60.0f);
	camera.aspect = float(INITIAL_WINDOW_WIDTH) / float(INITIAL_WINDOW_HEIGHT);
	camera.near = 0.1f;
	camera.far = 1000.0f;
	camera.CalculatePerspective();
}

void Player::handleJumps(std::chrono::duration<float> deltaTime)
{
	float gravity = -9.8f * 30.0f;
	float height = camera.pos.y;
	float ground = terrain->getHeight(glm::vec2(camera.pos.x, camera.pos.z)) + 15.0f;
	bool inAir = height > ground + 1.0f;
	static float speed = 0.0f;
	const float jumpStrength = 100.0f;

	if (inAir)
		speed += gravity * deltaTime.count();

	if (input->lastKey == KEY_SPACE && !inAir) {
		speed = glm::max(speed, 0.0f);
		speed += jumpStrength;
	}

	height += speed * deltaTime.count();

	camera.pos.y = glm::max(height, ground);
}

void Player::maintainBounds()
{
	glm::vec3 pos = camera.pos;
	if (pos.x < 0.0f)
		pos.x += Terrain::terrainWidth;
	if (pos.x > Terrain::terrainWidth)
		pos.x -= Terrain::terrainWidth;
	if (pos.z < 0.0f)
		pos.z += Terrain::terrainWidth;
	if (pos.z > Terrain::terrainWidth)
		pos.z -= Terrain::terrainWidth;
	camera.pos = pos;
}

void Player::updateCamera(std::chrono::duration<float> dt) // time in seconds
{
	// Begin
	const float speed = 30.0f; // units / second
	const float mouseSpeed = 0.005f;
	const float MAX_VERTICAL_ANGLE = M_PI / 2.0f * 0.99f;
	float deltaTime = dt.count();

	glm::vec3 dir = camera.dir;
	glm::vec3 up = camera.up;
	glm::vec3 right = glm::cross(dir, camera.up);
	glm::vec3 pos = camera.pos;

	// Keyboard
	
	const unsigned char * state = GetKeyboardState(NULL);

	if (state[LSHIFT] || state[RSHIFT])
		deltaTime *= 5;
	if (state[LALT] || state[RALT])
		deltaTime /= 10;

	if (state[KEY_W])
			pos += dir * (deltaTime * speed);
	if (state[KEY_D])
			pos += right * (deltaTime * speed);
	if (state[KEY_S])
			pos -= dir * (deltaTime * speed);
	if (state[KEY_A])
			pos -= right * (deltaTime * speed);

	// Mouse
	glm::vec3 horizontalDirection = glm::normalize(glm::vec3(dir.x, 0.0f, dir.z));

	float horizontalAngle = acos(horizontalDirection.x);
	if (dir.z > 0.0f)
		horizontalAngle = M_PI * 2.0f - horizontalAngle;

	float verticalAngle = asin(dir.y);
	
	if (!input->buttonMask) {
	// Yaw (left and right)
		horizontalAngle -= ((float) input->xrel) / 400.0f;
	// Pitch (up and down)
		verticalAngle += ((float) input->yrel) / 400.0f;
		if (verticalAngle > MAX_VERTICAL_ANGLE)
			verticalAngle = MAX_VERTICAL_ANGLE;
		if (verticalAngle < -MAX_VERTICAL_ANGLE)
			verticalAngle = -MAX_VERTICAL_ANGLE;
	}

	dir.x = cos(horizontalAngle);
	dir.z = -sin(horizontalAngle);
	dir.y = sin(verticalAngle);

	// Finish

	camera.pos = pos;
	camera.dir = dir;
	camera.up = up;
	camera.CalculateView();
	
	if (input->windowEvent) {
		camera.aspect = float(input->windowWidth) / float(input->windowHeight);
		camera.CalculatePerspective();
	}
}
