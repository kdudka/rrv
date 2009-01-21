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
