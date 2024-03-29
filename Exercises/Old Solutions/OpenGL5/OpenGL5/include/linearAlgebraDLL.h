#ifndef __LINEARALGEBRA_H
#define __LINEARALGEBRA_H

#include <ostream>

namespace linearAlgebraDLL
{

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
		// Vector comparison
		__declspec(dllexport) bool operator==(Vector &other);

        // Functions for magnitude, quadratic magnitude and normalization
		__declspec(dllexport) float getMagnitude(void);
		__declspec(dllexport) float getQuadraticMagnitude(void);
        __declspec(dllexport) Vector normalize(void);

        // Functions and operator to access 
		// individual vector elements
		__declspec(dllexport) float operator[](unsigned short i);
        __declspec(dllexport) float get(int i) const { return data[i]; }
		__declspec(dllexport) float getX(void) const { return data[0]; }
		__declspec(dllexport) float getY(void) const { return data[1]; }
		__declspec(dllexport) float getZ(void) const { return data[2]; }
        __declspec(dllexport) void set(int i, float val) { data[i] = val; }
		__declspec(dllexport) void setX(float val) { data[0] = val; }
		__declspec(dllexport) void setY(float val) { data[1] = val; }
		__declspec(dllexport) void setZ(float val) { data[2] = val; }

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

class Quaternion
{
	public:
		// Constructors
        __declspec(dllexport) Quaternion(void);
        __declspec(dllexport) Quaternion(Vector axis, float degree);
		__declspec(dllexport) Quaternion(float p_x, float p_y, float p_z, float p_w);

		// Operator overload
		__declspec(dllexport) Quaternion operator+(Quaternion &other);
		__declspec(dllexport) Quaternion operator*(Quaternion &other);
		__declspec(dllexport) Vector operator*(Vector &other);
		// Quaternion comparison
		__declspec(dllexport) bool operator==(Quaternion &other);

		// Quaternion magnitude calculation
		__declspec(dllexport) float getMagnitude(void);
		// Quaternion normalization
		 __declspec(dllexport) Quaternion normalize(void);
		 // Quaternion conjugate calculation
		 __declspec(dllexport) Quaternion getConjugate(void);

		// Get/set functions
		__declspec(dllexport) Vector getVector(void);
		__declspec(dllexport) float getX(void){ return vector.getX();};
		__declspec(dllexport) float getY(void){ return vector.getY();};
		__declspec(dllexport) float getZ(void){ return vector.getZ();};
		__declspec(dllexport) float getW(void);
		__declspec(dllexport) void getAxisAngle(Vector *axis, float *angle);
		__declspec(dllexport) void setX(float value);
		__declspec(dllexport) void setY(float value);
		__declspec(dllexport) void setZ(float value);
		__declspec(dllexport) void setW(float value);

		// Print functions
		__declspec(dllexport) friend std::ostream & operator<< (std::ostream &os, const Quaternion &q);

	protected:
		Vector vector;
		float w;
};

// Describe a matrix
class Matrix
{
	protected:
		// Float array that will contain the matrix
        float data[16];

    public:
		// Generic constructors
        __declspec(dllexport) Matrix(void); 
        __declspec(dllexport) Matrix(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33);
		__declspec(dllexport) Matrix(float * values);

		// Generate the identity matrix
		__declspec(dllexport) static Matrix generateIdentityMatrix(void);

		// Generate a translation matrix from three float values
		__declspec(dllexport) static Matrix generateTranslationMatrix(float tX, float tY, float tZ);

		// Generate a scaling matrix from three float values
		__declspec(dllexport) static Matrix generateScalingMatrix(float sX, float sY, float sZ);
		// Generate a uniform scaling matrix from a float value
		__declspec(dllexport) static Matrix generateUniformScalingMatrix(float s);

		// Generate a rotation matrix about x-axes, from a  float value
		__declspec(dllexport) static Matrix generateXRotationMatrix(float degree);
		// Generate a rotation matrix about y-axes, from a  float value
		__declspec(dllexport) static Matrix generateYRotationMatrix(float degree);
		// Generate a rotation matrix about z-axes, from a  float value
		__declspec(dllexport) static Matrix generateZRotationMatrix(float degree);
		// Generate a rotation matrix about an arbitrary axes
		__declspec(dllexport) static Matrix generateAxisRotationMatrix(Vector axis, float degree);
		// Generate a rotation matrix from a quaternion
		__declspec(dllexport) static Matrix generateQuaternionRotationMatrix(Quaternion q);

		//Generate a shearing matrix
		__declspec(dllexport) static Matrix generateShearingMatrix(float sXY,float sXZ,float sYX,float sYZ,float sZX,float sZY);

		// Function for returning the quaternion from a matrix
		__declspec(dllexport) static Quaternion getQuaternion(Matrix &mat);

		// Matrix multiplication
        __declspec(dllexport) Matrix operator*(Matrix &other); 
		// Matrix * scalar multiplication
		__declspec(dllexport) Matrix operator*(float other);
		// Matrix * vector multiplication
		// can also be used for points
        __declspec(dllexport) Vector operator*(Vector &other);
		// Matrix comparison
		__declspec(dllexport) bool operator==(Matrix &other);

		// Function for transpose matrix
		__declspec(dllexport) Matrix getTranspose();
		// Function for inverse matrix
		__declspec(dllexport) Matrix getInverse();

		//Function for the determinant of the matrix
		__declspec(dllexport) float getDeterminant();

        // Functions and operator to access 
		// individual matrix elements
        __declspec(dllexport) float get(unsigned short row, unsigned short col) const { return data[4*row + col]; }
		__declspec(dllexport) float get(unsigned short index) const { return data[index]; }
        __declspec(dllexport) void set(unsigned short row, unsigned short col, float val) { data[4*row + col] = val; }
		__declspec(dllexport) void getMatrix(float* matrix);

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
