// LinearAlgebraUnityTest.cpp : Defines the entry point for the console application.
//

#include "linearAlgebraDLL.h"


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


using namespace linearAlgebraDLL;
using namespace boost::unit_test;

// Test case for methods of the Vector class
BOOST_AUTO_TEST_CASE( vector_methods )
{
	// Istantiation of various vectors for the tests
	Vector nX = Vector(1.0f, 0.0f, 0.0f);
	Vector nY = Vector(0.0f, 1.0f, 0.0f);
	Vector nZ = Vector(0.0f, 0.0f, 1.0f);

	Vector temp = Vector(3.0f, 1.0f, 2.0f);
	Vector zero = Vector(0.0f, 0.0f, 0.0f);
	Vector one = Vector(1.0f, 1.0f, 1.0f);
	Vector two = Vector(2.0f, 2.0f, 2.0f);
	

	// Test case for the vector getMagnitude() method
	BOOST_CHECK(nX.getMagnitude() == 1.0f);
	BOOST_CHECK_CLOSE( two.getMagnitude(), 3.4641f, 0.1 );

	// Test case for the vector getQuadraticMagnitude() method
	BOOST_CHECK(one.getQuadraticMagnitude() == 3.0f);
	BOOST_CHECK(two.getQuadraticMagnitude() == 12.0f);

	// Test case for the vector normalize() method
	BOOST_CHECK(nX.normalize() == nX);
	temp = temp.normalize();
	BOOST_CHECK_CLOSE( temp.get(0), 0.802f, 0.1 );
	BOOST_CHECK_CLOSE( temp.get(1), 0.267f, 0.1 );
	BOOST_CHECK_CLOSE( temp.get(2), 0.534f, 0.1 );

	// Test case for the vector get() method
	BOOST_CHECK(nX.get(0) == 1.0f);
	BOOST_CHECK(nZ.get(2) == 1.0f);
	BOOST_CHECK(nY.get(0) != 1.0f);

	// Test case for the vector set() method
	zero.set(0, 2.0f);
	zero.set(1, 2.0f);
	zero.set(2, 2.0f);
	BOOST_CHECK(zero.get(0) == 2.0f);
	BOOST_CHECK(zero.get(1) == 2.0f);
	BOOST_CHECK(zero.get(2) == 2.0f);

}

// Test case for operators overload for Vector class
BOOST_AUTO_TEST_CASE( vector_operators_overloads )
{
	// Istantiation of normalized vectors coincident with axes
	Vector nX = Vector(1.0f, 0.0f, 0.0f);
	Vector nY = Vector(0.0f, 1.0f, 0.0f);
	Vector nZ = Vector(0.0f, 0.0f, 1.0f);

	//Istantiation of various vectors for the tests
	Vector one = Vector(1.0f, 1.0f, 1.0f);


	// Test case for the vector comparison operation
	BOOST_CHECK(nX == nX);
	BOOST_CHECK(one == one);

	// Test case for the vectors sum
	BOOST_CHECK(nX + nY == Vector(1.0f, 1.0f, 0.0f));
	BOOST_CHECK(nX + nZ == Vector(1.0f, 0.0f, 1.0f));
	BOOST_CHECK(nX + nY + nZ == Vector(1.0f, 1.0f, 1.0f));

	// Test case for the vectors subtraction
	BOOST_CHECK(one - nY == Vector(1.0f, 0.0f, 1.0f));
	BOOST_CHECK(one - nZ - nX == Vector(0.0f, 1.0f, 0.0f));
	BOOST_CHECK(nX - nY - nZ == Vector(1.0f, -1.0f, -1.0f));

	// Test case for the product between vectors and scalars
	BOOST_CHECK(nX * 3.0f == Vector(3.0f, 0.0f, 0.0f));
	BOOST_CHECK(one * -5.0f == Vector(-5.0f, -5.0f, -5.0f));

	// Test case for the vector scalar product
	BOOST_CHECK(nX * nY == 0);
	BOOST_CHECK(nX * nX == 1);

	// Test case for the vector cross product
	BOOST_CHECK(nX % nY == nZ);
	BOOST_CHECK(one % nY == Vector(-1.0f, 0.0f, 1.0f));

	// Test case for the vector [] operator
	BOOST_CHECK(nX[0] == 1.0f);
	BOOST_CHECK(nZ[2] == 1.0f);
	BOOST_CHECK(nY[0] != 1.0f);


}

// Test case for operators overload for Point class
BOOST_AUTO_TEST_CASE( point_operators_overload )
{
	// Istantiation of various points for the tests
	Point p1 = Point(1.0f, 1.0f, 1.0f);
	Point p2 = Point(2.0f, 2.0f, 2.0f);
	Point p123 = Point(1.0f, 2.0f, 3.0f);
	Point p234 = Point(2.0f, 3.0f, 4.0f);

	//Istantiation of various vectors for the tests
	Vector zero = Vector(0.0f, 0.0f, 0.0f);
	Vector one = Vector(1.0f, 1.0f, 1.0f);
	Vector test1 = Vector(0.0f, 1.0f, 2.0f);


	// Test case for the points subtraction
	BOOST_CHECK(p123 - p1 == test1);
	BOOST_CHECK(p1 - p1 == zero);

	// Test case for the points and vectors sum
	BOOST_CHECK(p1 + one == p2);
	BOOST_CHECK(p123 + one == p234);
}

