#ifndef __LINEARALGEBRA_H
#define __LINEARALGEBRA_H

#include <vector>
#include <ostream>

namespace linearAlgebra
{

class DivisionByZeroException {};

class Vector
{
    protected:
        std::vector<float> data;

    public:
        Vector(void); // default constructor
        Vector(float x, float y, float z);

        Vector operator+(Vector &other);
        Vector operator-(Vector &other);
        float  operator*(Vector &other);
        Vector operator*(float s);
        float  operator[](int i);
		float getMagnitude(void);
        Vector normalize(void);

        // functions to access individual matrix elements
        float get(int i) const { return data[i]; }
        void set(int i, float val) { data[i] = val; }

        // this op needs to return the original stream
        // otherwise it is not possible to chain them
        friend std::ostream & operator<< (std::ostream &os, const Vector &v);
};

class Point: public Vector
{
    public:
        Point(void);
        Point(float x, float y, float z);
		friend std::ostream & operator<< (std::ostream &os, const Point &v);
};

class Matrix
{
    protected:
        std::vector<float> data;

    public:
        Matrix(void); // default constructor
        Matrix(float a00, float a01, float a02, float a03,
               float a10, float a11, float a12, float a13,
               float a20, float a21, float a22, float a23,
               float a30, float a31, float a32, float a33);

        Matrix operator*(Matrix &other); // matrix multiplication
        Vector operator*(Vector &other); // matrix * vector multiplication
		Point operator*(Point &other); // matrix * vector multiplication

        // functions to access individual matrix elements
        float get(int row, int col) const { return data[4*row + col]; }
        void set(int row, int col, float val) { data[4*row + col] = val; }

        // this op needs to return the original stream
        // otherwise it is not possible to chain them
        friend std::ostream & operator<< (std::ostream &os, const Matrix &m);
};

};

#endif
