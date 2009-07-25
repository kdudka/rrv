/*
 * Copyright (C) 2007 TODO
 *
 * This file is part of rrv (Radiosity Renderer and Visualizer).
 *
 * rrv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * rrv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rrv.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GlobeEntity.h"

#include <math.h>
#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

using namespace XML;

void GlobeEntity::impl_deserialize (XMLNode *from ) {
	setName( from, "globe" );
	this->polygonize();
}

void GlobeEntity::polygonize ( ) {
	const int PLANE_COUNT = 20;
	const float RADIUS = 0.5;

	Vertex lastV[PLANE_COUNT+1+1+1];
	Vertex thisV(0.0f, 0.0f, 0.0f), now;

	Vertex v[4];
	for(int j=0; j<PLANE_COUNT/2+1; j++)
	{
		float angle = 2 * M_PI / PLANE_COUNT * j;
		float x0 = (RADIUS) * cos(angle);
		float z0 = (RADIUS) * sin(angle);
		for(int i=0; i<PLANE_COUNT+1+1+1; i++)
		{
			float angle = 2 * M_PI / PLANE_COUNT * i;
			float x = z0 * cos(angle);
			float z = z0 * sin(angle);
			
			now.x=x;now.y=x0;now.z=z;
			
			if(0 == j)
			{
				lastV[i].x=lastV[i].z=0;lastV[i].y=RADIUS;
			}
			else
			{
				if(0 == i)
				{
					thisV = now;
				}
				else
				{
					// present
					v[0] = now;
					// last
					v[1] = thisV;
					// from last pass
					v[2] = lastV[i-1];
					v[3] = lastV[i];
					// fix of floating point error
					if(j==PLANE_COUNT/2)
					{
						v[0].x=0;
						v[0].z=0;
						v[1].x=0;
						v[1].z=0;
					}
					if((i!=PLANE_COUNT+1+1)&&(i!=1))
						this->addQuad(v);

					lastV[i-1] = thisV;
					thisV = now;
				} // i != 0
			} // j != 0
		} // for i
	} // for j
}

inline bool operator== (const Vertex &a, const Vertex &b) {
	return (a.x==b.x && a.y==b.y && a.z==b.z);
}

inline bool operator!= (const Vertex &a, const Vertex &b) {
	return (a.x!=b.x || a.y!=b.y || a.z!=b.z);
}

void GlobeEntity::addQuad(Vertex vertex[4])
{
	// Triangle initialization
	Triangle t;
	setTriangleProperties( t );
	
	// Add triangle #1
	for (int i=0; i<3; i++)
		t.vertex[i] = vertex[i];
	if(t.vertex[0]!=t.vertex[1]) // bottom 0
		this->addTriangle(&t);

	// Add triangle #2
	t.vertex[1] = vertex[2];
	t.vertex[2] = vertex[3];
	if(t.vertex[1]!=t.vertex[2]) // top 0
		this->addTriangle(&t);
}
