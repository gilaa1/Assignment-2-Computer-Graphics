//#include <cmath>
#include "myLight.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

SpotLight::SpotLight()
{
	this->position = glm::vec3(0, 0, 0);
	this->alpha = 0;
	this->isSpotLight = 1;
}


//void SpotLight::setIntensity(glm::vec4 intensity)
//{
//	this->intensity = intensity;
//}

void SpotLight::setPositionAndAlpha(glm::vec4 vector)
{
	this->position = glm::vec3(vector.x, vector.y, vector.z);
	this->alpha = vector.w;
	std::cout << "alpha: " << alpha << std::endl;

}

glm::vec3 SpotLight::getPosition()
{
	return this->position;
}

bool SpotLight::getLightType()
{
	return true;
}

