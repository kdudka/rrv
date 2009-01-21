#include "Entity.h"
#include "BarrelEntity.h"
#include <math.h>
#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

using namespace XML;

void BarrelEntity::impl_deserialize( XMLNode *from ) {
	setName(from, "barrel");
	this->polygonize();
}

void BarrelEntity::polygonize ( ) {
	const int PLANE_COUNT = 20;
	const float RADIUS = 0.5;

	Vertex v[4];
	for(int i=0; i<PLANE_COUNT+1; i++)
	{
		v[3] = v[2];
		v[0] = v[1];
		// compute coords (http://en.wikipedia.org/wiki/Sphere)
		float angle = 2 * M_PI / PLANE_COUNT * i;
		float x = RADIUS * cos(angle);
		float z = RADIUS * sin(angle);
		v[2].x = v[1].x = x;
		v[2].z = v[1].z = z;
		v[2].y = -0.5;
		v[1].y = +0.5;
		if(0 != i)
		{
			// face
			this->addQuad(v);
			// upper and lower base
			this->addBaseTriangles(v);
		}
	}
}

void BarrelEntity::addBaseTriangles(Vertex vertex[4])
{
	// Triangle initialization
	Triangle t;
	t.emission = emission_;
	t.reflectivity = reflectivity_;

	Vertex v;
	for(int j=0; j<4; j+=2)
	{
		for (int i=j+0; i<j+2; i++)
		{
			t.vertex[i-j] = vertex[2*j+1-i];
			v.x=v.z=0;
			v.y=vertex[i].y;
			t.vertex[i+1-j]=v;
		}
		this->addTriangle(&t);
	}
}

void BarrelEntity::addQuad(Vertex vertex[4])
{
	// Triangle initialization
	Triangle t;
	t.emission = emission_;
	t.reflectivity = reflectivity_;
	
	// Add triangle #1
	for (int i=0; i<3; i++)
		t.vertex[i] = vertex[i];
	this->addTriangle(&t);
	
	// Add triangle #2
	t.vertex[1] = vertex[2];
	t.vertex[2] = vertex[3];
	this->addTriangle(&t);
}
