#include "stdafx.h"
#include "Sphere.h"
#include "Shape.h"
#include "Plane.h"
#include "Triangle.h"

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <vector>

#define PI 3.14159265359

using namespace std;

float width = 640;
float height = 480;

// Pointer array
vec3 **image = new vec3*[(int)width];

// Top class pointer using virtual function
Shape* shapes[6]; 

void setup()
{
	//////////////////////////////////////////////////////////////////////
	//																	//
	// Spheres as (vec3 position, vec3 colour, float radius)			//
	// Triangles as (vec3 vert1, vec3 vert2, vec3 vert3, vec3 colour)	//
	// Plane as (vec3 position, vec3 normal, vec3 colour)				//
	//																	//
	//////////////////////////////////////////////////////////////////////

	shapes[0] = new Triangle(vec3(-10, 5, -10), vec3(-3, 5, -10), vec3(-10, 1, -10), vec3(0.0, 0.6, 1.0), vec3(-0.4, -0.4, -1.0), vec3(0.4, -0.4, 1.0), vec3(0.1, 0.1, 0.6));
	shapes[1] = new Plane(vec3(0, -20, -10), vec3(0, -1, 0), vec3(0.4, 0.4, 0.4));
	shapes[2] = new Sphere(vec3(-5, 1, -10), vec3 (1.00, 0.32, 0.36), 4);			// Red Sphere
	shapes[3] = new Sphere(vec3(4, 1, -15), vec3(0.90, 0.76, 0.46), 2);				// Yellow Sphere
	shapes[4] = new Sphere(vec3(5, 1, -5), vec3(0.65, 0.77, 0.97), 3);				// Blue Sphere
	shapes[5] = new Sphere(vec3(-8, 4, -10), vec3(0.80, 0.80, 0.80), 3);			// Light Grey Sphere
	shapes[6] = new Triangle(vec3(4, 6, -8), vec3(-4, 6, -8), vec3(4, -3, -8), vec3(0.0, 0.6, 1.0), vec3(-0.4, -0.4, -1.0), vec3(0.4, -0.4, 1.0), vec3(0.4, 0.4, 0.1));

	// Lighting Constraints
	vec3 lighting[4];
	lighting[0] = vec3(0.0, 100.0, 0);		// Position of the light source in the scene
	lighting[1] = vec3(0.5, 0.5, 0.5);		// Diffuse vector value
	lighting[2] = vec3(0.7, 0.7, 0.7);		// Specular vector value
	lighting[3] = vec3(0.5, 0.5, 0.5);		// Ambient light vector container - reference p504 in book

	double angle;
	angle = 90 * (PI / 180.0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//																									//
	// Sets up vector array with 2 dimensions using pointers											//
	// As image iterates through x values, iterate through y values before moving to next x value		//
	//																									//
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	for (int i(0); i < width; i++) {
		image[i] = new vec3[(int)height];
	}

	// Initialise loop to run through each pixel of the image, ready for Ray Casting
	for (int j(0); j < height; j++) {
		for (int i(0); i < width; i++) {
			// Ray Casting Loop
			double pixNormX = (i + 0.5) / width;
			double pixNormY = (j + 0.5) / height;
			// Aspect Ratio Calc
			float aspRatio = width / height;
			// Pixel Remap
			double pixRemapX = ((2 * pixNormX) - 1) * aspRatio;
			double pixRemapY = 1 - 2 * pixNormY;
			// FOV calc
			double pixCamX = (pixRemapX * tan(angle / 2.0));
			double pixCamY = (pixRemapY * tan(angle / 2.0));

			// Ray Calc
			vec3 pCamspace(pixCamX, pixCamY, -1);			// Take Camera space and distance away from cam for new Vector
			vec3 rayOrigin(0, 0, 0);						// Ray starts at camera's world position
			vec3 rayDirection = pCamspace - rayOrigin;		// Define direction of ray through normal calculation
			rayDirection = normalize(rayDirection);			// Normalize ray into unit vector	

			float minT = FLT_MAX;

			// Shape Creation and Ray Collision Detection Loop
			// k used for clarity in reading
			for (int k(0); k < 7; k++) {
				vec3 collisions[2];
				float shapeDistance;

				bool hitShape = shapes[k]->detect(rayOrigin, rayDirection, collisions, shapeDistance);		// Iterates through ray collisions per shape in the scene
				if (hitShape == true && shapeDistance < minT) {
					minT = shapeDistance;
					image[i][j] = shapes[k]->colour(lighting, collisions, rayDirection, shapes, pCamspace, shapeDistance, k);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////
	//															//
	// Save to a PPM image										//
	//															//
	// Loop to create the image that the file will take in		//
	// Top line titles the file name and extension				//
	//															//
	//////////////////////////////////////////////////////////////

	string filedir = "./";
	filedir.append("image");
	filedir.append(".ppm");
	ofstream ofs(filedir, ios::out | ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";	// Formatting, do not change
	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			ofs << (unsigned char)(fmin((float)1, (float)image[x][y].x) * 255) <<
				(unsigned char)(fmin((float)1, (float)image[x][y].y) * 255) <<
				(unsigned char)(fmin((float)1, (float)image[x][y].z) * 255);
		}
	}

	ofs.close();
};

int main() {
	setup();
	return 0;
}

