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

#ifndef TRIANGLEENTITY_H
#define TRIANGLEENTITY_H

/**
 * @file TriangleEntity.h
 * @brief @b TODO: Document
 * @todo Document.
 * @author xfilak01
 * @date 2007-11-17
 */

#include "Entity.h"
#include "Triangle.h"

/**
 * @brief @b TODO: Document
 * @todo Document.
 */
class TriangleEntity : public Entity {
protected:

		/**
		 * @brief  Read entity properties and set its in object.
		 * @param  from XMLNode which has tagname triangleset
		 */
		virtual void impl_deserialize ( XMLNode *from );


private:
		/**
		 * @param  from XMLNode with tagname vertex 
		 * @return New vertex with properties from XMLNode
		 */
		Vertex vertexFromXMLNode( XMLNode* from);

		/**
		 * @param  from XMLNode with tagname triangle or triangleset
  		 * @param  defEm default emmision color value		 
		 * @param  defRefl default reflectivity color value		 
		 * @param  defRad default radiosity value 
		 * @return triangle with properties from XMLNode 
		 */
		Triangle triangleFromXMLNode( XMLNode* from );
};

#endif // TRIANGLEENTITY_H
