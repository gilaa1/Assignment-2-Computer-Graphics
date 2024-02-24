#pragma once
#include "myLight.h"

myLight::myLight()
{
}

void myLight::setIntensity(glm::vec4 intensity)
{
		this->intensity = intensity;
}

void myLight::setDirection(glm::vec3 direction)
{
		this->direction = direction;
}

