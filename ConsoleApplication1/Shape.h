#pragma once

//#include "Sphere.h" - Do not inlcude below functions as this will cause circular dependencies 

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

using namespace glm;	// Keep this in - all primitive classes use for vec3 def
using namespace std;

class Shape
{
public:
	virtual bool detect(vec3 ray, vec3 dir, vec3 collide[], float& dist)
	{
		std::cout << "Passed" << std::endl;
		return true;
	} 

	virtual vec3 reflections(vec3 dir, vec3 norm)
	{
		return vec3(0, 0, 0);
	}

	virtual vec3 colour(vec3 lighting[], vec3 collide[], vec3 dir, Shape *shapes[], vec3 camSpace, float& dist, int k)
	{
		return vec3(0, 0, 0);
	}

	Shape();
	~Shape();
};

