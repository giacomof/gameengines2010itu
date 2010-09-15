#include "transformation.h"

Transformation::~Transformation() 
{
}


Matrix Transformation::getTransformation(void)
{
	Matrix result;
	// ****************************************
	// *************** TO DO ******************
	// ****************************************
	return result;

}

void Transformation::addTransformation(Transformation t)
{
	transformationStack = retrieveTransformationStack();
	transformationStack.push_back(t);
	this->setTransformationStack(transformationStack);
}

void Transformation::removeTransformation(void)
{
	transformationStack = retrieveTransformationStack();
	transformationStack.pop_back();
}

void Transformation::setTransformationStack(list<Transformation> tStack)
{
	transformationStack = tStack;
}

Transformation Transformation::retrieveTransformation(void)
{
	transformationStack = retrieveTransformationStack();
	Transformation temp = transformationStack.back();
	transformationStack.pop_back();
	return temp;
	
}

list<Transformation> Transformation::retrieveTransformationStack(void)
{
	return transformationStack;
}


Matrix TraslationTransformation::getTransformation(void) 
{
	return Matrix::generateTranslationMatrix(tX, tY, tZ).getTranspose();
}


ScalingTransformation::ScalingTransformation(float x, float y, float z)
{
	sX = x;
	sY = y;
	sZ = z;
}

Matrix ScalingTransformation::getTransformation(void) 
{
	return Matrix::generateScalingMatrix(sX, sY, sZ).getTranspose();
}