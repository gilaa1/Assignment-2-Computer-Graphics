#include "myParser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <array>

myParser::myParser()
{
}

myParser::~myParser()
{
}

void myParser::parse(const std::string& filePath)
{
    std::ifstream file(filePath); // Open file for reading
    if (file.is_open()) {
        // File is open, read its contents
        char type;
        //code for parsing the file goes here read char
        while(file >> type) {
            switch (type) {
                case 'e': {
                    file >> std::fixed;
                    file >> eyePos.x >> eyePos.y >> eyePos.z >> bonus;
                    break;
                }
                case 'a': {
                    file >> ambient.x >> ambient.y >> ambient.z >> ambient.w;
                    break;
                }
                case 'd': {
                    glm::vec4 light;
                    file >> light.x >> light.y >> light.z >> light.w;
                    lightsDirection.push_back(light);
                    break;
                }
                case 'p': {
                    glm::vec4 spotlight;
                    file >> spotlight.x >> spotlight.y >> spotlight.z >> spotlight.w;
                    spotlightPos.push_back(spotlight);
                    break;
                }
                case 'i':{
				    glm::vec4 intensity;
				    file >> intensity.x >> intensity.y >> intensity.z >> intensity.w;
				    lightIntensity.push_back(intensity);
				    break;
			    }
                case 'o':{
                    std::array<float, 6> shape;
                    file >> shape[0] >> shape[1] >> shape[2] >> shape[3];
                    shape[4] = 0;
                    shape[5] = 0;
                    vectorOfShapes.push_back(shape);
				    break;
			    }
                case 't': {
                    std::array<float, 6> shape;
                    file >> shape[0] >> shape[1] >> shape[2] >> shape[3];
                    shape[4] = 1;
                    shape[5] = 0;
                    vectorOfShapes.push_back(shape);
                    break;
                }
                case 'r':{
                    std::array<float, 6> shape;
                    file >> shape[0] >> shape[1] >> shape[2] >> shape[3];
                    shape[4] = 0;
                    shape[5] = 1;
                    vectorOfShapes.push_back(shape);
                    break;
                    }
                case 'c': {
                    glm::vec4 color;
                    file >> color.x >> color.y >> color.z >> color.w;
                    colors.push_back(color);
                    break;
                }
            }
        }
        file.close(); // Close the file when done reading
    }
    else {
        // File couldn't be opened
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
    }
}

void myParser::print()
{
    std::cout << "Eye Position: " << eyePos.x << " " << eyePos.y << " " << eyePos.z << std::endl;
	std::cout << "Bonus: " << bonus << std::endl;
	std::cout << "Ambient: " << ambient.x << " " << ambient.y << " " << ambient.z << " " << ambient.w << std::endl;
    for (int i = 0; i < lightsDirection.size(); i++) {
		std::cout << "Light Direction: " << lightsDirection[i].x << " " << lightsDirection[i].y << " " << lightsDirection[i].z << " " << lightsDirection[i].w << std::endl;
	}
    for (int i = 0; i < spotlightPos.size(); i++) {
		std::cout << "Spotlight Position: " << spotlightPos[i].x << " " << spotlightPos[i].y << " " << spotlightPos[i].z <<" "<< spotlightPos[i].w << std::endl;
	}
    for (int i = 0; i < lightIntensity.size(); i++) {
		std::cout << "Light Intensity: " << lightIntensity[i].x << " " << lightIntensity[i].y << " " << lightIntensity[i].z << " " << lightIntensity[i].w << std::endl;
	}
    for (int i = 0; i < vectorOfShapes.size(); i++) {
        std::cout << "Shapes: " << vectorOfShapes[i][0] << " " << vectorOfShapes[i][1] << " " << vectorOfShapes[i][2] << " " << vectorOfShapes[i][3] << " " << vectorOfShapes[i][4] << " " << vectorOfShapes[i][5] << std::endl;
    }
	for (int i = 0; i < colors.size(); i++) {
    		std::cout << "Colors: " << colors[i].x << " " << colors[i].y << " " << colors[i].z << " " << colors[i].w << std::endl;
    }
}

std::vector<myShape*> myParser::getShapes()
{
    std::vector<myShape*> shapes;
    Sphere* s = nullptr;
    Plane* p = nullptr;

    //Print vectorOfShapes
 //   for (int i = 0; i < vectorOfShapes.size(); i++) {
	//	std::cout << "Shapes: " << vectorOfShapes[i][0] << " " << vectorOfShapes[i][1] << " " << vectorOfShapes[i][2] << " " << vectorOfShapes[i][3] << " " << vectorOfShapes[i][4] << " " << vectorOfShapes[i][5] << std::endl;
	//}
    while(!vectorOfShapes.empty()) {
        std::array<float, 6> shape = vectorOfShapes.back();
        if (shape[3] > 0) {
			s = new Sphere(glm::vec3(shape[0], shape[1], shape[2]), shape[3]);
		}
        else if (shape[3] <= 0) {
            p = new Plane(shape[0], shape[1], shape[2], shape[3]);
        }
        if (shape[4] == 1) {
            if (s) s->setTransparent();
            if (p) p->setTransparent();

		}
        if (shape[5] == 1) {
            if (s) s->setReflective();
            
            if (p) p->setReflective();

		}
        if (s) {
            s->setColorAndShininess(colors.back());
            colors.pop_back();
        }
        if (s) {
            shapes.push_back(s);
        }
        if (p) {
            p->setColorAndShininess(colors.back());
            colors.pop_back();
        }
        if (p) {
            shapes.push_back(p);
        }
		vectorOfShapes.pop_back();
        s = nullptr;
        p = nullptr;
    }
    //if (s) delete s;
    //if (p) delete p;
    return shapes;
}

glm::vec3 myParser::getEyePos()
{
    return eyePos;
}

glm::vec4 myParser::getAmbiant()
{
    return ambient;
}

std::vector<myLight*> myParser::getLights()
{
    std::vector<myLight*> lights;
    SpotLight* sl = nullptr;
    DirectionalLight* dl = nullptr;

    while (!lightsDirection.empty()) {
        glm::vec4 light = lightsDirection.back();
        if (light[3] == 1.0) {
            sl = new SpotLight();
            sl->setDirection(glm::vec3(light.x, light.y, light.z));
            sl->setIntensity(lightIntensity.back());
            lightIntensity.pop_back();
            sl->setPositionAndAlpha(spotlightPos.back());
            spotlightPos.pop_back();
            lights.push_back(sl);
        }
        else if (light[3] == 0.0) {
            dl = new DirectionalLight();
            dl->setDirection(glm::vec3(light.x, light.y, light.z));
            std::cout << "hey" << std::endl;
            dl->setIntensity(lightIntensity.back());
            std::cout << "hey1" << std::endl;
            lightIntensity.pop_back();
            std::cout << "hey2" << std::endl;
            lights.push_back(dl);

        }
        lightsDirection.pop_back();
		sl = nullptr;
        dl = nullptr;
    }
    return lights;
}

