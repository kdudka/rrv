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

#ifndef CUBOIDENTITY_H
#define CUBOIDENTITY_H

/**
 * @file CuboidEntity.h
 * @brief Class CuboidEntity
 * @todo Document.
 * @author xdudka00, xfilak01
 * @date 2007-11-17
 */

#include "Entity.h"

/**
 * This entity has cuboid as base shape. Size of the cuboid is 1.0x1.0x1.0
 * and center is in Vertex(0.0, 0.0, 0.0). See documentation of Entity class
 * for more information about transformations, patch division, etc.
 * @brief Cuboid entity.
 */
class CuboidEntity : public Entity {
protected:

		/**
		 * @brief  Read entity properties and set its in object 
		 * @param  from XMLNode which has tagname cuboid
		 */
		void impl_deserialize (XMLNode *from );

		
private:
		/**
		 * @attention reflectivity_ and emission_ properties have to be
		 * initialized before call of this method!
		 */
		void polygonize();
		
		void addQuad(Vertex vertex[4]);
};

#endif // CUBOIDENTITY_H
