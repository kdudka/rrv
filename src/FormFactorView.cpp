/*
 * Copyright (C) 2007 David Barina <DaBler@gmail.com>
 * Copyright (C) 2015 Claude Heiland-Allen <claude@mathr.co.uk>
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

#include "FormFactorView.h"

FormFactorView::FormFactorView (Triangle &t0)
{
    // center of this triangle
    c = Triangle::centerOf(t0);

    // normal vector and inverse normal vector of this triangle
    norm = Vector::fromTriangle(t0);
    norm_m = Vector(-norm.dx,-norm.dy,-norm.dz);

    Vector side(norm, Vector(1,2,3));
    if(0 == side.size())
        side = Vector(norm, Vector(1,1,1)); // a neudelat Vector::operator= ??
    Vector side_m(-side.dx,-side.dy,-side.dz);

    // side vectors
    vctD = Vector(/*t0.vertex[0], t0.vertex[1]*/side);
    vctC = Vector(/*t0.vertex[1], t0.vertex[0]*/side_m);
    vctA = Vector( vctD, norm );
    vctB = Vector( vctC, norm );
}
