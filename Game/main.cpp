#include "InputManager.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include "Ray.h"
#include "myParser.h"
#include "myScene.h"
#include <iostream>
#include <fstream>


struct Point
{
	glm::vec3 position;
	glm::vec3 normal;
	int t = -1;
	int shape_i = -1;

};

Point* FindIntersection(Ray ray, myScene* scn, float near, float far, int currshape) {
	double t = far;
	int nearestShape = -1;
	for (int i = 0; i < scn->getShapes().size(); i++) {
		if (i != currshape) {
			double temp_t = scn->getShapes()[i]->intersect(ray);
			if (temp_t < t && temp_t > near) {
				t = temp_t;
				nearestShape = i;
			}
		}	
	}
	if (nearestShape != -1) {
		Point* p = new Point();
		p->position = ray.getOrigin() + (float)t * ray.getDirection();;
		p->normal = scn->getShapes()[nearestShape]->getNormal(p->position);
		p->t = t;
		p->shape_i = nearestShape;
		return p;
	}
	return nullptr;
}

float calcShadow(myScene* scn, myLight* light, Point* hit, glm::vec3 L, float near, float far) {
	float s = 1;
	Ray shadowRay = Ray(hit->position, L);
	Point* p = FindIntersection(shadowRay, scn, near, far, -1);
	if (p != nullptr && hit->shape_i != p->shape_i)
		s = 0;
	return s;
}

glm::vec3 calcDiffuseColor(myScene* scn, Point* hit, myLight* light, Ray ray) {
	glm::vec3 intensity = glm::vec3(light->getIntensity().x, light->getIntensity().y, light->getIntensity().z);
	glm::vec3 lightDirection = glm::normalize(light->getLightDirection());
	glm::vec3 color = glm::vec3(0, 0, 0);
	if (hit == nullptr)
		return color;
	else {
		glm::vec3 N = hit->normal; //intersection normal (N)
		glm::vec3 Kd = scn->getShapes()[hit->shape_i]->getColor(hit->position);
		if (light->isSpotLight) {
			SpotLight* spotLight = (SpotLight*)light;
			float cosAngel = spotLight->getAngel();
			glm::vec3 L = spotLight->getPosition() - hit->position; //intersection to light (L)
			glm::vec3 normalizeL = glm::normalize(-L); //intersection to light (L)
			float cosTheta = glm::dot(normalizeL, lightDirection);

			if (cosTheta >= cosAngel) {
				float NL = (float)std::max(glm::dot(glm::normalize(N), -normalizeL), 0.0f);
				float s = calcShadow(scn, light, hit, -normalizeL, 0.01f, glm::length(L));
				color = (Kd * NL * intensity) * s;
			}
		}
		else {
			glm::vec3 L = glm::normalize(light->getLightDirection()); //intersection to light (L)
			float NL = (float)std::max(glm::dot(glm::normalize(N), -L), 0.0f);
			float s = calcShadow(scn, light, hit, -L, 0.01f, 10000.0f);
			color = (Kd * NL * intensity) * s;
		}
		color = glm::clamp(color, 0.0f, 1.0f);
		return color;
	}
}

glm::vec3 calcSpecularColor(myScene* scn, Point* hit, myLight* light, Ray ray) {
	glm::vec3 intensity = glm::vec3(light->getIntensity().x, light->getIntensity().y, light->getIntensity().z);
	glm::vec3 lightDirection = glm::normalize(light->getLightDirection());
	glm::vec3 color = glm::vec3(0, 0, 0);
	if (hit == nullptr)
		return color;
	else {
		float shininess = scn->getShapes()[hit->shape_i]->getShininess();
		glm::vec3 N = hit->normal; //intersection normal (N)
		glm::vec3 V = glm::normalize(ray.getOrigin() - hit->position); //intersection to eye (V)
		float Ks = 0.7;
		if (light->isSpotLight) {
			SpotLight* spotLight = (SpotLight*)light;
			float cosAngel = spotLight->getAngel();
			glm::vec3 L = spotLight->getPosition() - hit->position; //intersection to light (L)
			glm::vec3 normalizeL = glm::normalize(L); //intersection to light (L)
			float cosTheta = glm::dot(-normalizeL, lightDirection);

			if (cosTheta >= cosAngel) {
				glm::vec3 R = glm::normalize(glm::reflect(-normalizeL, N));
				float VR = (float)std::max(glm::dot(V, R), 0.0f);
				float s = calcShadow(scn, light, hit, normalizeL, 0.01f, glm::length(L));
				color = Ks * pow(VR, shininess) * intensity * s;
			}
		}
		else {
			glm::vec3 L = glm::normalize(light->getLightDirection()); //intersection to light (L)
			glm::vec3 R = glm::normalize(glm::reflect(L, N));
			float VR = (float)std::max(glm::dot(V, R), 0.0f);
			float s = calcShadow(scn, light, hit, -L, 0.01f, 10000.0f);
			color = Ks * pow(VR, shininess) * intensity * s;
		}
		color = glm::clamp(color, 0.0f, 1.0f);
		return color;
	}
}

