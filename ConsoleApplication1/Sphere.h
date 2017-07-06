#pragma once

#include "Shape.h"

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

using namespace std;

class Sphere : public Shape
{
public:
	float radius;
	vec3 position, shapeColour;
	float colDist;

	Sphere(vec3 pos, vec3 colour, float rad);
	Sphere();

	bool detect(vec3 ray, vec3 dir, vec3 collide[], float& dist);
	vec3 reflections(vec3 dir, vec3 norm);
	vec3 colour(vec3 lighting[], vec3 collide[], vec3 dir, Shape *shapes[], vec3 camSpace, float& dist, int k);

	~Sphere();
};

