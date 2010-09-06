class matrix {

public:
	matrix(float * matrix);
	matrix();
	~matrix();

	float * getMatrix();
	float * getElement(unsigned int row, unsigned int column);
	void setElement(float value, unsigned int row, unsigned int column);


private:
	float * matrixPtr;

};