Ray ConstructoutRayReflactive(Ray ray, Point* hit) {
	glm::vec3 normalizeDir = glm::normalize(ray.getDirection());
	glm::vec3 ReflectiveRay = glm::reflect(normalizeDir, hit->normal);
	ReflectiveRay = glm::normalize(ReflectiveRay);
	Ray reflectedRay = Ray(hit->position, ReflectiveRay);
	return reflectedRay;
}

Ray ConstructoutRayTransparent(myScene* scn, Ray ray, Point* hit) {
	int currentShape = hit->shape_i;
	if (!(scn->getShapes()[currentShape]->getIsPlane())) {
		glm::vec3 refracted_direction = glm::refract(glm::normalize(ray.getDirection()), hit->normal, 1.0f / 1.5f);
		refracted_direction = glm::normalize(refracted_direction);
		Ray refractedRay = Ray(hit->position + 0.0001f * refracted_direction, refracted_direction);

		float t = scn->getShapes()[currentShape]->intersect(refractedRay);
		glm::vec3 hit2 = refractedRay.getOrigin() + t * refractedRay.getDirection();
		glm::vec3 N2 = scn->getShapes()[currentShape]->getNormal(hit2);

		glm::vec3 refracted_direction2 = glm::refract(refractedRay.getDirection(), -N2, 1.5f / 1.0f);
		refracted_direction2 = glm::normalize(refracted_direction2);
		return Ray(hit2 + 0.0001f * refracted_direction2, refracted_direction2);
	}
	else {
		glm::vec3 refracted_direction = glm::refract(ray.getDirection(), hit->normal, 1.0f / 1.5f);
		return Ray(hit->position + 0.0001f * refracted_direction, refracted_direction);
	}
}

glm::vec3 calcColor(myScene* scn, Ray ray, int level, int currshape) {
	if (level == 5)
		return glm::vec3(0, 0, 0);

	Point* hit = FindIntersection(ray, scn, 0.0f, 1000.0f, currshape);
	glm::vec3 color = glm::vec3(0, 0, 0);
	if (hit != nullptr) {
		 
		if (!scn->getShapes()[hit->shape_i]->getReflective() && !scn->getShapes()[hit->shape_i]->getTransparent()) {
			color = scn->getShapes()[hit->shape_i]->getColor(hit->position) * scn->getAmbient();
			for (int i = 0; i < scn->getLights().size();i++) {
				myLight* light = scn->getLights()[i];
				color += calcDiffuseColor(scn, hit, light, ray) +
					calcSpecularColor(scn, hit, light, ray);
				color = glm::clamp(color, 0.0f, 1.0f);
			}
		}
		
		if (scn->getShapes()[hit->shape_i]->getReflective()) {
			Ray outRay = ConstructoutRayReflactive(ray, hit);
			color = calcColor(scn, outRay, level + 1, -1);
			color = glm::clamp(color, 0.0f, 1.0f);
		}
		if (scn->getShapes()[hit->shape_i]->getTransparent()) {
			Ray outRay = ConstructoutRayTransparent(scn, ray, hit);
			color += calcColor(scn, outRay, level + 1, -1);
			color = glm::clamp(color, 0.0f, 1.0f);
		}
	}
	return color;
}

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 800;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
	
	scn->Init();

	myParser* parser = new myParser();
	myScene* scene = new myScene();
	parser->parse("../res/textures/scene5.txt");
	glm::vec3 eyePos = parser->getEyePos();
	glm::vec4 ambiant = parser->getAmbiant();
	scene->setAmbient(glm::vec3(ambiant.x, ambiant.y, ambiant.z));
	std::vector<myShape*> scene1 = parser->getShapes();
	while (!scene1.empty()) {
		scene->addShape(scene1.back());
		scene1.pop_back();
	}
	std::vector<myLight*> lights = parser->getLights();
	while (!lights.empty()) {
		scene->addLight(lights.back());
		lights.pop_back();
	}

	unsigned char* image = new unsigned char[DISPLAY_HEIGHT * DISPLAY_WIDTH * 4];
	for (int y = 0; y < DISPLAY_HEIGHT; y = y + 1) {
		for (int x = 0; x < DISPLAY_WIDTH; x = x + 1) {
			float xPos = 2.0 / DISPLAY_WIDTH * x - 1.0 - (1.0 / DISPLAY_WIDTH);
			float yPos = 2.0 / DISPLAY_HEIGHT * y - 1.0 - (1.0 / DISPLAY_HEIGHT);
			float zPos = 0;
			Ray ray = Ray::constructRayThroughPixel(eyePos, xPos, -yPos, zPos);
			glm::vec3 color = calcColor(scene, ray, 0, -1);
			image[(x + y * DISPLAY_WIDTH) * 4] = color.x * 255;
			image[(x + y * DISPLAY_WIDTH) * 4 + 1] = color.y * 255;
			image[(x + y * DISPLAY_WIDTH) * 4 + 2] = color.z * 255;
			image[(x + y * DISPLAY_WIDTH) * 4 + 3] = 255;
		}
	}

	display.SetScene(scn);
	scn->AddTexture(DISPLAY_HEIGHT, DISPLAY_WIDTH, image);
	scn->SetShapeTex(0, 1);
	scn->Draw(1, 0, scn->BACK, true, false);
	scn->Motion();
	display.SwapBuffers();


	while(!display.CloseWindow())
	{
		display.PollEvents();	
			
	}
	delete scn;
	return 0;
}
