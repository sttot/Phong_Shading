#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

using namespace glm;
using namespace std;

class Shape
{
public:

	//////////////////////////////////////////////////////
	//													//
	// Virtuals enable all instances of shape classes	//
	// Default instances are below						//
	//													//
	//////////////////////////////////////////////////////

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

