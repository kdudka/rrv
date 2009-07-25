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

#ifndef GLOBEENTITY_H
#define GLOBEENTITY_H

/**
 * @file GlobeEntity.h
 * @brief Class GlobeEntity
 * @author xbarin02, xfilak01
 * @date 2007-11-17
 */

#include "Entity.h"

/**
 * This entity has sphere as base shape. Radius of this sphere is 0.5
 * and center is in Vertex(0.0, 0.0, 0.0). See documentation
 * of Entity class for more information about transformations, patch division, etc.
 * @brief Globe entity.
 */
class GlobeEntity : public Entity {
protected:

		/**
		 * @brief  Read entity properties and set its in object
		 * @param  from XMLNode which has tagname globe
		 */
		virtual void impl_deserialize (XMLNode *from );

private:
		void polygonize ( );
		void addQuad(Vertex vertex[4]);
};

#endif // GLOBEENTITY_H
