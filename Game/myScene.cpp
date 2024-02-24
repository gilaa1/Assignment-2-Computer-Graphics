#include "myScene.h"


//constructor
myScene::myScene()
{
	
}
void myScene::addShape(myShape* shape)
{
	shapes.push_back(shape);
}

std::vector<myShape*> myScene::getShapes()
{
	return shapes;
}

void myScene::addLight(myLight* light)
{
	lights.push_back(light);
}
std::vector<myLight*> myScene::getLights()
{
	return lights;
}

glm::vec3 myScene::getAmbient()
{
	return ambient;
}

void myScene::setAmbient(glm::vec3 ambient)
{
		this->ambient = ambient;
}






