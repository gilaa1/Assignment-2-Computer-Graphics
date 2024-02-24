#include <glm/glm.hpp>
#include <vector>
#include "myShape.h"
#include "myLight.h"

class myScene 
{
public:
	myScene();
	void myScene::addShape(myShape* shape);
	std::vector<myShape*> getShapes();
	void addLight(myLight* light);
	std::vector<myLight*> getLights();
	glm::vec3 getAmbient();
	void setAmbient(glm::vec3 ambient);


private:
	std::vector<myShape*> shapes;
	std::vector<myLight*> lights;
	glm::vec3 ambient;

};


