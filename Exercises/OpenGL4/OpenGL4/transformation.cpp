#include "transformation.h"

// Constructor that take the initial position and the initial orientation
Transformation::Transformation(	Vector p_translation,
								Vector p_axis, float p_angle )
{
	// The transformation matrix initially is equal to the identity matrix
	transformationMatrix = Matrix::generateIdentityMatrix();
	
	// Initialise all the values
	rotation = Quaternion(p_axis, p_angle);

	translation = p_translation;

	sX = sY = sZ = 1;
	shXY = shXZ = shYX = shYZ = shZX = shZY = 0;
	
}

// Returns the transpose of the actual transformation matrix
Matrix Transformation::getTransformation(void)
{

	float tranM[16];
	transformationMatrix = Matrix::generateQuaternionRotationMatrix(rotation);
	transformationMatrix = Matrix::generateTranslationMatrix(translation.get(0), translation.get(1), translation.get(2)) * transformationMatrix;
	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ) * transformationMatrix;
	transformationMatrix = Matrix::generateShearingMatrix(shXY, shXZ, shYX, shYZ, shZX, shZY) * transformationMatrix;
	
	
	transformationMatrix.getMatrix(&tranM[0]);
	return transformationMatrix.getTranspose();
}

Matrix Transformation::getInverseTransformation(void)
{
	float tranM[16];
	
	transformationMatrix = Matrix::generateShearingMatrix(shXY, shXZ, shYX, shYZ, shZX, shZY).getInverse();
	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ).getInverse() * transformationMatrix;
	transformationMatrix = Matrix::generateTranslationMatrix(translation.get(0), translation.get(1), translation.get(1)).getInverse() * transformationMatrix;
	transformationMatrix = Matrix::generateQuaternionRotationMatrix(rotation).getInverse() * transformationMatrix;

	transformationMatrix.getMatrix(&tranM[0]);
	return transformationMatrix.getTranspose();
}


// Apply a quaternion rotation
void Transformation::addQuaternionRotation(Quaternion q) 
{
	rotation = q * rotation;
}

// Apply a translation
void Transformation::addTranslation(Vector v)
{
	
	translation = translation + v;
}

// Apply a scale
void Transformation::addScaling(float p_sX, float p_sY, float p_sZ)
{
	sX *= p_sX;
	sY *= p_sY;
	sZ *= p_sZ;
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
}


Vector Transformation::getTranslation()
{
	return translation;
}

Quaternion Transformation::getOrientationQuaternion()
{
	return rotation;
}


/*
		float getOrientationVector();
		float getOrientationAngle();
		float getScaling();
		float getShearing();

		*/