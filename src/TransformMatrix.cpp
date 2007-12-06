#include "TransformMatrix.h"

#include <math.h>
#ifndef M_PI
	#define M_PI		3.14159265358979323846
#endif

SpatialVector::operator float() const {
	return sqrt(dx*dx + dy*dy + dz*dz);
}

/**
 * Create matrix of identity
 */
void TransformMatrix::setIdentity(TTransformMatrix dest) {
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			dest[i][j] = (i==j)?
					1.0f:
					0.0f;
}

TransformMatrix::TransformMatrix() {
	setIdentity(matrix_);
}

/**
 * FIXME: ad hoc algoritmus
 * @param  angle
 * @param  vect
 */
void TransformMatrix::rotate (float angle, SpatialVector vect) {
	// Convert angle to radians
	angle *= M_PI/180.0;
	
	// Normalize spatial vector and
	// compute angle for all axes
	float vectSize = static_cast<float>(vect);
	vect*= angle/vectSize;
/*	std::cout << "Rotation: " << vect << std::endl;
	std::abort();*/
	
	// Working variables
	TTransformMatrix mm;
	float sinAlpha;
	float cosAlpha;
	
	// X-axis rotation
	setIdentity(mm);
	sinAlpha = sinf(vect.dx);
	cosAlpha = cosf(vect.dx);
	mm[1][1] = cosAlpha;
	mm[1][2] = sinAlpha;
	mm[2][1] = -sinAlpha;
	mm[2][2] = cosAlpha;
	this->mulMatrixFromLeft(mm);
	
	// Y-axis rotation
	setIdentity(mm);
	sinAlpha = sinf(vect.dy);
	cosAlpha = cosf(vect.dy);
	mm[0][0] = cosAlpha;
	mm[0][2] = sinAlpha;
	mm[2][0] = -sinAlpha;
	mm[2][2] = cosAlpha;
	this->mulMatrixFromLeft(mm);
	
	// Z-axis rotation
	setIdentity(mm);
	sinAlpha = sinf(vect.dz);
	cosAlpha = cosf(vect.dz);
	mm[0][0] = cosAlpha;
	mm[0][1] = sinAlpha;
	mm[1][0] = -sinAlpha;
	mm[1][1] = cosAlpha;
	this->mulMatrixFromLeft(mm);
}


/**
 * @param  vect
 */
void TransformMatrix::translate (SpatialVector vect) {
	TTransformMatrix mm;
	setIdentity(mm);
	
	mm[3][0] = vect.dx;
	mm[3][1] = vect.dy;
	mm[3][2] = vect.dz;
	
	// FIXME: mulMatrixFromRight??
	this->mulMatrixFromLeft(mm);
}


/**
 * @param  vect
 */
void TransformMatrix::scale (SpatialVector vect ) {
	TTransformMatrix mm;
	setIdentity(mm);
	
	mm[0][0] = vect.dx;
	mm[1][1] = vect.dy;
	mm[2][2] = vect.dz;
	
	// FIXME: mulMatrixFromRight??
	this->mulMatrixFromLeft(mm);
}


/**
 * @param  vect
 */
void TransformMatrix::shear (SpatialVector) {
	// TODO: Implement
}


/**
 * @return Vertex
 * @param  vertex
 */
Vertex TransformMatrix::transform (Vertex in) {
	float x=
			matrix_[0][0]*in.x +
			matrix_[1][0]*in.y +
			matrix_[2][0]*in.z +
			matrix_[3][0];
	                 
	float y=         
			matrix_[0][1]*in.x +
			matrix_[1][1]*in.y +
			matrix_[2][1]*in.z +
			matrix_[3][1];
	                 
	float z=         
			matrix_[0][2]*in.x +
			matrix_[1][2]*in.y +
			matrix_[2][2]*in.z +
			matrix_[3][2];
	                 
	float w=         
			matrix_[0][3]*in.x +
			matrix_[1][3]*in.y +
			matrix_[2][3]*in.z +
			matrix_[3][3];
	
	float factor= 1/w;
	return Vertex(
		x/factor,
		y/factor,
		z/factor
		);
}

void TransformMatrix::mulMatrixFromLeft(TTransformMatrix mm) {
	TTransformMatrix tmp;
	copyMatrix(tmp, matrix_);
	mulMatrix(matrix_, mm, tmp);
}

void TransformMatrix::mulMatrixFromRight(TTransformMatrix mm) {
	TTransformMatrix tmp;
	copyMatrix(tmp, matrix_);
	mulMatrix(matrix_, tmp, mm);
}

void TransformMatrix::copyMatrix(
		TTransformMatrix dest,
		TTransformMatrix src)
{
	for(int i=0; i< MATRIX_DIM; i++)
		for(int j=0; j< MATRIX_DIM; j++)
			dest[i][j] = src[i][j];
}

void TransformMatrix::mulMatrix(
		TTransformMatrix dest,
		TTransformMatrix a,
		TTransformMatrix b)
{
	for(int i=0; i< MATRIX_DIM; i++)
		for(int j=0; j< MATRIX_DIM; j++) {
			float &destItem = dest[i][j];
			destItem = 0.0;
			for (int n=0; n< MATRIX_DIM; n++)
				destItem+= a[i][n] * b[n][j];
		}
}
