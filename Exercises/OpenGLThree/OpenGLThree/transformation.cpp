#include "transformation.h"

Transformation::Transformation(	float p_tX,		float p_tY,		float p_tZ,
								float p_angleX, float p_angleY, float p_angleZ)
{
	transformationMatrix = Matrix::generateIdentityMatrix();
	
	angleX = angleY = angleZ = 0;
	tX = tY = tZ = 0;
	sX = sY = sZ = 1;
	shXY = shXZ = shYX = shYZ = shZX = shZY = 0;

	addTranslation(p_tX, p_tY, p_tZ);
	addRotation(p_angleX, p_angleY, p_angleZ);
}


Matrix Transformation::getTransformation(void)
{
	/*Matrix transformationMatrix = Matrix::generateTranslationMatrix(tX, tY, tZ).getTranspose();
	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ).getTranspose() * transformationMatrix;
    transformationMatrix = Matrix::generateXRotationMatrix(angleX).getTranspose() * transformationMatrix;
    transformationMatrix = Matrix::generateYRotationMatrix(angleY).getTranspose() * transformationMatrix;
    transformationMatrix = Matrix::generateZRotationMatrix(angleZ).getTranspose() * transformationMatrix;
    transformationMatrix = Matrix::generateShearingMatrix(shXY, shXZ, shYX, shYZ, shZX, shZY).getTranspose() * transformationMatrix;*/

	return transformationMatrix.getTranspose();
}

void Transformation::addRotation(float p_angleX, float p_angleY, float p_angleZ)
{
	
	angleX += p_angleX;
	angleY += p_angleY;
	angleZ += p_angleZ;
	
	transformationMatrix = Matrix::generateXRotationMatrix(angleX) * transformationMatrix;
	transformationMatrix = Matrix::generateYRotationMatrix(angleY) * transformationMatrix;
	transformationMatrix = Matrix::generateZRotationMatrix(angleZ) * transformationMatrix;

}

void Transformation::addQuaternionRotation(Vector arbitraryAxis, float p_Angle) 
{

	transformationMatrix = Matrix::generateAxesRotationMatrix(arbitraryAxis, p_Angle);
}

void Transformation::addTranslation(float p_tX, float p_tY, float p_tZ)
{
	
	tX += p_tX;
	tY += p_tY;
	tZ += p_tZ;

	transformationMatrix = Matrix::generateTranslationMatrix(tX, tY, tZ);
}
void Transformation::addScaling(float p_sX, float p_sY, float p_sZ)
{
	sX *= p_sX;
	sY *= p_sY;
	sZ *= p_sZ;

	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ) * transformationMatrix;
}

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