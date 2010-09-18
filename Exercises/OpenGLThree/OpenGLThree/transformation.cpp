#include "transformation.h"

// Constructor that take the initial position and the initial orientation
Transformation::Transformation(	float p_tX,		float p_tY,		float p_tZ,
								Vector p_axis, float p_angle )
{
	// The transformation matrix initially is equal to the identity matrix
	transformationMatrix = Matrix::generateIdentityMatrix();
	
	// Initialise all the values
	angleX = angleY = angleZ = 0;
	vector = Vector(0.0f, 0.0f, 0.0f);
	angle = 0;
	tX = tY = tZ = 0;
	sX = sY = sZ = 1;
	shXY = shXZ = shYX = shYZ = shZX = shZY = 0;
	
	// Apply the firs translation and rotation
	addTranslation(p_tX, p_tY, p_tZ);
	addQuaternionRotation(p_axis, p_angle);
}

// Returns the transpose of the actual transformation matrix
Matrix Transformation::getTransformation(void)
{
	return transformationMatrix.getTranspose();
}

// Apply a rotation about one of the axes
void Transformation::addRotation(float p_angleX, float p_angleY, float p_angleZ)
{
	
	angleX += p_angleX;
	angleY += p_angleY;
	angleZ += p_angleZ;
	
	transformationMatrix = Matrix::generateXRotationMatrix(angleX) * transformationMatrix;
	transformationMatrix = Matrix::generateYRotationMatrix(angleY) * transformationMatrix;
	transformationMatrix = Matrix::generateZRotationMatrix(angleZ) * transformationMatrix;

}

// Apply a quaternion rotation
void Transformation::addQuaternionRotation(Vector p_ArbitraryAxis, float p_Angle) 
{
	vector = vector + p_ArbitraryAxis;
	float temp = vector.getQuadraticMagnitude();
	if (temp != 1 && temp != 0) {
		vector = vector.normalize();
	}
	angle += p_Angle;

	transformationMatrix = Matrix::generateAxesRotationMatrix(vector, angle);
}

// Apply a translation
void Transformation::addTranslation(float p_tX, float p_tY, float p_tZ)
{
	
	tX += p_tX;
	tY += p_tY;
	tZ += p_tZ;

	transformationMatrix = Matrix::generateTranslationMatrix(tX, tY, tZ);
}

// Apply a scale
void Transformation::addScaling(float p_sX, float p_sY, float p_sZ)
{
	sX *= p_sX;
	sY *= p_sY;
	sZ *= p_sZ;

	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ) * transformationMatrix;
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

	transformationMatrix = Matrix::generateShearingMatrix(shXY, shXZ, shYX, shYZ, shZX, shZY) * transformationMatrix;
}