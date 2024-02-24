#pragma once
#include <glm/glm.hpp>
#include <camera.h>

class Ray 
{
public:
	Ray(const glm::vec3& origin1, const glm::vec3& direction1);
	static Ray constructRayThroughPixel(glm::vec3 eyePos, float xPos, float yPos, float zPos);
	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;

private:
	glm::vec3 origin;
	glm::vec3 direction;
};


