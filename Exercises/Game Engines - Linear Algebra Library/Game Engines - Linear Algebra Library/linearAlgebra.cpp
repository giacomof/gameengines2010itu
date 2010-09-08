#include <iostream>
#include <cmath>
using namespace std;

#include "linearAlgebra.h"

namespace linearAlgebra
{

//WORKS
// Constructor for vectors without parameters.
Vector::Vector(void)
{
     // I resize the variable to the right size and set the fourth coordinate to 0 since it is a vector.
     data.resize(4);
     data[3] = 0;
}

//WORKS
// Constructor for vectors with parameters.
Vector::Vector(float x, float y, float z)
{
     // Here I also fill in the parameters.
     data.resize(4);
     data[0] = x;
     data[1] = y;
     data[2] = z;
     data[3] = 0;
}

//WORKS
float Vector::getMagnitude(void)
{
     float magnitude = *this * *this;
     magnitude = sqrt(magnitude);
     // I return the result.
     return magnitude;
}

//WORKS
// Function to normalize the vector.
Vector Vector::normalize(void)
{
     // I create a new vector to store the results.
     Vector result;
     // Calculating the length using the the formula "square root of ( x^2 + y^2 + z^2 )", a short cut is multiplying the entire vector with itself.
     float magnitude = this->getMagnitude();
     // Checking that I am not about to divide by zero...
     if (magnitude != 0) {
        // ...if I isn't, I go ahead with normalization.
        result.set(0, this->get(0) / magnitude);
        result.set(1, this->get(1) / magnitude);
        result.set(2, this->get(2) / magnitude);
     } else {
        // ...if I is, I throw an exception to be handle by whoever is calling this function and are smart enough to use try and catch.
        throw DivisionByZeroException();
     }

     // I return the result.
     return result;
}

//WORKS
// Operator overload for the + sign.
Vector Vector::operator+(Vector &other)
{
     // I create a new vector to store the results.
     Vector result;
     // I calculate the result.
     result.set(0, this->get(0) + other.get(0) );
     result.set(1, this->get(1) + other.get(1) );
     result.set(2, this->get(2) + other.get(2) );
     // I return it.
     return result;
}

//WORKS
// Operator overload for the - sign.
Vector Vector::operator-(Vector &other)
{
     // I create a new vector to store the results.
     Vector result;
     // I calculate the result.
     result.set(0, this->get(0) - other.get(0) );
     result.set(1, this->get(1) - other.get(1) );
     result.set(2, this->get(2) - other.get(2) );
     // I return it.
     return result;
}

//WORKS
// Operator overload for the * sign between two vectors.
float  Vector::operator*(Vector &other)
{
     // I create a new float to store the result.
     float result;
     // I calculate the result using the formula "x1*x2 + y1*y2 + z1*z2".
     result = (data[0] * other.data[0] + data[1] * other.data[1] + data[2] * other.data[2]);
     // I return the Dot product.
     return result;
}

//WORKS
// Operator overload for the * sign between a vector and a float.
Vector Vector::operator*(float s)
{
     Vector result;
     // I scale the the vector.
     result.set(0, data[0] * s);
     result.set(1, data[1] * s);
     result.set(2, data[2] * s);
     return result;
}

//WORKS
// Operator overload for the [] symbols (example, Vector[0] returns the content of data[0]).
float  Vector::operator[](int i)
{
     // I access the internal stl vector in the same place as the class vector was accessed and return the content.
     return data[i];
}

//WORKS
// Operator overload for << for sending a vector to the output stream.
std::ostream & operator<< (std::ostream &os, const Vector &v)
{
     // I add to the output stream in the following format: "{x,y,z}".
     os << "{" << v.get(0) << "," << v.get(1) << "," << v.get(2)<< "," << v.get(3) << "}";
     // I return the stream.
     return os;
}

//WORKS
// Constructor for points without parameters.
Point::Point(void)
{
     // This constructor is called identically to the vector constructor. The fourth coordinate is 1 because we're dealing with a point.
     data.resize(4);
     data[3] = 1;
}

//WORKS
// Constructor for points with parameters.
Point::Point(float x, float y, float z)
{
     // We're doing the same here, only also filling in the parameters now.
     data.resize(4);
     data[0] = x;
     data[1] = y;
     data[2] = z;
     data[3] = 1;
}

//WORKS
// Operator overload for << for sending a vector to the output stream.
std::ostream & operator<< (std::ostream &os, const Point &v)
{
     // I add to the output stream in the following format: "{x,y,z}".
     os << "{" << v.get(0) << "," << v.get(1) << "," << v.get(2)<< "," << v.get(3) << "}";
     // I return the stream.
     return os;
}

//WORKS
// Constructor for matrices without parameters.
Matrix::Matrix(void)
{
     data.resize(16);
}

//WORKS
// Constructor for matrices with parameters.
Matrix::Matrix(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33)
{
     data.resize(16);
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


//WORKS
// Operator overload for the * sign between two matrices.
Matrix Matrix::operator*(Matrix &other)
{
     Matrix result;

     // This set of loops will go though each field in the result matrix, then calculate using another loop to resuse code for each multiplication between elements.
     // r is row, c is column, i is row for one element of a multiplication and column for the other element of the same multiplication
     for (int c = 0; c < 4; c++)
         for (int r = 0; r < 4; r++) {
             float temp = 0;
             
             for (int i = 0; i < 4; i++) {
                 temp = temp + this->get(r,i) * other.get(i,c);
             }
             
             result.set(r,c, temp);
         }

     return result;
}

//WORKS
// Operator overload for the * sign between a matrix and a vector.
Vector Matrix::operator*(Vector &other)
{
     Vector result;
     
     // This reuses the code from the matrix * matrix overload. Here I have removed the column loop since a vector only has one.
     for (int r = 0; r < 4; r++) {
         float temp = 0;

         for (int i = 0; i < 4; i++) {
             temp = temp + this->get(r,i) * other.get(i);
         }

         result.set(r, temp);
     }
         
     // Checking that I am not about to divide by zero...
     if (result.get(3) != 0) {
        // ...if i isn't, I go ahead with homogeneous divide. I use a loop to reuse code.
        for (int i = 0; i < 4; i++) {
            result.set(i, result.get(i) / result.get(3));
        }
     }

     return result;
}

//WORKS
// Operator overload for the * sign between a matrix and a point.
Point Matrix::operator*(Point &other)
{
     Point result;
     
     // This reuses the code from the matrix * matrix overload. Here I have removed the column loop since a vector only has one.
     for (int r = 0; r < 4; r++) {
         float temp = 0;

         for (int i = 0; i < 4; i++) {
             temp = temp + this->get(r,i) * other.get(i);
         }

         result.set(r, temp);
     }
         
     // Checking that I am not about to divide by zero...
     if (result.get(3) != 0) {
        // ...if i isn't, I go ahead with homogeneous divide. I use a loop to reuse code.
        for (int i = 0; i < 4; i++) {
            result.set(i, result.get(i) / result.get(3));
        }
     }

     return result;
}

//WORKS
// Operator overload for << for sending a matrix to the output stream.
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
     
     // I return the stream.
     return os;
}

}
