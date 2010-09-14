#include "transformation.h"

Transformation::~Transformation() 
{
	delete this;
}


void Transformation::applyTransformation(void)
{

	// ****************************************
	// *************** TO DO ******************
	// ****************************************

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