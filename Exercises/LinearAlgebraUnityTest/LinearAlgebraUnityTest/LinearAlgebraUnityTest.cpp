// LinearAlgebraUnityTest.cpp : Defines the entry point for the console application.
//

#include "linearAlgebraDLL.h"


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


using namespace linearAlgebraDLL;
using namespace boost::unit_test;

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

BOOST_AUTO_TEST_CASE( vector_methods )
{
	// Istantiation of normalized vectors coincident with axes
	Vector nX = Vector(1.0f, 0.0f, 0.0f);
	Vector nY = Vector(0.0f, 1.0f, 0.0f);
	Vector nZ = Vector(0.0f, 0.0f, 1.0f);
	//Istantiation of various vectors for the tests
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

BOOST_AUTO_TEST_CASE( vector_methods )
{
	// Istantiation of normalized vectors coincident with axes
	Vector nX = Vector(1.0f, 0.0f, 0.0f);
	Vector nY = Vector(0.0f, 1.0f, 0.0f);
	Vector nZ = Vector(0.0f, 0.0f, 1.0f);
	//Istantiation of various vectors for the tests
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

//point - point
//point + vector