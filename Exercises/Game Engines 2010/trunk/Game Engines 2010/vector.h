// Class vector define a vector of 4 elements

#pragma once
class vector{

public:
	vector(float * vector);

	~vector(void);

	float * getElement(unsigned short index);

private:
	float * vectorPtr;
};

