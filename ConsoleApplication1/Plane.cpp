#include "stdafx.h"
#include "Plane.h"

Plane::Plane(vec3 pos, vec3 norm, vec3 colour)
{
	position = pos;
	normal = norm;
	shapeColour = colour;
}

Plane::Plane()
{
}

bool Plane::detect(vec3 ray, vec3 dir, vec3 collide[], float& dist)
{
	double test = dot(dir, normal);

	if (test <= 0.01)
	{
		return false;
	}
	else
	{
		t = dot((position - ray), normal) / test;

		collide[0] = ray + (t * dir);
	}
	return true;
}

vec3 Plane::colour(vec3 lighting[], vec3 collide[], vec3 dir, Shape *shapes[], vec3 camSpace, float& dist, int k)
{
	vec3 pixelColour;
	vec3 ambientColour, diffuseColour, specularColour;
	vec3 lightRay, pixelNormal;

	lightRay = lighting[0] - collide[0];
	pixelNormal = collide[0] - position;

	float shadowT = FLT_MAX;

	vec3 rayNorm = normalize(lightRay);	// Used in shadow creation
	vec3 pixelNorm = normalize(pixelNormal);

	double diffuseNormalise = dot(rayNorm, pixelNorm);

	if (diffuseNormalise < 0)
	{
		diffuseNormalise = -diffuseNormalise;
	}

	diffuseColour.x = lighting[1].x * 1.0 * max(0.0, diffuseNormalise);
	diffuseColour.y = lighting[1].y * 1.0 * max(0.0, diffuseNormalise);
	diffuseColour.z = lighting[1].z * 1.0 * max(0.0, diffuseNormalise);

	vec3 reflection;	// r = 2 (l dot n) n - l
	reflection = 2 * (dot(lightRay, pixelNormal)) * pixelNormal - lightRay;

	vec3 reflectNorm = normalize(reflection);
	vec3 dirNorm = normalize(dir);

	double specularNormalise = dot(reflectNorm, dirNorm);

	specularColour.x = lighting[2].x * 0.5 * pow(max(0.0, specularNormalise), 128);
	specularColour.y = lighting[2].y * 0.5 * pow(max(0.0, specularNormalise), 128);
	specularColour.z = lighting[2].z * 0.5 * pow(max(0.0, specularNormalise), 128);

	ambientColour = shapeColour * lighting[3];

	float shadowDistance;
	vec3 shadowCollisions[2];
	
	// For all Shapes
	for (int a(0); a < 5; a++)
	{
		if (a != k && t <= 0)
		{
			bool shadowRay = shapes[a]->detect(collide[0], lightRay, shadowCollisions, shadowDistance);
			if (shadowRay == true && shadowDistance < shadowT)
			{
				pixelColour = ambientColour;
			}
			else
				pixelColour = ambientColour + diffuseColour + specularColour;
		}
	}
	return pixelColour;
}

Plane::~Plane()
{
}
