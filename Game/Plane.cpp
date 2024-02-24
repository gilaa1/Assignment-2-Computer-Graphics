#include "myShape.h"
#include <iostream>
Plane::Plane(float a, float b, float c, float d)
{
	this->normal = glm::vec3(a, b, c);
	this->distance = d;
	this->isPlane = 1;
}

float Plane::intersect(const Ray& ray) 
{
	//std::cout << "Plane intersect" << std::endl;
	glm::vec3 origin = ray.getOrigin();
	glm::vec3 direction = ray.getDirection();
	float t = -(distance + glm::dot(normal, origin)) / glm::dot(normal, direction);
	//std::cout << "t: " << t << std::endl;
	if (t < 0)
	{
		return -1.0f;
	}
	else
	{
		float threshold = 0.001f;
		if (abs(t) <= threshold) {
			t = -1;
		}
		return t;
	}
}

glm::vec3 Plane::getColor(glm::vec3 hitPoint) {
	//std::cout << "getColorPlane" << std::endl;
	float scale = 0.5f;
	float chessBoard = 0;

	if (hitPoint.x < 0) {
		chessBoard += floor((0.5 - hitPoint.x) / scale);
	}

	else {
		chessBoard += floor(hitPoint.x / scale);
	}

	if (hitPoint.y < 0) {
		chessBoard += floor((0.5 - hitPoint.y) / scale);
	}

	else {
		chessBoard += floor(hitPoint.y / scale);
	}

	chessBoard = (chessBoard*0.5) - int(chessBoard*0.5);
	chessBoard *= 2;
	if (chessBoard > 0.5) {
		return 0.5f * this->color;
	}

	return this->color;
	
}


glm::vec3 Plane::getNormal(glm::vec3 hit) {
	return -(glm::normalize(this->normal));
}