// Test case for methods of Quaternion class
BOOST_AUTO_TEST_CASE( quaternion_methods )
{
	// Istantiation of various quaternions for the tests
	Quaternion q1 = Quaternion(Vector(1.0f, 0.0f, 0.0f), 60.0f);
	Quaternion q2 = Quaternion(Vector(0.0f, 0.0f, 1.0f), 45.0f);
	Quaternion q3 = Quaternion(Vector(1.0f, 0.0f, 1.0f), 90.0f);


	// Test case for the quaternion getVector() method
	Vector test1 = q1.getVector();
	BOOST_CHECK_CLOSE( test1.get(0), 0.4999f, 0.1 );
	BOOST_CHECK_CLOSE( test1.get(1), 0.0f, 0.1 );
	BOOST_CHECK_CLOSE( test1.get(2), 0.0f, 0.1 );
	Vector test2 = q3.getVector();
	BOOST_CHECK_CLOSE( test2.get(0), 0.4999f, 0.1 );
	BOOST_CHECK_CLOSE( test2.get(1), 0.0f, 0.1 );
	BOOST_CHECK_CLOSE( test2.get(2), 0.4999f, 0.1 );

	// Test case for the quaternion getW() method
	BOOST_CHECK_CLOSE( q1.getW(), 0.8660f, 0.1 );
	BOOST_CHECK_CLOSE( q3.getW(), 0.7071f, 0.1 );

	// Test case for the quaternion getAxisAngle() method
	Vector * vectorTestPtr;
	vectorTestPtr = &Vector();
	float testF = 0;

	q1.getAxisAngle(vectorTestPtr, &testF);
	BOOST_CHECK(*vectorTestPtr == Vector(1.0f, 0.0f, 0.0f));
	BOOST_CHECK_CLOSE( testF, 60.0f, 0.1 );

	/*
		__declspec(dllexport) void getAxisAngle(Vector *axis, float *angle);
		__declspec(dllexport) void setW(float value);
		*/

}

// Test case for operators overload for Quaternion class
BOOST_AUTO_TEST_CASE( quaternion_operators_overload )
{
	// Istantiation of various quaternions for the tests
	Quaternion q1 = Quaternion(Vector(1.0f, 0.0f, 0.0f), 60.0f);
	Quaternion q2 = Quaternion(Vector(0.0f, 0.0f, 1.0f), 45.0f);
	Quaternion q3 = Quaternion(Vector(1.0f, 0.0f, 1.0f), 90.0f);

	//Istantiation of various vectors for the tests
	Vector zero = Vector(0.0f, 0.0f, 0.0f);
	Vector one = Vector(1.0f, 1.0f, 1.0f);


	// Test case for the quaternions comparison operation
	BOOST_CHECK(q1 == q1);
	BOOST_CHECK(q2 == q2);

	// Test case for the quaternions sum operation
	Quaternion test1 = q1 + q2;
	BOOST_CHECK_CLOSE( test1.getVector().get(0), 0.4999f, 0.1 );
	BOOST_CHECK_CLOSE( test1.getVector().get(1), 0.0f, 0.1 );
	BOOST_CHECK_CLOSE( test1.getVector().get(2), 0.3826f, 0.1 );
	BOOST_CHECK_CLOSE( test1.getW(), 1.7899f, 0.1 );

	Quaternion test2 = q2 + q3;
	BOOST_CHECK_CLOSE( test2.getVector().get(0), 0.4999f, 0.1 );
	BOOST_CHECK_CLOSE( test2.getVector().get(1), 0.0f, 0.1 );
	BOOST_CHECK_CLOSE( test2.getVector().get(2), 0.8826f, 0.1 );
	BOOST_CHECK_CLOSE( test2.getW(), 1.6309f, 0.1 );

	// Test case for the quaternions multiplication operation
	Quaternion test3 = q1 * q2;
	BOOST_CHECK_CLOSE( test3.getVector().get(0), 0.4619f, 0.1 );
	BOOST_CHECK_CLOSE( test3.getVector().get(1), -0.1913f, 0.1 );
	BOOST_CHECK_CLOSE( test3.getVector().get(2), 0.3314f, 0.1 );
	BOOST_CHECK_CLOSE( test3.getW(), 0.8001f, 0.1 );

	Quaternion test4 = q2 * q3;
	BOOST_CHECK_CLOSE( test4.getVector().get(0), 0.4619f, 0.1 );
	BOOST_CHECK_CLOSE( test4.getVector().get(1), 0.1913f, 0.1 );
	BOOST_CHECK_CLOSE( test4.getVector().get(2), 0.7325f, 0.1 );
	BOOST_CHECK_CLOSE( test4.getW(), 0.4619f, 0.1 );



}

