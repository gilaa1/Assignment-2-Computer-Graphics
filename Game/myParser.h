#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "myShape.h"
#include "myLight.h"

class myParser
{
public:
	myParser();
	~myParser();
	void parse(const std::string& filePath);
	void print();
	std::vector<myShape*> getShapes();
	glm::vec3 getEyePos();
	glm::vec4 getAmbiant();
	std::vector<myLight*> getLights();


	protected:
	
	private:
		glm::vec3 eyePos = glm::vec3(0.0f, 0.0f, 0.0f);
		bool bonus;
		glm::vec4 ambient;
		std::vector <glm::vec4> lightsDirection;
		std::vector <glm::vec4> spotlightPos;
		std::vector <glm::vec4> lightIntensity;
		std::vector<std::array<float, 6>> vectorOfShapes;
		std::vector <glm::vec4> colors;
		std::vector <Plane> Planes;
		std::vector <Sphere> Spheres;





};