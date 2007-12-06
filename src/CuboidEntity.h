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
public:

		/**
		 * @brief  Read entity properties and set its in object 
		 * @param  from XMLNode which has tagname cuboid
		 */
		void deserialize (XMLNode *from );

		
private:
		Color reflectivity_;
		Color emission_;
		Color radiosity_;
		
		/**
		 * @attention reflectivity_ and emission_ properties have to be
		 * initialized before call of this method!
		 */
		void polygonize();
		
private:
		void addQuad(Vertex vertex[4]);
};

#endif // CUBOIDENTITY_H
