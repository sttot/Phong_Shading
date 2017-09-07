#include "stdafx.h"
#include "Triangle.h"

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 n1, vec3 n2, vec3 n3, vec3 col) {
	
	pOne = p1;
	pTwo = p2;
	pThree = p3;

	normOne = n1;
	normTwo = n2;
	normThree = n3;

	shapeColour.x = col.x;
	shapeColour.y = col.y;
	shapeColour.z = col.z;
}

Triangle::Triangle(){}

bool Triangle::detect(vec3 ray, vec3 dir, vec3 collide[], float& dist) {
	
	vec3 edgeOne, edgeTwo;	// Used to calculate e1 and e2 in barycentric equation

	// Co-Ordinate Calculation
	edgeOne = pTwo - pOne;		// e1 = b - a
	edgeTwo = pThree - pOne;	// e2 = c - a
	
	float denominator = dot(edgeOne, cross(dir, edgeTwo));
	u = dot((ray - pOne), cross(dir, edgeTwo)) / denominator;	// u = ((o - a) dot (d x e2)) / e1 dot (d x e2)
	v = dot(dir, (cross(ray - pOne, edgeOne))) / denominator;	// v = d dot ((o - a) x e1) / e1 dot (d x e2)
	
	w = 1 - u - v; 	// 3rd Barycentric Co-Ordinate

	t = dot(edgeTwo, (cross(ray - pOne, edgeOne))) / denominator;

	// Check if point is in triangle
	if (u < 0 || u > 1) {
		// u = False
		return false;
	}
	else if (v < 0 || u + v > 1) {
		// u + v & False 
		return false;
	}
	else
		// u + v < 1 & True
		collide[0] = (u * pOne) + (v * pTwo) + (w * pThree);
	return true;
}

//////////////////////////
//						//
// Colour calculation	//
//						//
//////////////////////////

vec3 Triangle::colour(vec3 lighting[], vec3 collide[], vec3 dir, Shape *shapes[], vec3 camSpace, float& dist, int k) {
	
	vec3 pixelColour;
	vec3 ambientColour, diffuseColour, specularColour;
	vec3 lightRay;

	lightRay = lighting[0] - collide[0];

	float shadowT = FLT_MAX;

	vec3 rayNorm = normalize(lighting[0] - collide[0]);
	vec3 triNorm = normalize((w * normOne) + (u * normTwo) + (v * normThree));

	double diffuseNormalise = dot(rayNorm, triNorm);

	ambientColour = shapeColour * lighting[3];

	diffuseColour.x = lighting[1].x * 1.0 * max(0.0, diffuseNormalise);
	diffuseColour.y = lighting[1].y * 1.0 * max(0.0, diffuseNormalise);
	diffuseColour.z = lighting[1].z * 1.0 * max(0.0, diffuseNormalise);

	vec3 reflection = 2 * (dot(lightRay, triNorm)) * triNorm - lightRay;

	vec3 reflectNorm = normalize(reflection);
	vec3 dirNorm = normalize(dir);

	double specularNormalise = dot(reflectNorm, dirNorm);

	specularColour.x = lighting[2].x * 0.5 * pow(max(0.0, specularNormalise), 128);
	specularColour.y = lighting[2].y * 0.5 * pow(max(0.0, specularNormalise), 128);
	specularColour.z = lighting[2].z * 0.5 * pow(max(0.0, specularNormalise), 128);
	
	pixelColour = ambientColour + diffuseColour + specularColour;

	float shadowDistance;
	vec3 shadowCollisions[2];

	// For all Shapes
	for (int a(0); a < 5; a++) {
		if (a != k && t <= 0) {
			bool shadowRay = shapes[a]->detect(collide[0], lightRay, shadowCollisions, shadowDistance);
			if (shadowRay == true && shadowDistance < shadowT) {
				pixelColour = ambientColour + diffuseColour + specularColour;
			}
			else
				pixelColour = ambientColour;
		}
	}
	

	return pixelColour;
}

Triangle::~Triangle() {}
