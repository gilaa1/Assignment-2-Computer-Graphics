#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class myShape
{
public:
	myShape();
	void setColorAndShininess(glm::vec4 color);
	virtual glm::vec3 getColor(glm::vec3 hit) = 0;
	virtual float intersect(const Ray& ray) = 0;;
	void setTransparent() { transparent = 1; }
	void setReflective() { reflective = 1; }
	float getShininess() { return shininess; }
	bool getTransparent() { return transparent; }
	bool getReflective() { return reflective; }
	bool getIsPlane() { return isPlane; }
	virtual glm::vec3 getNormal(glm::vec3 hit) = 0;

protected:
	bool isPlane = 0;
	glm::vec3 color;
	bool transparent = 0;
	bool reflective = 0;


private:
	float shininess;
	glm::vec3 specular;


};


class Sphere : public myShape

{
public:
	Sphere(glm::vec3 centerPosition, float radius);
	float intersect(const Ray& ray) override;
	glm::vec3 getColor(glm::vec3 hitPoint) override;
	glm::vec3 getNormal(glm::vec3 hit) override;

protected:
private:
	glm::vec3 centerPosition;
	float radius;
};


class Plane : public myShape
{
public:
	Plane(float a, float b, float c, float d);
	float intersect(const Ray& ray) override;
	glm::vec3 getColor(glm::vec3 hitPoint) override;
	glm::vec3 getNormal(glm::vec3 hit) override;

protected:
private:
	glm::vec3 normal;
	float distance;
};
