#include "matrix.h"


// matrix constructor from an array of float (12)
matrix::matrix(float * matrix) {
	matrixPtr = matrix;
}

// emtpy matrix constructor 
matrix::matrix() {
	float matrix[4][3];

	for(unsigned int i = 1; i < 5; i++) {
		for(unsigned int j = 1; j < 4; j++) {
			matrix[i][j] = 0;
		}
	}

	float * matrixPtr = &matrix[0][0];
}

// return a pointer to the matrix
float * matrix::getMatrix() {
	return matrixPtr;
}

// return a pointer to an element of the matrix, based on coordinate going from 1-4 for rows, and 1-3 for columns
float * matrix::getElement(unsigned short row, unsigned short column) {

	unsigned int element = (row-1)*3 + column-1;
	return &matrixPtr[element];
}

// set one element of the matrix
void matrix::setElement(unsigned short row, unsigned short column, float value) {
	unsigned int element = (row-1)*3 + column-1;
	matrixPtr[element] = value;
}


float * matrix::getRowVector(unsigned short row) {

	float vector[4];
	unsigned short temp = 3*(row-1);
	for(unsigned short i = 0; i < 3; i++) {
		vector[i] = matrixPtr[temp+i];
	}
	
	if (row == 4) {
		vector[3] = 1;
	} else {
		vector[3] = 0;
	}


	return &vector[0];
}


float * matrix::getColumnVector(unsigned short column) {
	float vector[4];

	return &vector[0];

}