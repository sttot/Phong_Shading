#pragma once

#include "Shape.h"

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

using namespace std;

class Plane : public Shape
{
public:
	vec3 position, normal, shapeColour;
	float planeSize, t;

	Plane(vec3 position, vec3 normal, vec3 colour);
	Plane();

	bool detect(vec3 ray, vec3 dir, vec3 collide[], float& dist);
	vec3 colour(vec3 lighting[], vec3 collide[], vec3 dir, Shape *shapes[], vec3 camSpace, float& dist, int k);

	~Plane();
};

	