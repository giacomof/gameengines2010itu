#include "transformation.h"

// Constructor that take the initial position and the initial orientation
Transformation::Transformation(	float p_tX,		float p_tY,		float p_tZ,
								Vector p_axis, float p_angle )
{
	// The transformation matrix initially is equal to the identity matrix
	transformationMatrix = Matrix::generateIdentityMatrix();
	
	// Initialise all the values
	rotation = Quaternion(p_axis, p_angle);

	tX = p_tX; 
	tY = p_tY;
	tZ = p_tZ;

	sX = sY = sZ = 1;
	shXY = shXZ = shYX = shYZ = shZX = shZY = 0;
	
}

// Returns the transpose of the actual transformation matrix
Matrix Transformation::getTransformation(void)
{

	float tranM[16];
	transformationMatrix = Matrix::generateQuaternionRotationMatrix(rotation);
	transformationMatrix = Matrix::generateTranslationMatrix(tX, tY, tZ) * transformationMatrix;
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
	transformationMatrix = Matrix::generateTranslationMatrix(tX, tY, tZ).getInverse() * transformationMatrix;
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
void Transformation::addTranslation(float p_tX, float p_tY, float p_tZ)
{
	
	tX += p_tX;
	tY += p_tY;
	tZ += p_tZ;
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

/*
float Transformation::getTranslation()
{
	float translation[3];
	translation[0] = tX;
	translation[1] = tY;
	translation[2] = tZ;

	return translation;
}
		float getOrientationVector();
		float getOrientationAngle();
		float getScaling();
		float getShearing();

		*/