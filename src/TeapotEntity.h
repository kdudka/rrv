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
public:

		/**
		 * @brief  Read entity properties and set its in object
		 * @param  from XMLNode which has tagname teapot
		 */
		void deserialize (XMLNode *from );

private:
		Color reflectivity_;
                Color emission_;
		Color radiosity_;

		static const int teapot_v_count = 1976;
		static const int teapot_t_count = 3752;
		static const Vertex teapot_v[1976];
		static const int teapot_t[3752][3];

		void polygonize ( );
};

#endif // TEAPOTENTITY_H
