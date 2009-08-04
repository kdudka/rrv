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

#ifndef TEAPOTENTITY_H
#define TEAPOTENTITY_H
#include "Entity.h"

/**
 * @file TeapotEntity.h
 * @brief Class TeapotEntity
 * @author xbarin02, xfilak01
 * @date 2007-11-17
 */

/**
 * This entity has Utah Teapot as base shape. Bottom of this teapot
 * is in Vertex(0.0, 0.0, 0.0). See documentation of Entity class
 * for more information about transformations, patch division, etc.
 * @brief Teapot entity.
 */
class TeapotEntity : public Entity {
    protected:

        /**
         * @brief  Read entity properties and set its in object
         * @param  from XMLNode which has tagname teapot
         */
        virtual void impl_deserialize (XMLNode *from );

    private:
        static const int teapot_v_count = 1976;
        static const int teapot_t_count = 3752;
        static const Vertex teapot_v[1976];
        static const int teapot_t[3752][3];

        void polygonize ( );
};

#endif // TEAPOTENTITY_H
