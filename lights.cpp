#include "lights.h"

void LightArray::AddLight(glm::vec4 pos, glm::vec4 color)
{
	lights.push_back(LightSource{pos, color});
}
