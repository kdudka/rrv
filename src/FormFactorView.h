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

#ifndef FORMFACTORVIEW_H
#define FORMFACTORVIEW_H

#include <math.h>

#include "Triangle.h"

class Vector;
inline float operator* (const Vector &u, const Vector &v);

#ifndef NDEBUG
inline std::ostream& operator<< (std::ostream &out, const Vector &vect);
#endif

class Vector {
    public:
        // tohle by mohlo byt pozdeji private
        float dx,dy,dz;

    public:
        static Vector fromTriangle(Triangle &t) {
            return Vector(
                    t.vertex[0],
                    t.vertex[1],
                    t.vertex[2]
                    );
        }

        static float cos(const Vector &u, const Vector &v) {
            return (u*v)/(u.size()*v.size());
        }

        float& operator[](int i) {
            if(0==i)
                return dx;
            if(1==i)
                return dy;
            if(2==i)
                return dz;
            return(dx);
        }

        const float& operator[](int i) const {
            if(0==i)
                return dx;
            if(1==i)
                return dy;
            if(2==i)
                return dz;
            return(dx);
        }

        Vector() : dx(0), dy(0), dz(0) { };

        Vector(const float dx, const float dy, const float dz)
        {
            this->dx = dx;
            this->dy = dy;
            this->dz = dz;
        }

        Vector(const Vertex &a, const Vertex &b) {
            dx = a.x - b.x;
            dy = a.y - b.y;
            dz = a.z - b.z;
        }

        Vector(const Vector &u, const Vector &v) { // normalovy
            dx = u[1]*v[2] - v[1]*u[2];
            dy = u[2]*v[0] - v[2]*u[0];
            dz = u[0]*v[1] - v[0]*u[1];
        }

        Vector(const Vertex &a, const Vertex &b, const Vertex &c) {
            Vector u(b,a);
            Vector v(c,a);
            dx = u[1]*v[2] - v[1]*u[2];
            dy = u[2]*v[0] - v[2]*u[0];
            dz = u[0]*v[1] - v[0]*u[1];
        }

        float size() const {
            return sqrtf( dx*dx + dy*dy + dz*dz );
        }
        /*
           Vertex operator+ (const Vertex &v) {
           return Vertex(v.x+dx, v.y+dy, v.z+dz);
           }
           */
};

inline float operator* (const Vector &u, const Vector &v) {
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}
#ifndef NDEBUG
inline std::ostream& operator<< (std::ostream &out, const Vector &vect) {
    out << "vect(" <<
        vect[0] << ", " <<
        vect[1] << ", " <<
        vect[2] << ")";
    return out;
}
#endif

inline Vertex operator+ (const Vertex &v, const Vector &n) {
    return Vertex(v.x+n.dx, v.y+n.dy, v.z+n.dz);
}

/**
 * @brief Calculate viewing vectors from a patch.
 */
class FormFactorView
{
    public:
        FormFactorView (Triangle &t0);
        // center of triangle
        Vertex c;
        // forward / outwards
        Vector norm;
        // opposite
        Vector norm_m;
        // sides
        Vector vctA;
        Vector vctB;
        Vector vctC;
        Vector vctD;
};

#endif // FORMFACTORVIEW_H
