#include "stdafx.h"
#include "Sphere.h"

#define MAX_DEPTH 5

Sphere::Sphere(vec3 pos, vec3 colour, float rad)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;

	shapeColour.x = colour.x;	// Red
	shapeColour.y = colour.y;	// Green
	shapeColour.z = colour.z;	// Blue

	radius = rad;
}

// Make operation return true if there is a collision between the ray and a sphere
// Return false for any iteration where there is no collision with a sphere
// Pass through each sphere to determine if collision occurs with any sphere

bool Sphere::detect(vec3 ray, vec3 dir, vec3 collide[], float& dist)
{
	// First collision point, second collision point 
	vec3 firstCol, secondCol;
	vec3 collisionVecs[2] { firstCol, secondCol }; // Container array for looping

	float colOne = 0;
	float colTwo = 0;
	float collisions[2] { colOne, colTwo }; // Container array for looping

	// Vector storing scalar values of distance between ray and sphere origin
	// Equivalent to: l = c - o
	vec3 length;
	float lengthDist;
	length = position - ray;

	lengthDist = -(length.x + length.y + length.z);
	
	dist = lengthDist;

	// Equivalent to: t(ca) = l dot d
	float traj;
	traj = (length.x * dir.x) + (length.y * dir.y) + (length.z * dir.z); // Provides a scalar value that, if positive, means a collision is possible
	if (traj < 0)
	{
		return false; // If collision is not possible, return false
	}
	else
	{
		float sphereCentreRay;
		float lenSqu = (length.x * length.x) + (length.y * length.y) + (length.z * length.z);
		// Calculate scalar value that states distance of ray and it's direction away from sphere centre
		// If this value is larger than the radius, ray never intersects sphere
		// Equivalent to: s^2 = l^2 - t(ca)^2 --> s = sqrt(l^2 - t(ca)^2)
		sphereCentreRay = sqrt(lenSqu - (traj * traj));
		if (sphereCentreRay > radius)
		{
			return false;
		}
		else
		{
			float sphereCollision;
			// Equivalent to: t(hc) = sqrt(r^2 - s^2)
			sphereCollision = sqrt((radius * radius) - (sphereCentreRay * sphereCentreRay));

			colOne = traj - sphereCollision; // t(0) = t(ca) - t(hc)
			colTwo = traj + sphereCollision; // t(1) = t(ca) + t(hc)

			float collisionLength;

			if (colOne < colTwo)
				collisionLength = colOne;
			else
				collisionLength = colTwo;

			// Makes calculations work afaik
			collisions[0] = colOne;
			collisions[1] = colTwo;
			// Returns all values 

			for (int i(0); i < 2; i++)
			{
				// Returns vectors for collision points with scalar form:
				// p = o + t(n)d
				collisionVecs[i] = ray + (collisions[i] * dir);

				collide[i] = collisionVecs[i];

				// Shadow Calculation
				// n = (p - c) / ||p - c||
				// Using OpenGL Book Page 506
				vec3 dirNorm, normal, normAlter;
				normAlter.x = collisionVecs[i].x + (collisionVecs[i].x - position.x) * 1e-4;
				normAlter.y = collisionVecs[i].y + (collisionVecs[i].y - position.y) * 1e-4;
				normAlter.z = collisionVecs[i].z + (collisionVecs[i].z - position.z) * 1e-4;

				dirNorm = normalize(dir);							// V Ray Normalised
				normal = normalize(collisionVecs[0] - position);	// N Vector Normalised
				
				// Recurse call for reflections
				for (int r(0); r < 6; r++)
				{
					reflections(dir, normal);
				}

			}
		}
	}
	return true && dist;
}

vec3 Sphere::reflections(vec3 dir, vec3 norm)
{
	vec3 refRay;

	refRay = dir - (((dot(dir, norm)) * norm) * ((dot(dir, norm)) * norm));

	return refRay;
}

// Lighting array = position, diffuse, specular, ambient
// Collide takes in collision points of ray with object
vec3 Sphere::colour(vec3 lighting[], vec3 collide[], vec3 dir, Shape *shapes[], vec3 camSpace, float& dist, int k)
{
	vec3 pixelColour;
	// Ambient Calculation
	vec3 ambientColour, diffuseColour, specularColour;
	
	ambientColour = shapeColour * lighting[3];

	// Diffuse Calculation	
	vec3 lightRay, pixelNormal;	// l = s - p, n = p - c

	for (int i(0); i < 2; i++)
	{
		// l = s - p
		lightRay = lighting[0] - collide[i];		// Create light ray from point of intersection to light source
		pixelNormal = collide[i] - position;

		float shadowT = FLT_MAX;

		vec3 rayNorm = normalize(lightRay);	// Used in shadow creation
		vec3 pixelNorm = normalize(pixelNormal);

		double diffuseNormalise = dot(rayNorm, pixelNorm);

		diffuseColour.x = lighting[1].x * 0.3 * max(0.0, diffuseNormalise);
		diffuseColour.y = lighting[1].y * 0.3 * max(0.0, diffuseNormalise);
		diffuseColour.z = lighting[1].z * 0.3 * max(0.0, diffuseNormalise);

		vec3 reflection;	// r = 2 (l dot n) n - l
		reflection = 2 * (dot(lightRay, pixelNormal)) * pixelNormal - lightRay;

		vec3 reflectNorm = normalize(reflection);

		double specularNormalise = dot(reflectNorm, pixelNorm);

		specularColour.x = lighting[2].x * 1.0 * pow(max(0.0, specularNormalise), 128);
		specularColour.y = lighting[2].y * 1.0 * pow(max(0.0, specularNormalise), 128);
		specularColour.z = lighting[2].z * 1.0 * pow(max(0.0, specularNormalise), 128);

		pixelColour = ambientColour + diffuseColour + specularColour;

		float shadowDistance; 
		vec3 shadowCollisions[2];
		vec3 intersection;

		intersection.x = collide[i].x + (collide[i].x - position.x) * 1e-4;
		intersection.y = collide[i].y + (collide[i].y - position.y) * 1e-4;
		intersection.z = collide[i].z + (collide[i].z - position.z) * 1e-4;

		// For all Shapes
		for (int a(0); a < 5; a++)
		{
			if (a != k)
			{
				bool shadowRay = shapes[a]->detect(intersection, lightRay, shadowCollisions, shadowDistance);
				if (shadowRay == true && shadowDistance < shadowT)
				{
					pixelColour = ambientColour;
				}
				else
					pixelColour = ambientColour + diffuseColour + specularColour;
			}
		}
		
	}
	return pixelColour;
}

Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}
