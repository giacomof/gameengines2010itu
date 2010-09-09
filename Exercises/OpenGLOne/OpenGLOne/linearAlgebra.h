#ifndef __LINEARALGEBRA_H
#define __LINEARALGEBRA_H

#include <vector>
#include <ostream>

namespace linearAlgebra
{

static const double PI = 4.0 * atan(1.0);

// Exception used to signal divisions by zero
class DivisionByZeroException {};


// Describe a vector and its standard operations
class Vector
{
    protected:
        std::vector<float> data;

    public:
		// Constructors
        Vector(void);							
        Vector(float x, float y, float z);

		// Vectors sum and subtraction operators
        Vector operator+(Vector &other);
        Vector operator-(Vector &other);
		// Vector dot product
		Vector operator*(float s);
		// Vector scalar product
        float operator*(Vector &other);

        // Functions for magnitude and normalization
		float getMagnitude(void);
        Vector normalize(void);

        // Functions and operator to access 
		// individual vector elements
		float operator[](int i);
        float get(int i) const { return data[i]; }
        void set(int i, float val) { data[i] = val; }

		// Print to stream operator
        // This operator needs to return the original stream
        // otherwise it is not possible to chain them
        friend std::ostream & operator<< (std::ostream &os, const Vector &v);
};


// Describe a point as a child of vector
class Point: public Vector
{
    public:
		// Constructors
        Point(void);
        Point(float x, float y, float z);
};

// Describe a matrix
class Matrix
{
	protected:
        std::vector<float> data;

    public:
		// Generic constructors
        Matrix(void); 
        Matrix(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33);

		// Generate a translation matrix from three float values
		static Matrix generateTranslationMatrix(float Tx, float Ty, float Tz);

		// Generate a scaling matrix from three float values
		static Matrix generateScalingMatrix(float Sx, float Sy, float Sz);
		// Generate a uniform scaling matrix from a float value
		static Matrix generateUniformScalingMatrix(float S);

		// Generate a rotation matrix about x-axis, from a  float value
		static Matrix generateXRotationMatrix(float degree);
		// Generate a rotation matrix about y-axis, from a  float value
		static Matrix generateYRotationMatrix(float degree);
		// Generate a rotation matrix about z-axis, from a  float value
		static Matrix generateZRotationMatrix(float degree);

		// Matrix multiplication
        Matrix operator*(Matrix &other); 
		// Matrix * vector multiplication
		// can also be used for points
        Vector operator*(Vector &other);

		// Function for transpose matrix
		Matrix getTranspose();

        // Functions and operator to access 
		// individual matrix elements
        float get(int row, int col) const { return data[4*row + col]; }
        void set(int row, int col, float val) { data[4*row + col] = val; }

        // Print to stream operator
        // This operator needs to return the original stream
        // otherwise it is not possible to chain them
        friend std::ostream & operator<< (std::ostream &os, const Matrix &m);
};

};

#endif
