#include "Ray.h"

//constructor
Ray::Ray(const glm::vec3& origin1, const glm::vec3& direction1)
{
	origin = origin1;
	direction = direction1;
}

 Ray Ray::constructRayThroughPixel(glm::vec3 eyePos, float xPos, float yPos, float zPos)
{
	glm::vec3 direction = glm::normalize(glm::vec3(xPos-eyePos[0], yPos- eyePos[1], zPos - eyePos[2]));
	Ray ray = Ray(eyePos, direction);
	return ray;
}

 glm::vec3 Ray::getOrigin() const
 {
	 return origin;
 }

 glm::vec3 Ray::getDirection() const
 {
	 return direction;
 }


