#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "LinearAlgebraDLL.h"

using namespace std;

static const float PI = 3.14159f;

namespace linearAlgebraDLL
{

// Constructor for vectors without parameter
Vector::Vector(void)
{
     // Resize the variable to the right size and set the fourth coordinate to 0 since it is a vector.
     data[3] = 0;
}

// Constructor for vectors with parameters
Vector::Vector(float x, float y, float z)
{
     // Fill in the parameters
     data[0] = x;
     data[1] = y;
     data[2] = z;
     data[3] = 0;
}

// Function to calculate the magnitude of a vector
float Vector::getMagnitude(void)
{
     float magnitude = *this * *this;
     magnitude = sqrt(magnitude);
     // Return the result
     return magnitude;
}

// Returns the quadratic magnitude
float Vector::getQuadraticMagnitude(void) 
{	
	return *this * *this;
}

// Function to normalize the vector
Vector Vector::normalize(void)
{
     // Create a new vector to store the results
     Vector result;
     // Calculate the magnitude of the vector
     float magnitude = this->getMagnitude();

     // If the vector has magnitude = 0, then return an empty vector
     if (magnitude == 0) {

		result.set(0, 0);
        result.set(1, 0);
        result.set(2, 0);

		// If the vector magnitude is 1, then it is already normalized
     } else if (magnitude == 1) {
        
		result.set(0, this->get(0));
        result.set(1, this->get(1));
        result.set(2, this->get(2));

		// else, normalize the vector
     } else {
		result.set(0, this->get(0) / magnitude);
        result.set(1, this->get(1) / magnitude);
        result.set(2, this->get(2) / magnitude);
	 }

     // Return the result
     return result;
}

// Operator overload for the + sign
Vector Vector::operator+(Vector &other)
{
     // Create a new vector to store the results
     Vector result;
     // Calculate the result
     result.set(0, this->get(0) + other.get(0) );
     result.set(1, this->get(1) + other.get(1) );
     result.set(2, this->get(2) + other.get(2) );

	 // If was a sum between a point and a vector
	 // the result will be a point
	 if ((data[3] == 1) || (other.get(3) == 1)) {
		 result.set(3, 1);
	 }

     return result;
}

// Operator overload for the - sign between two vectors
Vector Vector::operator-(Vector &other)
{
     // Create a new vector to store the results
     Vector result;
     // Calculate the result
     result.set(0, this->get(0) - other.get(0) );
     result.set(1, this->get(1) - other.get(1) );
     result.set(2, this->get(2) - other.get(2) );
     // Return it
     return result;
}

// Operator overload for the * sign between two vectors
float Vector::operator*(Vector &other)
{
     // Create a new float to store the result
     float result;
     // Calculate the result
     result = (data[0] * other.data[0] + data[1] * other.data[1] + data[2] * other.data[2]);
     // Return the Dot product
     return result;
}

// Operator overload for the * sign between a vector and a float
Vector Vector::operator*(float s)
{
	Vector result;
	// Scale the the vector
	result.set(0, data[0] * s);
	result.set(1, data[1] * s);
	result.set(2, data[2] * s);
	return result;
}

// Vector cross product
Vector Vector::operator%(Vector &other)
{
	Vector result;

	// Scale the the vector
	result.set(0, data[1] * other.data[2] - data[2] * other.data[1]);
	result.set(1, -data[0] * other.data[2] - data[2] * other.data[0]);
	result.set(2, data[0] * other.data[1] - data[1] * other.data[0]);

	return result;
}

// Operator Overload for vector comparison
bool Vector::operator==(Vector &other)
{
	bool equal = true;

	for (unsigned short i = 0; i < 4; i++) {
		if (data[i] != other.data[i]) {
			equal = false;
		}
	}

	return equal;
}

// Operator overload for the [] symbols (Vector[0] returns the content of data[0])
float  Vector::operator[](unsigned short i)
{
     // Access the internal stl vector in the same place as the class vector was accessed and return the content
     return data[i];
}

// Operator overload for << for sending a vector to the output stream
std::ostream & operator<< (std::ostream &os, const Vector &v)
{
     // Add to the output stream in the following format: "{x,y,z}"
     os << "{" << v.get(0) << "," << v.get(1) << "," << v.get(2)<< "," << v.get(3) << "}";
     // Return the stream
     return os;
}

// Constructor for points without parameters
Point::Point(void)
{
     // This constructor is called identically to the vector constructor. The fourth coordinate is 1 because we're dealing with a point
     data[3] = 1;
}

// Constructor for points with parameters
Point::Point(float x, float y, float z)
{
     // Fill in the parameters
     data[0] = x;
     data[1] = y;
     data[2] = z;
     data[3] = 1;
}

// Constructor for quaternions without parameter
Quaternion::Quaternion(void)
{
	vector = Vector(0.0f, 0.0f, 0.0f);
	w = 0;
}

// Constructor for quaternions from a vector and an angle
Quaternion::Quaternion(Vector axis, float angle)
{
	float sinAngle;
	angle *= 0.5f;

	Vector vn = axis.normalize();
 
	sinAngle = sin(angle*PI/180);
 
	vn = vn * sinAngle;

	for (unsigned short i = 0; i < 3; i++) {
		vector.set(i, vn.get(i));
	}
	w = cos(angle*PI/180);
}

// Operator overload for sum between quaternions
Quaternion Quaternion::operator+(Quaternion &other)
{
	Vector resultVector = Vector(this->getVector() + other.getVector());

	float resultDegree = this->getW() + other.getW();

	Quaternion resultQuaternion = Quaternion();
	resultQuaternion.vector = resultVector;
	resultQuaternion.w = resultDegree;

	return resultQuaternion;
}

// Operator overload for multiplication between quaternions
Quaternion Quaternion::operator*(Quaternion &other)
{
	Vector resultVector = Vector();
	resultVector.set(0, w * other.vector.get(0) + vector.get(0) * other.w + vector.get(1) * other.vector.get(2) - vector.get(2) * other.vector.get(1)); 
	resultVector.set(1, w * other.vector.get(1) + vector.get(1) * other.w + vector.get(2) * other.vector.get(0) - vector.get(0) * other.vector.get(2)); 
	resultVector.set(2, w * other.vector.get(2) + vector.get(2) * other.w + vector.get(0) * other.vector.get(1) - vector.get(1) * other.vector.get(0));

	float resultW = w * other.w - vector.get(0) * other.vector.get(0) - vector.get(1) * other.vector.get(1) - vector.get(2) * other.vector.get(2);

	Quaternion resultQuaternion = Quaternion();
	resultQuaternion.vector = resultVector;
	resultQuaternion.w = resultW;

	return resultQuaternion;
}

// Quaternion comparison
bool Quaternion::operator==(Quaternion &other)
{
	bool equal;

	if ((vector == other.getVector()) && (w == other.getW())) {
		equal = true;
	} else {
		equal = false;
	}

	return equal;
}


//Function for getting members of quaternion
Vector Quaternion::getVector(void)
{
	return vector;
}

float Quaternion::getW(void)
{
	return w;
}

// Function for getting axis and angle from quaternion
void Quaternion::getAxisAngle(Vector *axis, float *angle)
{
	float magnitude = vector.getMagnitude();
	axis->set(0,  vector.get(0) / magnitude);
	axis->set(1,  vector.get(1) / magnitude);
	axis->set(2,  vector.get(2) / magnitude);
	*angle = acos(w*PI/180) * 2.0f;
}

// Function to output quaternions
std::ostream & operator<< (std::ostream &os, const Quaternion &q)
{
	// I add to the output stream in the following format: "{x,y,z}"
     os << "{(" << q.vector.get(0) << "," << q.vector.get(1) << "," << q.vector.get(2)<< ")," << q.w << "}";
    // I return the stream
    return os;
}


// Constructor for matrices without parameters
Matrix::Matrix(void)
{
}

// Constructor for matrices with parameters
Matrix::Matrix(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33)
{
     data[0]  = a00;
     data[1]  = a01;
     data[2]  = a02;
     data[3]  = a03;
     data[4]  = a10;
     data[5]  = a11;
     data[6]  = a12;
     data[7]  = a13;
     data[8]  = a20;
     data[9]  = a21;
     data[10] = a22;
     data[11] = a23;
     data[12] = a30;
     data[13] = a31;
     data[14] = a32;
     data[15] = a33;
}

// Constructor for matrices from an array of float
Matrix::Matrix(float * values) 
{
	for(unsigned short i=0; i<16; i++) {
		data[i]=values[i];
	}

}

// Generate the identity matrix
Matrix Matrix::generateIdentityMatrix(void)
{
	Matrix result;

	result.set(0,0,1);
	result.set(0,1,0);
	result.set(0,2,0);
	result.set(0,3,0);
	result.set(1,0,0);
	result.set(1,1,1);
	result.set(1,2,0);
	result.set(1,3,0);
	result.set(2,0,0);
	result.set(2,1,0);
	result.set(2,2,1);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Generate a translation matrix from three float values
Matrix Matrix::generateTranslationMatrix(float tX, float tY, float tZ) 
 {
	Matrix result;

	result.set(0,0,1);
	result.set(0,1,0);
	result.set(0,2,0);
	result.set(0,3,tX);
	result.set(1,0,0);
	result.set(1,1,1);
	result.set(1,2,0);
	result.set(1,3,tY);
	result.set(2,0,0);
	result.set(2,1,0);
	result.set(2,2,1);
	result.set(2,3,tZ);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
 }

// Generate a scaling matrix from three float values
Matrix Matrix::generateScalingMatrix(float sX, float sY, float sZ)
{
	Matrix result;

	result.set(0,0,sX);
	result.set(0,1,0);
	result.set(0,2,0);
	result.set(0,3,0);
	result.set(1,0,0);
	result.set(1,1,sY);
	result.set(1,2,0);
	result.set(1,3,0);
	result.set(2,0,0);
	result.set(2,1,0);
	result.set(2,2,sZ);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Generate a uniform scaling matrix from a float value
Matrix Matrix::generateUniformScalingMatrix(float s)
{
	Matrix result;

	result.set(0,0,s);
	result.set(0,1,0);
	result.set(0,2,0);
	result.set(0,3,0);
	result.set(1,0,0);
	result.set(1,1,s);
	result.set(1,2,0);
	result.set(1,3,0);
	result.set(2,0,0);
	result.set(2,1,0);
	result.set(2,2,s);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Generate a rotation matrix about x-axes, from a  float value
Matrix Matrix::generateXRotationMatrix(float degree)
{
	Matrix result;
	float sincos[2];
	sincos[0] = sin(degree*PI/180);
	sincos[1] = cos(degree*PI/180);

	result.set(0,0,1);
	result.set(0,1,0);
	result.set(0,2,0);
	result.set(0,3,0);
	result.set(1,0,0);
	result.set(1,1,sincos[1]);
	result.set(1,2,-sincos[0]);
	result.set(1,3,0);
	result.set(2,0,0);
	result.set(2,1,sincos[0]);
	result.set(2,2,sincos[1]);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Generate a rotation matrix about y-axes, from a  float value
Matrix Matrix::generateYRotationMatrix(float degree)
{
	Matrix result;
	float sincos[2];
	sincos[0] = sin(degree*PI/180);
	sincos[1] = cos(degree*PI/180);

	result.set(0,0,sincos[1]);
	result.set(0,1,0);
	result.set(0,2,sincos[0]);
	result.set(0,3,0);
	result.set(1,0,0);
	result.set(1,1,1);
	result.set(1,2,0);
	result.set(1,3,0);
	result.set(2,0,-sincos[0]);
	result.set(2,1,0);
	result.set(2,2,sincos[1]);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Generate a rotation matrix about z-axes, from a  float value
Matrix Matrix::generateZRotationMatrix(float degree)
{
	Matrix result;
	float sincos[2];
	sincos[0] = sin(degree*PI/180);
	sincos[1] = cos(degree*PI/180);

	result.set(0,0,sincos[1]);
	result.set(0,1,-sincos[0]);
	result.set(0,2,0);
	result.set(0,3,0);
	result.set(1,0,sincos[0]);
	result.set(1,1,sincos[1]);
	result.set(1,2,0);
	result.set(1,3,0);
	result.set(2,0,0);
	result.set(2,1,0);
	result.set(2,2,1);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Generate a rotation matrix about an arbitrary axes, from a Vector and a float
Matrix Matrix::generateAxesRotationMatrix(Vector axes, float degree)
{
	Matrix result;
	float sincos[2];
	float k;
	sincos[0] = sin(degree*PI/180);
	sincos[1] = cos(degree*PI/180);

	k = 1-sincos[1];

	result.set(0,0,(axes.get(0)*axes.get(0)*k)+sincos[1]);
	result.set(0,1,(axes.get(0)*axes.get(1)*k)-(axes.get(2)*sincos[0]));
	result.set(0,2,(axes.get(0)*axes.get(2)*k)+(axes.get(1)*sincos[0]));
	result.set(0,3,0);
	result.set(1,0,(axes.get(0)*axes.get(1)*k)+(axes.get(2)*sincos[0]));
	result.set(1,1,(axes.get(1)*axes.get(1)*k)+sincos[1]);
	result.set(1,2,(axes.get(1)*axes.get(2)*k)-(axes.get(0)*sincos[0]));
	result.set(1,3,0);
	result.set(2,0,(axes.get(0)*axes.get(1)*k)-(axes.get(1)*sincos[0]));
	result.set(2,1,(axes.get(1)*axes.get(2)*k)+(axes.get(0)*sincos[0]));
	result.set(2,2,(axes.get(2)*axes.get(2)*k)+sincos[1]);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Generate a rotation matrix from a quaternion
Matrix Matrix::generateQuaternionRotationMatrix(Quaternion q)
{
	float x2 = q.getVector().get(0) * q.getVector().get(0);
	float y2 = q.getVector().get(1) * q.getVector().get(1);
	float z2 = q.getVector().get(2) * q.getVector().get(2);
	float xy = q.getVector().get(0) * q.getVector().get(1);
	float xz = q.getVector().get(0) * q.getVector().get(2);
	float yz = q.getVector().get(1) * q.getVector().get(2);
	float dx = q.getW() * q.getVector().get(0);
	float dy = q.getW() * q.getVector().get(1);
	float dz = q.getW() * q.getVector().get(2);
 
	Matrix result;

	result.set(0,0,(1.0f - 2.0f * (y2 + z2)));
	result.set(0,1,(2.0f * (xy - dz)));
	result.set(0,2,(2.0f * (xz + dy)));
	result.set(0,3,0);
	result.set(1,0,(2.0f * (xy + dz)));
	result.set(1,1,(1.0f - 2.0f * (x2 + z2)));
	result.set(1,2,(2.0f * (yz - dx)));
	result.set(1,3,0);
	result.set(2,0,(2.0f * (xz - dy)));
	result.set(2,1,(2.0f * (yz + dx)));
	result.set(2,2,(1.0f - 2.0f * (x2 + y2)));
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;

}

// Generate a shearing matrix from six float values 
Matrix Matrix::generateShearingMatrix(float sXY,float sXZ,float sYX,float sYZ,float sZX,float sZY)
{
	Matrix result;

	result.set(0,0,1);
	result.set(0,1,sXY);
	result.set(0,2,sXZ);
	result.set(0,3,0);
	result.set(1,0,sYX);
	result.set(1,1,1);
	result.set(1,2,sYZ);
	result.set(1,3,0);
	result.set(2,0,sZX);
	result.set(2,1,sZY);
	result.set(2,2,1);
	result.set(2,3,0);
	result.set(3,0,0);
	result.set(3,1,0);
	result.set(3,2,0);
	result.set(3,3,1);

	return result;
}

// Operator overload for the * sign between two matrices
Matrix Matrix::operator*(Matrix &other)
{
     Matrix result;

     // This set of loops will go though each field in the result matrix, 
	 // then calculate using another loop to resuse code for each multiplication 
	 // between elements r is row, c is column, i is row for one element 
	 // of a multiplication and column for the other element of the same multiplication
     for (unsigned short c = 0; c < 4; c++)
         for (unsigned short r = 0; r < 4; r++) {
             float temp = 0;
             
             for (unsigned short i = 0; i < 4; i++) {
                 temp = temp + this->get(r,i) * other.get(i,c);
             }
             
             result.set(r,c, temp);
         }

     return result;
}

// Operator overload for the * sign between a matrix and a scalar
Matrix Matrix::operator*(float other)
{
	Matrix result;

	for (unsigned short r = 0; r < 4; r++)
         for (unsigned short c = 0; c < 4; c++) {             
             
             result.set(r,c, data[4*r + c]*other);
         }

     return result;
}

// Operator overload for the * sign between a matrix and a vector
Vector Matrix::operator*(Vector &other)
{
     Vector result;
     
     // This reuses the code from the matrix * matrix overload. Here I have removed the column loop since a vector only has one
     for (unsigned int r = 0; r < 4; r++) {
         float temp = 0;

         for (unsigned short i = 0; i < 4; i++) {
             temp = temp + this->get(r,i) * other.get(i);
         }

         result.set(r, temp);
     }
         
     // Checking that is not about to divide by zero
     if (result.get(3) != 0) {

        for (unsigned short i = 0; i < 4; i++) {
            result.set(i, result.get(i) / result.get(3));
        }
     }

     return result;
}

// Operator overload for matrix comparison
bool Matrix::operator==(Matrix &other)
{
	bool equal = true;

	for (unsigned short r = 0; r < 4; r++) {
		for (unsigned short c = 0; c < 4; c++) {
			if(data[r*4 + c] != other.get(r, c)) {
				equal = false;
			}
		}
	}

	return equal;
}

// Function for the transpose matrix
Matrix Matrix::getTranspose() 
{
	// Result matrix
	Matrix result;

	// Inversion of row and columns
	for (unsigned int r = 0; r < 4; r++) {
		for (unsigned int c = 0; c < 4; c++) {
			result.set(c,r,this->get(r,c));
		}
	}

	return result;
}

// Calculate the inverse of a 4x4 matrix with Laplace expansion algorithm
// if the determinant is != 0, otherwise return the identity matrix
Matrix Matrix::getInverse()
{
	Matrix result;

	float temp0 = data[0]*data[5] - data[1]*data[4];
	float temp1 = data[0]*data[6] - data[2]*data[4];
	float temp2 = data[0]*data[7] - data[3]*data[4];
	float temp3 = data[1]*data[6] - data[2]*data[5];
	float temp4 = data[1]*data[7] - data[3]*data[5];
	float temp5 = data[2]*data[7] - data[3]*data[6];
	float temp6 = data[8]*data[13] - data[9]*data[12];
	float temp7 = data[8]*data[14] - data[10]*data[12];
	float temp8 = data[8]*data[15] - data[11]*data[12];
	float temp9 = data[9]*data[14] - data[10]*data[13];
	float temp10 = data[9]*data[15] - data[11]*data[13];
	float temp11 = data[10]*data[15] - data[11]*data[14];

	float determinant = temp0*temp11 - temp1*temp10 + temp2*temp9 + 
						temp3*temp8 - temp4*temp7 + temp5*temp6;

	if (determinant != 0) {
		result.set(0,0,data[5]*temp11-data[6]*temp10+data[7]*temp9);
		result.set(0,1,-data[1]*temp11+data[2]*temp10-data[3]*temp9);
		result.set(0,2,data[13]*temp5-data[14]*temp4+data[15]*temp3);
		result.set(0,3,-data[9]*temp5+data[10]*temp4-data[11]*temp3);
		result.set(1,0,-data[4]*temp11+data[6]*temp8-data[7]*temp7);
		result.set(1,1,data[0]*temp11-data[2]*temp8+data[3]*temp7);
		result.set(1,2,-data[12]*temp5+data[14]*temp2-data[15]*temp1);
		result.set(1,3,data[8]*temp5-data[10]*temp2+data[11]*temp1);
		result.set(2,0,data[4]*temp10-data[5]*temp8+data[7]*temp6);
		result.set(2,1,-data[0]*temp10+data[1]*temp8-data[3]*temp6);
		result.set(2,2,data[12]*temp4-data[13]*temp2+data[15]*temp0);
		result.set(2,3,-data[8]*temp4+data[9]*temp2-data[11]*temp0);
		result.set(3,0,-data[4]*temp9+data[5]*temp7-data[6]*temp6);
		result.set(3,1,data[0]*temp9-data[1]*temp7+data[2]*temp6);
		result.set(3,2,-data[12]*temp3+data[13]*temp1-data[14]*temp0);
		result.set(3,3,data[8]*temp3-data[9]*temp1+data[10]*temp0);
	} else {
		result = Matrix::generateIdentityMatrix();
		return result;
	}

	return result*(1/determinant);
}



// Calculate the determinant of a 4x4 matrix with Laplace expansion algorithm
float Matrix::getDeterminant()
{

	float temp0 = data[0]*data[5] - data[1]*data[4];
	float temp1 = data[0]*data[6] - data[2]*data[4];
	float temp2 = data[0]*data[7] - data[3]*data[4];
	float temp3 = data[1]*data[6] - data[2]*data[5];
	float temp4 = data[1]*data[7] - data[3]*data[5];
	float temp5 = data[2]*data[7] - data[3]*data[6];
	float temp6 = data[8]*data[13] - data[9]*data[12];
	float temp7 = data[8]*data[14] - data[10]*data[12];
	float temp8 = data[8]*data[15] - data[11]*data[12];
	float temp9 = data[9]*data[14] - data[10]*data[13];
	float temp10 = data[9]*data[15] - data[11]*data[13];
	float temp11 = data[10]*data[15] - data[11]*data[14];

	return temp0*temp11 - temp1*temp10 + temp2*temp9 + 
			temp3*temp8 - temp4*temp7 + temp5*temp6;
}

// Return the matrix inside the float vector received as argument
void Matrix::getMatrix(float * matrix) {

	for(unsigned short i=0; i<16; i++) {
		matrix[i]=data[i];
	}
}



// Return the specified row as vector
Vector Matrix::getRowAsVector(unsigned short row)
{
	 Vector result;

	 for (unsigned int i = 0; i < 4; i++){
		 result.set(i,this->get(row,i));
	 }

	 return result;
}

// Return the specified column as vector
Vector Matrix::getColumnAsVector(unsigned short column)
{
	 Vector result;

	for (unsigned int i = 0; i < 4; i++){
		 result.set(i,this->get(i,column));
	 }

	return result;
}

// Operator overload for << for sending a matrix to the output stream
std::ostream & operator<< (std::ostream &os, const Matrix &m)
{
     // I add to the output stream in the following format: "{a00,a01,a02,a03
     //                                                       a10,a11,a12,a13
     //                                                       a20,a21,a22,a23
     //                                                       a30,a31,a32,a33}".
     os << endl;
     os << "{" << m.get(0,0) << "," << m.get(0,1) << "," << m.get(0,2) << "," << m.get(0,3) << endl;
     os << " " << m.get(1,0) << "," << m.get(1,1) << "," << m.get(1,2) << "," << m.get(1,3) << endl;
     os << " " << m.get(2,0) << "," << m.get(2,1) << "," << m.get(2,2) << "," << m.get(2,3) << endl;
     os << " " << m.get(3,0) << "," << m.get(3,1) << "," << m.get(3,2) << "," << m.get(3,3) << "}";
     
     // I return the stream
     return os;
}

}
