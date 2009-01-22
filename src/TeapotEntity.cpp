#include "TeapotEntity.h"
//#include "TeapotEntityDef.h"

using namespace XML;

void TeapotEntity::impl_deserialize (XMLNode *from ) {
	setName( from, "teapot" );
	this->polygonize();
}

void TeapotEntity::polygonize ( )
{
	for(int i=0; i<teapot_t_count; i++)
	{
		Triangle t;
		setTriangleProperties( t );
		
		for (int j=0; j<3; j++)
		{
			t.vertex[j] = teapot_v[teapot_t[i][2-j]];
			float f = t.vertex[j].y; t.vertex[j].y = t.vertex[j].z; t.vertex[j].z = f;
		}

		this->addTriangle(&t);
	}
}
