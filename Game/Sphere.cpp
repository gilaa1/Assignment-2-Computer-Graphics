#include "myShape.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>


Sphere::Sphere(glm::vec3 centerPosition, float radius)
{
	this->centerPosition = centerPosition;
	this->radius = radius;
	this->isPlane = 0;
}

float Sphere::intersect(const Ray& ray) 
{
	glm::vec3 origin = ray.getOrigin();
	glm::vec3 direction = ray.getDirection();
	glm::vec3 oc = origin - centerPosition;
	float a = glm::dot(direction, direction);
	float b = 2.0f * glm::dot(oc, direction);
	float c = glm::dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return -1.0f;
	}
	else if (discriminant == 0)
	{
		return -b / (2.0f * a);
	}
	else
	{
		float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
		float t2 = (-b - sqrt(discriminant)) / (2.0f * a);
		if (t1 < 0 && t2 < 0)
		{
			return -1.0f;
		}
		else if (t1 < 0)
		{
			return t2;
		}
		else if (t2 < 0)
		{
			return t1;
		}

		// Handle Reflective and Transparent case
		float threshold = 0.001f;

		float result =  std::min(t1, t2);	

		if (abs(result) <= threshold) {
			result = glm::max(t1, t2);
		}

		return result;
	}
}

glm::vec3 Sphere::getColor(glm::vec3 hitPoint)
{
	return this->color;
}

glm::vec3 Sphere::getNormal(glm::vec3 hit)
{
	return glm::normalize(hit - centerPosition);
}
