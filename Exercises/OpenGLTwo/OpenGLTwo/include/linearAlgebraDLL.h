#ifndef __LINEARALGEBRA_H
#define __LINEARALGEBRA_H

#include <ostream>

namespace linearAlgebraDLL
{

static const double PI = 4.0 * atan(1.0);

// Exception used to signal divisions by zero
class DivisionByZeroException {};


// Describe a vector and its standard operations
class Vector
{
    protected:
		// Float array that will contain the vector
        float data[4];

    public:
		// Constructors
        __declspec(dllexport) Vector(void);							
        __declspec(dllexport) Vector(float x, float y, float z);

		// Vectors sum and subtraction operators
        __declspec(dllexport) Vector operator+(Vector &other);
        __declspec(dllexport) Vector operator-(Vector &other);
		// Vector dot product
		__declspec(dllexport) Vector operator*(float s);
		// Vector * scalar product
        __declspec(dllexport) float operator*(Vector &other);
		// Vector cross product
		__declspec(dllexport) Vector operator%(Vector &other);

        // Functions for magnitude, quadratic magnitude and normalization
		__declspec(dllexport) float getMagnitude(void);
		__declspec(dllexport) float getQuadraticMagnitude(void);
        __declspec(dllexport) Vector normalize(void);

        // Functions and operator to access 
		// individual vector elements
		__declspec(dllexport) float operator[](unsigned short i);
        __declspec(dllexport) float get(int i) const { return data[i]; }
        __declspec(dllexport) void set(int i, float val) { data[i] = val; }

		// Print to stream operator
        // This operator needs to return the original stream
        // otherwise it is not possible to chain them
        __declspec(dllexport) friend std::ostream & operator<< (std::ostream &os, const Vector &v);
};


// Describe a point as a child of vector
class Point: public Vector
{
    public:
		// Constructors
        __declspec(dllexport) Point(void);
        __declspec(dllexport) Point(float x, float y, float z);
};

// Describe a matrix
class Matrix
{
	protected:
		// Float array that will contain the matrix
        float data[16];

		// Function for correct floating point calculation for sine and cosine
		__declspec(dllexport) static void floatingPointSinCos(float* sincos, float* degree);

    public:
		// Generic constructors
        __declspec(dllexport) Matrix(void); 
        __declspec(dllexport) Matrix(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33);

		// Generate a translation matrix from three float values
		__declspec(dllexport) static Matrix generateTranslationMatrix(float Tx, float Ty, float Tz);

		// Generate a scaling matrix from three float values
		__declspec(dllexport) static Matrix generateScalingMatrix(float Sx, float Sy, float Sz);
		// Generate a uniform scaling matrix from a float value
		__declspec(dllexport) static Matrix generateUniformScalingMatrix(float S);

		// Generate a rotation matrix about x-axes, from a  float value
		__declspec(dllexport) static Matrix generateXRotationMatrix(float degree);
		// Generate a rotation matrix about y-axes, from a  float value
		__declspec(dllexport) static Matrix generateYRotationMatrix(float degree);
		// Generate a rotation matrix about z-axes, from a  float value
		__declspec(dllexport) static Matrix generateZRotationMatrix(float degree);
		// Generate a rotation matrix about an arbitrary axes
		__declspec(dllexport) static Matrix generateAxesRotationMatrix(Vector axes, float degree);

		//Generate a shearing matrix
		__declspec(dllexport) static Matrix generateShearingMatrix(float Sxy,float Sxz,float Syx,float Syz,float SZx,float Szy);

		// Matrix multiplication
        __declspec(dllexport) Matrix operator*(Matrix &other); 
		// Matrix * scalar multiplication
		__declspec(dllexport) Matrix operator*(float other);
		// Matrix * vector multiplication
		// can also be used for points
        __declspec(dllexport) Vector operator*(Vector &other);

		// Function for transpose matrix
		__declspec(dllexport) Matrix getTranspose();
		// Function for inverse matrix
		__declspec(dllexport) Matrix getInverse();

		//Function for the determinant of the matrix
		__declspec(dllexport) float getDeterminant();

        // Functions and operator to access 
		// individual matrix elements
        __declspec(dllexport) float get(unsigned short row, unsigned short col) const { return data[4*row + col]; }
        __declspec(dllexport) void set(unsigned short row, unsigned short col, float val) { data[4*row + col] = val; }

		//Function to get Vector from matrix
		__declspec(dllexport) Vector getRowAsVector(unsigned short row);
		__declspec(dllexport) Vector getColumnAsVector(unsigned short column);

        // Print to stream operator
        // This operator needs to return the original stream
        // otherwise it is not possible to chain them
        __declspec(dllexport) friend std::ostream & operator<< (std::ostream &os, const Matrix &m);
};

};

#endif
