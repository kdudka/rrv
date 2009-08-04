/*
 * Copyright (C) 2007 David Barina <DaBler@gmail.com>, Jakub Filak
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
