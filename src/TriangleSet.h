#ifndef TRIANGLESET_H
#define TRIANGLESET_H

/**
 * @file TriangleSet.h
 * @brief Class TriangleSet - container for Triangle objects
 * @author xdudka00
 * @date 2007-11-17
 */

#include "Triangle.h"

#include <vector>
#include <math.h>

#ifndef BUILDING_DOX
struct Triangle;
#endif

/**
 * There are also static methods to deal with Triangle object
 * and its components to leave Triangle structure as POD.
 * @brief Container for Triangle objects.
 */
class TriangleSet {
public:
		/**
		 * @brief Compute distance of two vertexes.
		 * @param a Vertex a.
		 * @param b Vertex b.
		 * @return Return distance of vertexes @e a and @e b.
		 */
		static float distance(Vertex a, Vertex b) {
			const float dx = a.x - b.x;
			const float dy = a.y - b.y;
			const float dz = a.z - b.z;
			
			return sqrtf(dx*dx + dy*dy + dz*dz);
		}

		/**
		 * @brief Compute center of two vertexes.
		 * @note Used for patch division.
		 * @param a Vertex a.
		 * @param b Vertex b.
		 * @return Return center of vertexes @e a and @e b as Vertex.
		 */
		static Vertex centre(Vertex a, Vertex b) {
			return Vertex(
					(a.x + b.x)/2,
					(a.y + b.y)/2,
					(a.z + b.z)/2); 
		}

		/**
		 * @brief Add @b copy @b of triangle to container.
		 * @note There is no need to keep Triangle object in memory outside this class.
		 * @param triangle Pointer to triangle to @b copy to container.
		 */
		void add(Triangle *triangle);
		
		/**
		 * @brief Add @b copy @b of triangles to container.
		 * @note There is no need to keep Triangle objects in memory outside this class.
		 * @param tset Pointer to TriangleSet containing source triangles.
		 */
		void add(TriangleSet* tset);

		/**
		 * @brief Return current count of triangle stored in container.
		 * @return Return current count of triangle stored in container.
		 */
		size_t count();

		/**
		 * @brief Access triangle using its index.
		 * @param index Index of triangle - must be in range <0, count()-1>.
		 * @return Return reference to desired triangle.
		 */
		Triangle& operator[] (unsigned index);

		/**
		 * Template parameter @c QUALITY Switch algorithm to express triangle size (0 - girth, 1 - area).
		 * There are two way to express triangle size - girth and area.
		 * The 1st is faster, the 2nd is realistic.
		 * @brief Compute triangle size.
		 * @param  triangle Triangle to compute.
		 * @return Return triangle size using selected algorithm.
		 */
		template <int QUALITY> static float size (Triangle* triangle);

		/**
		 * @brief Divide triangle to 4 triangles, each with 1/4 area.
		 * @note Vertex oriantation is kept.
		 * @param triangle Pointer to triangle to divide.
		 * @return Return TriangleSet object (allocated on the heap) containing 4 triangles.
		 */
		static TriangleSet* divide(Triangle *triangle);

	private:
		typedef std::vector<Triangle> TContainer;
		TContainer container_;

};

/**
	* @brief Compute triangle's girt.
	* @param  triangle Triangle to compute.
	* @return Return triangle's girt.
	*/
template <> inline float TriangleSet::size<0> (Triangle *t) {
	Vertex *v = t->vertex;
	float size = 0.0;
	for(int i=0; i<3; i++)
		size+= distance(v[i], v[(i+1)%3]);
	
	return size;
}

/**
	* @brief Compute triangle's area using Heron's calculation.
	* @param  triangle Triangle to compute.
	* @return Return triangle's area.
	*/
template <> inline float TriangleSet::size<1> (Triangle *t) {
	Vertex *v = t->vertex;
	float length[3];
	float sum = 0.0;
	for(int i=0; i<3; i++)
		sum+=	length[i]= distance(v[i], v[(i+1)%3]);
	
	float size = (sum/=2);
	for(int i=0; i<3; i++)
		size*= sum-length[i];
	
	return sqrtf(size);
}


#endif // TRIANGLESET_H
