// Class matrix define a 4x3 matrix

#pragma once
class matrix {

public:
	matrix(float * matrix);
	matrix();
	~matrix();

	float * getMatrix();
	float * getElement(unsigned short row, unsigned short column);
	void setElement(unsigned short row, unsigned short column, float value);

	float * getRowVector(unsigned short row);
	float * getColumnVector(unsigned short column);


private:
	float * matrixPtr;

};