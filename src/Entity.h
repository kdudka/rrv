#ifndef ENTITY_H
#define ENTITY_H

/**
 * @file Entity.h
 * @brief Abstract class Entity - wrapper around TriangleSet performing its (de)serialization.
 * @author xdudka00, xfilak01
 * @date 2007-11-17
 */

#include <string>

#include "XMLWrapper.h"
#include "TriangleSet.h"
#include "TransformMatrix.h"

#ifndef BUILDING_DOX
class TransformMatrix;
class PatchSequenceEnumerator;
class TriangleSetExt;
#endif

/**
 * @brief Wrapper around TriangleSet performing its (de)serialization.
 */
class Entity
{
public:
		Entity();
		virtual ~Entity();

		/**
		 * @brief Deep copy
		 */
		Entity(const Entity&);
		
		/**
		 * @brief Deep copy
		 */
		Entity& operator=(const Entity&);
		
		/**
		 * @brief  Create a instantiate of special entity class.
		 * @param  from XMLNode containing entity declaration.
		 * @return entity created from XMLNode
		 */
		static Entity* create (XMLNode *from );

		/**
		 * @brief Set transofrmation matrix.
		 * @attention Calling this method after deserialize has no effect.
		 * @param matrix Transformation matrix to set. There is no need to keep it
		 * in memory after call of this method.
		 */
		void setTransformMatrix (TransformMatrix *matrix);

		/**
		 * @brief Entity deserialization from XMLNode.
		 * @attention this method is unimlemented in this class
		 * @param from XMLNode which contains entity declaration
		 */
		 void deserialize (XMLNode *from);

		/**
		 * @brief Perform patch division.
		 * @param size Size of the largest acceptable patch.
		 */
		void divide (float size);

		/**
		 * @brief Create enumerator for entity's patches.
		 * @return Return instance of PatchSequenceEnumerator allocated on the heap.
		 */
		PatchSequenceEnumerator* createPatchSequenceEnumerator ( );

		/**
		 * @brief Compute per-vertex colors using simple interpolation.
		 * @return Return instance of TriangleSetExt allocated on the heap.
		 */
		TriangleSetExt* computeVertexColors();
		
		/**
		 * @brief  Entity serialization to XMLNode.
		 * @return XMLNode wich is representation of entity.
		 */
		XMLNode serialize ( );

		/**
		 * @brief All entities in input file can have explicit name. And this method set it.
		 * @param from XMLNode wich contains entity declaration.
		 * @param implicit name when in xml doesn't exist
		 */
		void setName( XMLNode* from, const char* implicit="entity");

		/**
		 * @brief  Set entity name
		 * @param  name of entity
		 */
		void setName( std::string name ) { this->name_ = name; }

		/**
		 * @brief  Get name of entity
		 * @return Name of entity
		 */
		std::string& getName() { return this->name_; }

		/**
		 * @brief  Read color value from attribute of XMLNode and create its object representation
		 * @param  from XMLnode with attribute contains color value
		 * @param  attName name of attribute with color
		 * @param  to reference of object to store value
		 * @param  wlevel is warnings level
		 * @return treu if color is succesfuly read and stored
		 */
#ifndef NDEBUG
		static bool colorFromXMLNode( XMLNode* from, XMLCSTR attName, Color& to, int wlevel=2 );
#else
		static bool colorFromXMLNode( XMLNode* from, XMLCSTR attName, Color& to );
#endif

protected:
		/**
		 * @brief Add triangle to entity using tranformation matrix.
		 * @param triangle Triangle to add.
		 */
		void addTriangle( Triangle *triangle );

		/**
		 * @brief Set protperties to from entity properties.
		 * @param triangle Updated triangle.
		 */
		void setTriangleProperties( Triangle& triangle );

		/**
		 * @brief  Read three colors from XMLnode from specified attributs and ctore its valeus
		 * @param  from XMLNode containing entity declaration.
		 * @param  em Emmision color value
		 * @param  refl Reflectiovity color value
		 * @param  rad Radiosity color value
		 */
		static void setColors( XMLNode* from, Color& em, Color& refl, Color& rad ); 

		/**
		 * @brief Entity deserialization from XMLNode.
		 * @attention this method is unimlemented in this class
		 * @param from XMLNode which contains entity declaration
		 */
		virtual void impl_deserialize (XMLNode *from);

		Color reflectivity_;
                Color emission_;
		Color radiosity_;

		double spec_;
		double refl_;
		double refr_;

private:
		TransformMatrix transformMatrix_;
		TriangleSet triangleSet_;
		TriangleSet *patchSet_;
		std::string name_;
};

#endif // ENTITY_H
