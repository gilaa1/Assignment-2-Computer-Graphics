#pragma once
#include "myShape.h"
#include <iostream>

//constructor
myShape::myShape()
{

}

void myShape::setColorAndShininess(glm::vec4 color)
{
	this->color.x = color.x ;
	this->color.y = color.y ;
	this->color.z = color.z ;
	this->shininess = color.w;
}


