/*
 * Copyright (C) 2007 Kamil Dudka <rrv@dudka.cz>, Jakub Filak
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

#include "CuboidEntity.h"
#include <iostream>

using namespace XML;
/**
 * @param  from
 */
void CuboidEntity::impl_deserialize (XMLNode *from ) {
    setName(from,"cuboid");
    this->polygonize();
}

/**
*/
void CuboidEntity::polygonize ( ) {
    const size_t PLANE_COUNT = 6;
    const size_t QUAD_VERTEX_COUNT = 4;
    Vertex tpl[PLANE_COUNT][QUAD_VERTEX_COUNT] = {{
        // left plane
        Vertex(-0.5, -0.5, -0.5),
            Vertex(-0.5, -0.5, +0.5),
            Vertex(-0.5, +0.5, +0.5),
            Vertex(-0.5, +0.5, -0.5),
    }, {
        // right plane
        Vertex(+0.5, -0.5, -0.5),
            Vertex(+0.5, +0.5, -0.5),
            Vertex(+0.5, +0.5, +0.5),
            Vertex(+0.5, -0.5, +0.5),
    }, {
        // down plane
        Vertex(-0.5, -0.5, -0.5),
            Vertex(+0.5, -0.5, -0.5),
            Vertex(+0.5, -0.5, +0.5),
            Vertex(-0.5, -0.5, +0.5),
    }, {
        // up plane
        Vertex(-0.5, +0.5, -0.5),
            Vertex(-0.5, +0.5, +0.5),
            Vertex(+0.5, +0.5, +0.5),
            Vertex(+0.5, +0.5, -0.5),
    }, {
        // rear plane
        Vertex(-0.5, -0.5, -0.5),
            Vertex(-0.5, +0.5, -0.5),
            Vertex(+0.5, +0.5, -0.5),
            Vertex(+0.5, -0.5, -0.5),
    }, {
        // front plane
        Vertex(-0.5, -0.5, +0.5),
            Vertex(+0.5, -0.5, +0.5),
            Vertex(+0.5, +0.5, +0.5),
            Vertex(-0.5, +0.5, +0.5),
    }
    };
    for(size_t i=0; i<PLANE_COUNT; i++)
        this->addQuad(tpl[i]);
}

void CuboidEntity::addQuad(Vertex vertex[4]) {
    // Triangle initialization
    Triangle t;
    setTriangleProperties( t );

    // Add triangle #1
    for (int i=0; i<3; i++)
        t.vertex[i] = vertex[i];
    this->addTriangle(&t);

    // Add triangle #2
    t.vertex[1] = vertex[2];
    t.vertex[2] = vertex[3];
    this->addTriangle(&t);
}
