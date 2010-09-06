#include "matrix.h"

matrix::matrix(float * matrix) {
	matrixPtr = matrix;
}

matrix::matrix() {
	float matrix[4][3];

	for(unsigned int i = 1; i < 5; i++) {
		for(unsigned int j = 1; j < 4; j++) {
			matrix[i][j] = 0;
		}
	}

	float * matrixPtr = &matrix[0][0];
}

float * matrix::getMatrix() {
	return matrixPtr;
}

float * matrix::getElement(unsigned int row, unsigned int column) {

	unsigned int element = (row-1)*3 + column;
	return &matrixPtr[element];
}

void matrix::setElement(float value, unsigned int row, unsigned int column) {
	unsigned int element = (row-1)*3 + column;
	matrixPtr[element] = value;
}
