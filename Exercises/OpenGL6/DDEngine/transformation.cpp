#include "transformation.h"

// Constructor that take the initial position and the initial orientation
Transformation::Transformation(	Vector p_translation,
								Vector p_axis, float p_angle )
{
	// The transformation matrix initially is equal to the identity matrix
	transformationMatrix = Matrix();
	transformationMatrix = transformationMatrix.generateIdentityMatrix();
	
	// Initialise all the values
	rotation = Quaternion();
	bbRotation = Quaternion();
	translation = Vector();
	bbTranslation = Vector();

	// default inizialitazion values
	rotation = Quaternion(p_axis, p_angle);
	translation = p_translation;
	sX = sY = sZ = 1;
	shXY = shXZ = shYX = shYZ = shZX = shZY = 0;

	calculateMatrix();
	
}

// Returns the transpose of the actual transformation matrix
Matrix Transformation::getTransformation(void)
{
	return transformationMatrix.getTranspose();
}

// Return the inverse of the transposed actual transformation matrix
Matrix Transformation::getInverseTransformation(void)
{
	return transformationMatrix.getInverse().getTranspose();
}


// Apply a quaternion rotation
void Transformation::addQuaternionRotation(Quaternion q) 
{
	rotation = q * rotation;

	calculateMatrix();
}

// Apply a translation
void Transformation::addTranslation(Vector v)
{
	translation = translation + v;

	calculateMatrix();
}

// Apply a scale
void Transformation::addScaling(float p_sX, float p_sY, float p_sZ)
{
	sX *= p_sX;
	sY *= p_sY;
	sZ *= p_sZ;

	calculateMatrix();
}

// Apply a shear
void Transformation::addShearing(float p_shXY, float p_shXZ, float p_shYX, float p_shYZ, float p_shZX, float p_shZY)
{
	shXY += p_shXY;
	shXZ += p_shXZ;
	shYX += p_shYX;
	shYZ += p_shYZ;
	shZX += p_shZX;
	shZY += p_shZY;

	calculateMatrix();
}


// Return the vector of the actual translation
Vector Transformation::getTranslation()
{
	return translation;
}

// Return the quaternion of the actual rotation
Quaternion Transformation::getOrientation()
{
	return rotation;
}

// Put inside the array received the values of the actual scaling
void Transformation::getScaling(float * scaleValuesArray)
{
	scaleValuesArray[0] = sX;
	scaleValuesArray[1] = sY;
	scaleValuesArray[2] = sZ;
}

// Put inside the array received the values of the actual shearing
void Transformation::getShearing(float * shearValuesArray)
{
	shearValuesArray[0] = shXY;
	shearValuesArray[1] = shXZ;
	shearValuesArray[2] = shYX;
	shearValuesArray[3] = shYZ;
	shearValuesArray[4] = shZX;
	shearValuesArray[5] = shZY;
}

Vector Transformation::getBBTranslation(void)
{
	return bbTranslation;
}

Quaternion Transformation::getBBOrientation(void) 
{
	return bbRotation;
}

void Transformation::setTranslation(Vector t)
{
	translation = t;

	calculateMatrix();
}

void Transformation::setOrientation(Quaternion q)
{
	rotation = q;

	calculateMatrix();
}

void Transformation::setBBTranslation(Vector t)
{
	bbTranslation = t;
}

void Transformation::setBBOrientation(Quaternion q)
{
	bbRotation = q;
}

void Transformation::calculateMatrix()
{
	transformationMatrix = Matrix::generateQuaternionRotationMatrix(rotation);
	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ) * transformationMatrix;
	transformationMatrix = Matrix::generateTranslationMatrix(translation.getX(), translation.getY(), translation.getZ()) * transformationMatrix;
	transformationMatrix = Matrix::generateShearingMatrix(shXY, shXZ, shYX, shYZ, shZX, shZY) * transformationMatrix;
}