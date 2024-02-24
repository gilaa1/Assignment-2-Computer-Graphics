#pragma once
#include <glm/glm.hpp>
#include "Ray.h"

class myLight
{
public:
	myLight();
	virtual bool getLightType() = 0;
	void setIntensity(glm::vec4 intensity);
	void setDirection(glm::vec3 direction);
	bool isSpotLight = 0;
	glm::vec3 getLightDirection() { return direction; }
	glm::vec4 getIntensity() { return intensity; }

protected:

private:
	glm::vec4 intensity;
	glm::vec3 direction;

};


class SpotLight : public myLight
{
public:
	SpotLight();
	//void setIntensity(glm::vec4 intensity) override;
	void setPositionAndAlpha(glm::vec4 vector);
	glm::vec3 getPosition();
	bool getLightType() override;
	float getAngel() { return alpha; }
	


protected:
private:

	glm::vec3 position;
	float alpha;

};

class DirectionalLight : public myLight
{
public:
	DirectionalLight();
	//void setIntensity(glm::vec4 intensity) override;
	bool getLightType() override;

protected:
private:

};

