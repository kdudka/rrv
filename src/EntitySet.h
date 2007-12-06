#ifndef ENTITYSET_H
#define ENTITYSET_H

/**
 * @file EntitySet.h
 * @brief Class EntitySet - container of Entity objects.
 * @author xdudka00, xfilak01
 * @date 2007-11-17
 */

#include "Entity.h"

#include <vector>

#ifndef BUILDING_DOX
#	ifndef NDEBUG
#		include <iostream>
# 	include "PatchSequenceEnumerator.h"
#	endif

	class PatchRandomAccessEnumerator;
	class TriangleSetExt;
#endif

/**
 * @brief Container of Entity objects.
 */
class EntitySet {
	public:
		/**
		 * @brief Add @b copy @b of entity to container.
		 * @note There is no need to keep Entity object in memory outside this class.
		 * @param entity Pointer to entity to copy to container.
		 */
		void add(Entity *entity);

		/**
		 * @copydoc Entity::divide(float)
		 */
		void divide(float size);

		/**
		 * @brief Create enumerator for all patches maintained indirectly by EntitySet.
		 * @return Return instance of PatchSequenceEnumerator allocated on the heap.
		 */
		PatchSequenceEnumerator* createPatchSequenceEnumerator();

		/**
		 * @brief Return current count of entities stored in container.
		 * @return Return current count of entities stored in container.
		 */
		size_t count();

		/**
		 * @brief Access entity using its index.
		 * @param index Index of entity - must be in range <0, count()-1>.
		 * @return Return reference to desired entity.
		 */
		Entity& operator[] (unsigned index);
		
		/**
		 * @brief @copydoc Entity::computeVertexColors()
		 */
		TriangleSetExt* computeVertexColors();

private:
		typedef std::vector<Entity> TContainer;
		TContainer container_;
};

#ifndef NDEBUG
/**
 * @brief Dump all maintained patches to output stream in human readable format.
 * @note This function will not be built if NDEBUG macro is defined.
 * @param out Output stream to write to.
 * @param entitySet Reference to EnitytSet object to dump.
 * @return Return output stream given as parameter.
 */
inline std::ostream& operator<< (std::ostream &out, EntitySet &entitySet) {
	PatchSequenceEnumerator *pe= entitySet.createPatchSequenceEnumerator();
	Triangle *t;
	while (0!= (t= pe->nextPatch()))
		out << *t;
	
	delete pe;
	return out;
}
#endif

#endif // ENTITYSET_H
