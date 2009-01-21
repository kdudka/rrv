#ifndef BARRELENTITY_H
#define BARRELENTITY_H

/**
 * @file BarrelEntity.h
 * @brief Class BarrelEntity
 * @author xbarin02, xfilak01
 * @date 2007-11-17
 */

#include "Entity.h"

/**
 * This entity has cylinder as base shape. Radius of the cylinder is 0.5,
 * height 1.0 and center is in Vertex(0.0, 0.0, 0.0). See documentation
 * of Entity class for more information about transformations, patch division, etc.
 * @brief Barrel entity.
 */
class BarrelEntity : public  Entity {
protected:

		/**
		 * @brief  Read entity properties and set its in object
		 * @param  from XMLNode which has tag name barrel
		 */
		virtual void impl_deserialize ( XMLNode *from );


private:
		void polygonize ( );

		void addQuad(Vertex vertex[4]);
		void addBaseTriangles(Vertex vertex[4]);
};

#endif // BARRELENTITY_H
