#pragma once

#include "Shape.h"

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

using namespace std;

class Triangle : public Shape
{
public:
	vec3 pOne, pTwo, pThree; // Vector containers for each of the 3 vertices of the triangle
	vec3 normOne, normTwo, normThree; // Vectors containing position of normals 
	vec3 shapeColour; // Triangle Colour
	float u, v, w, t;

	Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 n1, vec3 n2, vec3 n3, vec3 col);
	Triangle();

	bool detect(vec3 ray, vec3 dir, vec3 collide[], float& dist);
	vec3 colour(vec3 lighting[], vec3 collide[], vec3 dir, Shape *shapes[], vec3 camSpace, float& dist, int k);

	~Triangle();
};

