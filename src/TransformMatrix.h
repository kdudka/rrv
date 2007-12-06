#ifndef TRANSFORMMATRIX_H
#define TRANSFORMMATRIX_H

/**
 * @file TransformMatrix.h
 * @brief Class TransformMatrix - 3D transformations
 * @author xdudka00
 * @date 2007-11-17
 */

#include "Triangle.h"

#ifndef NDEBUG
#	ifndef BUILDING_DOX
#		include <iostream>
#	endif
#endif

/**
 * @brief 3-axes vector - simple structure
 */
struct SpatialVector {
		float dx;	///< x-axe
		float dy;	///< y-axe
		float dz;	///< z-axe
		
		/**
		 * @brief @return Return vector's size.
		 */
		operator float() const;
};

/**
 * @brief Multiplication by constant.
 * @param vect Spatial vector.
 * @param f Constant to multiplicate by.
 * @return Return reference to spatial vector (usual operator *= behavior).
 */
inline SpatialVector& operator*= (SpatialVector &vect, float f) {
	vect.dx*= f;
	vect.dy*= f;
	vect.dz*= f;
	return vect;
}

#ifndef NDEBUG
/**
 * @brief Dump SpatialVector object to stream in human readable format.
 * @note This function will not be built if NDEBUG macro is defined.
 * @param out Output stream to write to.
 * @param vect SpatialVector to Vertex object to dump.
 * @return Return output stream given as parameter.
 */
inline std::ostream& operator<< (std::ostream &out, const SpatialVector &vect) {
	out << "vect(" <<
			vect.dx << ", " <<
			vect.dy << ", " <<
			vect.dz << ")";
	return out;
}
#endif

/**
 * @brief 3D transformations
 */
class TransformMatrix {
public:
		/**
		 * @brief Dimension of internal matrix.
		 */
		enum { MATRIX_DIM = 4 };
		
		/**
		 * @brief Type of internal matrix.
		 */
		typedef float TTransformMatrix[MATRIX_DIM][MATRIX_DIM];
		
		/**
		 * @brief @b Reset transformation to identity.
		 */
		static void setIdentity(
				TTransformMatrix dest);
		
		/**
		 * @brief Copy internal matrix.
		 * @param dest Destination matrix.
		 * @param src Source matrix.
		 */
		static void copyMatrix(
				TTransformMatrix dest,
				TTransformMatrix src);
		
		/**
		 * @brief Matrix multiplication.
		 * @param dest Destination matrix (will be overwriten).
		 * @param a Matrix a.
		 * @param b Matrix b.
		 */
		static void mulMatrix(
				TTransformMatrix dest,
				TTransformMatrix a,
				TTransformMatrix b);
		
		/**
		 * @brief Create matrix of identity
		 */
		TransformMatrix();

		/**
		 * @brief Transformation: @b rotation
		 * @param angle Angle of rotation.
		 * @param vect Spatial vector to rotate around.
		 * @attention Behavior of vect parameter is messy
		 * and was not tested on non-trivial inputs.
		 */
		void rotate (float angle, SpatialVector vect );

		/**
		 * @brief Transformation: @b translation
		 * @param  vect Vector of translation.
		 */
		void translate (SpatialVector vect );

		/**
		 * @brief Transformation: @b scale
		 * @param  vect Scale vector.
		 */
		void scale (SpatialVector vect );

		/**
		 * @brief This method is @b not @b implemented.
		 * @todo Implement
		 */
		void shear (SpatialVector);

		/**
		 * @brief Perform transformation on given vertex.
		 * @param vertex Original vertex.
		 * @return Return transformed vertex.
		 */
		Vertex transform (Vertex vertex);
		
	private:
		TTransformMatrix matrix_;
	
	protected:
#ifndef BUILDING_DOX
		void mulMatrixFromLeft(TTransformMatrix mm);
		void mulMatrixFromRight(TTransformMatrix mm);
#endif
};

#endif // TRANSFORMMATRIX_H
