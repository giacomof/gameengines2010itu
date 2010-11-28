#ifdef __DDEngine
#    define linearAlgebra_D __declspec(dllexport)
#else
#    define linearAlgebra_D __declspec(dllimport)
#endif

#ifndef linearalgebra__H
#define linearalgebra__H

#include <iostream>
#include <ostream>
#include <cmath>
#include <omp.h>

// Describe a vector and its standard operations
class linearAlgebra_D Vector
{
    protected:
		// Float array that will contain the vector
        float data[4];

    public:
		// Constructors
        Vector(void);							
        Vector(float x, float y, float z);

		// Vectors sum and subtraction operators
        Vector operator+(Vector &other);
        Vector operator-(Vector &other);
		// Vector dot product
		Vector operator*(float s);
		// Vector * scalar product
        float operator*(Vector &other);
		// Vector cross product
		Vector operator%(Vector &other);
		// Vector comparison
		bool operator==(Vector &other);

        // Functions for magnitude, quadratic magnitude and normalization
		float getMagnitude(void);
		float getQuadraticMagnitude(void);
        Vector normalize(void);

        // Functions and operator to access 
		// individual vector elements
		float operator[](unsigned short i);
        float get(int i) const { return data[i]; }
		float getX(void) const { return data[0]; }
		float getY(void) const { return data[1]; }
		float getZ(void) const { return data[2]; }
        void set(int i, float val) { data[i] = val; }
		void setX(float val) { data[0] = val; }
		void setY(float val) { data[1] = val; }
		void setZ(float val) { data[2] = val; }
		void reset(void) { data[0] = 0; data[1] = 0; data[2] = 0; data[3] = 0; }

		// Print to stream operator
        // This operator needs to return the original stream
        // otherwise it is not possible to chain them
        friend std::ostream & operator<< (std::ostream &os, const Vector &v);
};


// Describe a point as a child of vector
class linearAlgebra_D Point: public Vector
{
    public:
		// Constructors
        Point(void);
        Point(float x, float y, float z);
		// Point * float result
		Point operator*(float s);
		// Points sum operators
        Point operator+(Point &other);
		void reset(void) { data[0] = 0; data[1] = 0; data[2] = 0; data[3] = 1; }
};

class linearAlgebra_D Quaternion
{
	public:
		// Constructors
        Quaternion(void);
        Quaternion(Vector axis, float degree);
		Quaternion(float p_x, float p_y, float p_z, float p_w);

		// Operator overload
		Quaternion operator+(Quaternion &other);
		Quaternion operator*(Quaternion &other);
		Vector operator*(Vector &other);
		// Quaternion comparison
		bool operator==(Quaternion &other);

		// Quaternion magnitude calculation
		float getMagnitude(void);
		// Quaternion normalization
		 Quaternion normalize(void);
		 // Quaternion conjugate calculation
		 Quaternion getConjugate(void);

		// Get/set functions
		Vector getVector(void);
		float getX(void){ return vector.getX();};
		float getY(void){ return vector.getY();};
		float getZ(void){ return vector.getZ();};
		float getW(void);
		void getAxisAngle(Vector *axis, float *angle);
		void setX(float value);
		void setY(float value);
		void setZ(float value);
		void setW(float value);

		// Print functions
		friend std::ostream & operator<< (std::ostream &os, const Quaternion &q);

	protected:
		Vector vector;
		float w;
};

// Describe a matrix
class linearAlgebra_D Matrix
{
	protected:
		// Float array that will contain the matrix
        float data[16];

    public:
		// Generic constructors
        Matrix(void); 
        Matrix(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33);
		Matrix(float * values);

		// Generate the identity matrix
		static Matrix generateIdentityMatrix(void);

		// Generate a translation matrix from three float values
		static Matrix generateTranslationMatrix(float tX, float tY, float tZ);

		// Generate a scaling matrix from three float values
		static Matrix generateScalingMatrix(float sX, float sY, float sZ);
		// Generate a uniform scaling matrix from a float value
		static Matrix generateUniformScalingMatrix(float s);

		// Generate a rotation matrix about x-axes, from a  float value
		static Matrix generateXRotationMatrix(float degree);
		// Generate a rotation matrix about y-axes, from a  float value
		static Matrix generateYRotationMatrix(float degree);
		// Generate a rotation matrix about z-axes, from a  float value
		static Matrix generateZRotationMatrix(float degree);
		// Generate a rotation matrix about an arbitrary axes
		static Matrix generateAxisRotationMatrix(Vector axis, float degree);
		// Generate a rotation matrix from a quaternion
		static Matrix generateQuaternionRotationMatrix(Quaternion q);

		//Generate a shearing matrix
		static Matrix generateShearingMatrix(float sXY,float sXZ,float sYX,float sYZ,float sZX,float sZY);

		// Function for returning the quaternion from a matrix
		static Quaternion getQuaternion(Matrix &mat);

		// Matrix multiplication
        Matrix operator*(Matrix &other); 
		// Matrix * scalar multiplication
		Matrix operator*(float other);
		// Matrix * vector multiplication
		// can also be used for points
        Vector operator*(Vector &other);
		Point operator*(Point &other);
		// Matrix comparison
		bool operator==(Matrix &other);

		// Function for transpose matrix
		Matrix getTranspose();
		// Function for inverse matrix
		Matrix getInverse();

		//Function for the determinant of the matrix
		float getDeterminant();

        // Functions and operator to access 
		// individual matrix elements
        float get(unsigned short row, unsigned short col) const { return data[4*row + col]; }
		float get(unsigned short index) const { return data[index]; }
        void set(unsigned short row, unsigned short col, float val) { data[4*row + col] = val; }
		void set(unsigned short index, float val) { data[index] = val; }
		void getMatrix(float* matrix);

		//Function to get Vector from matrix
		Vector getRowAsVector(unsigned short row);
		Vector getColumnAsVector(unsigned short column);

        // Print to stream operator
        // This operator needs to return the original stream
        // otherwise it is not possible to chain them
        friend std::ostream & operator<< (std::ostream &os, const Matrix &m);
};

#endif
