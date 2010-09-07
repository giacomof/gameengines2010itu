#include "vector.h"

// vector constructor from an array of float (3)
vector::vector(float * vector) {
	vectorPtr = vector;

}


vector::~vector(void) {

}


float * vector::getElement(unsigned short index) {
	return &vectorPtr[index];
}