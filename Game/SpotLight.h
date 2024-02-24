#pragma once
#include <glm/glm.hpp>


class SpotLight
{
public:
	SpotLight(glm::vec3 direction);
	void setIntensity(glm::vec4 intensity);
	void setPosition(glm::vec3 position);




protected:
private:
	glm::vec3 direction;
	glm::vec4 intensity;
	glm::vec3 position;

};

