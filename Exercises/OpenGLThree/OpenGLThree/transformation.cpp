#include "transformation.h"



Transformation::Transformation(	float p_tX,		float p_tY,		float p_tZ,
								float p_angleX, float p_angleY, float p_angleZ) 
{
	angleX = angleY = angleZ = 0;
	tX = tY = tZ = 0;
	sX = sY = sZ = 0;
	sxy = sxz = syx = syz = szx = szy = 0;

	transformationMatrix = Matrix::generateIdentityMatrix();

	addTranslation(p_tX, p_tY, p_tZ);
	addRotation(p_angleX, p_angleY, p_angleZ);
}


Matrix Transformation::getTransformation(void)
{
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

void Transformation::addTranslation(float p_tX, float p_tY, float p_tZ)
{
	
	tX += p_tX;
	tY += p_tY;
	tZ += p_tZ;

	transformationMatrix = Matrix::generateTranslationMatrix(tX, tY, tZ) * transformationMatrix;
}
void Transformation::addScaling(float p_sX, float p_sY, float p_sZ)
{
	sX += p_sX;
	sY += p_sY;
	sZ += p_sZ;

	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ) * transformationMatrix;
}

void Transformation::addShearing(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy)
{
	sxy += p_sxy;
	sxz += p_sxz;
	syx += p_syx;
	syz += p_syz;
	szx += p_szx;
	szy += p_szy;

	transformationMatrix = Matrix::generateShearingMatrix(sxy, sxz, syx, syz, szx, szy) * transformationMatrix;
}