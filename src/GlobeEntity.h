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
public:

		/**
		 * @brief  Read entity properties and set its in object
		 * @param  from XMLNode which has tagname globe
		 */
		void deserialize (XMLNode *from );

private:
		Color reflectivity_;
                Color emission_;
		Color radiosity_;

		void polygonize ( );
		void addQuad(Vertex vertex[4]);
};

#endif // GLOBEENTITY_H
