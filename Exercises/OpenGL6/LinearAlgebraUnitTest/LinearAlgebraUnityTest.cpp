// LinearAlgebraUnityTest.cpp : Defines the entry point for the console application.
//

#include "linearAlgebraDLL.h"


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define PI = 3.141592

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
	

	// Test for the vector getMagnitude() method
	BOOST_CHECK(nX.getMagnitude() == 1.0f);
	BOOST_CHECK_CLOSE( two.getMagnitude(), 3.4641f, 0.1 );

	// Test for the vector getQuadraticMagnitude() method
	BOOST_CHECK(one.getQuadraticMagnitude() == 3.0f);
	BOOST_CHECK(two.getQuadraticMagnitude() == 12.0f);

	// Test for the vector normalize() method
	BOOST_CHECK(nX.normalize() == nX);
	temp = temp.normalize();
	BOOST_CHECK_CLOSE( temp.get(0), 0.802f, 0.1 );
	BOOST_CHECK_CLOSE( temp.get(1), 0.267f, 0.1 );
	BOOST_CHECK_CLOSE( temp.get(2), 0.534f, 0.1 );

	// Test for the vector get() method
	BOOST_CHECK(nX.get(0) == 1.0f);
	BOOST_CHECK(nZ.get(2) == 1.0f);
	BOOST_CHECK(nY.get(0) != 1.0f);

	// Test for the vector set() method
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


	// Test for the vector comparison operation
	BOOST_CHECK(nX == nX);
	BOOST_CHECK(one == one);

	// Test for the vectors sum
	BOOST_CHECK(nX + nY == Vector(1.0f, 1.0f, 0.0f));
	BOOST_CHECK(nX + nZ == Vector(1.0f, 0.0f, 1.0f));
	BOOST_CHECK(nX + nY + nZ == Vector(1.0f, 1.0f, 1.0f));

	// Test for the vectors subtraction
	BOOST_CHECK(one - nY == Vector(1.0f, 0.0f, 1.0f));
	BOOST_CHECK(one - nZ - nX == Vector(0.0f, 1.0f, 0.0f));
	BOOST_CHECK(nX - nY - nZ == Vector(1.0f, -1.0f, -1.0f));

	// Test for the product between vectors and scalars
	BOOST_CHECK(nX * 3.0f == Vector(3.0f, 0.0f, 0.0f));
	BOOST_CHECK(one * -5.0f == Vector(-5.0f, -5.0f, -5.0f));

	// Test for the vector scalar product
	BOOST_CHECK(nX * nY == 0);
	BOOST_CHECK(nX * nX == 1);

	// Test for the vector cross product
	BOOST_CHECK(nX % nY == nZ);
	BOOST_CHECK(one % nY == Vector(-1.0f, 0.0f, 1.0f));

	// Test for the vector [] operator
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


	// Test for the points subtraction
	BOOST_CHECK(p123 - p1 == test1);
	BOOST_CHECK(p1 - p1 == zero);

	// Test for the points and vectors sum
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

	// Test for the quaternion getMagnitude() method
	BOOST_CHECK(q1.getMagnitude() == 1);

	// Test for the quaternion normalize() method
	Quaternion qTest = Quaternion();
	qTest.setX(12);
	qTest.setY(8);
	qTest.setZ(1);
	qTest.setW(459.0f);
	BOOST_CHECK(qTest.getMagnitude() > 1);
	BOOST_CHECK(qTest.normalize().getMagnitude() == 1);


	// Test for the quaternion getVector() method
	Vector test1 = q1.getVector();
	BOOST_CHECK_CLOSE( test1.get(0), 0.4999f, 0.1 );
	BOOST_CHECK_CLOSE( test1.get(1), 0.0f, 0.1 );
	BOOST_CHECK_CLOSE( test1.get(2), 0.0f, 0.1 );
	
	Vector test2 = q3.getVector();
	BOOST_CHECK_CLOSE( test2.get(0), 0.4999f, 0.1 );
	BOOST_CHECK_CLOSE( test2.get(1), 0.0f, 0.1 );
	BOOST_CHECK_CLOSE( test2.get(2), 0.4999f, 0.1 );

	// Test for the quaternion getW() method
	BOOST_CHECK_CLOSE( q1.getW(), 0.8660f, 0.1 );
	BOOST_CHECK_CLOSE( q3.getW(), 0.7071f, 0.1 );

	// Test for the quaternion getAxisAngle() method
	Vector * vectorTestPtr;
	vectorTestPtr = &Vector();
	float testF = 0;

	q1.getAxisAngle(vectorTestPtr, &testF);
	BOOST_CHECK(*vectorTestPtr == Vector(1.0f, 0.0f, 0.0f));
	BOOST_CHECK_CLOSE( testF, 60.0f, 0.1 );

	// Test for the quaternion setW() method
	q3.setW(12.0f);
	BOOST_CHECK(q3.getW() == 12.0f);
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

	// Test case for the quaternions and vectors multiplication
	Vector result = q3 * one;
	BOOST_CHECK_CLOSE(result.getX(), 0.5f, 0.1);
	BOOST_CHECK_CLOSE(result.getY(), 0.0f, 0.1);
	BOOST_CHECK_CLOSE(result.getZ(), 0.5f, 0.1);


}

// Test case for methods and operators of the Matrix class
BOOST_AUTO_TEST_CASE( matrix_methods )
{
	// Istantiation of various matrices for the tests
	Matrix m1 = Matrix(	0.0f,	1.0f,	2.0f,	3.0f,
						4.0f,	5.0f,	6.0f,	7.0f,
						8.0f,	9.0f,	10.0f,	11.0f,
						0.0f,	0.0f,	0.0f,	1.0f);

	Matrix m1t = Matrix(	0.0f,	4.0f,	8.0f,	0.0f,
							1.0f,	5.0f,	9.0f,	0.0f,
							2.0f,	6.0f,	10.0f,	0.0f,
							3.0f,	7.0f,	11.0f,	1.0f);

	Matrix m2 = Matrix(	0.0f,	2.0f,	4.0f,	6.0f,
						8.0f,	10.0f,	12.0f,	14.0f,
						16.0f,	18.0f,	20.0f,	22.0f,
						0.0f,	0.0f,	0.0f,	2.0f);

	Matrix manualIdentity = Matrix(	1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

	Matrix mTranslation345 = Matrix(	1.0f, 0.0f, 0.0f, 3.0f,
										0.0f, 1.0f, 0.0f, 4.0f,
										0.0f, 0.0f, 1.0f, 5.0f,
										0.0f, 0.0f, 0.0f, 1.0f);

	Matrix mScaling678 = Matrix(	6.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 7.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 8.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

	Matrix mUniformScaling9 = Matrix(	9.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 9.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 9.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f);

	Matrix mXRotationM90 = Matrix(	1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, -1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

	Matrix mYRotation45 = Matrix(	0.707107f,	0.0f, 0.707106f,	0.0f,
									0.0f,		1.0f, 0.0f,			0.0f,
									-0.707106f, 0.0f, 0.707106f,	0.0f,
									0.0f,		0.0f, 0.0f,			1.0f);

	Matrix mZRotation30 = Matrix(	0.866026f,	-0.5f,		0.0f,	0.0f,
									0.5f,		0.866026f,	0.0f,	0.0f,
									0.0f,		0.0f,		1.0f,	0.0f,
									0.0f,		0.0f,		0.0f,	1.0f);

	Matrix mAxisRotation = Matrix(	1.0f,		-0.866025f,	0.499999f,	0.0f,
									0.866025f,	0.500001f,	-0.866025f,	0.0f,
									0.0f,		0.866025f,	1.0f,		0.0f,
									0.0f,		0.0f,		0.0f,		1.0f);

	Matrix mQuaternionRotation = Matrix(	0.75f,		-0.612372f,	0.25f,		0.0f,
											0.612372f,	0.500001f,	-0.612372f,	0.0f,
											0.25f,		0.612372f,	0.75f,		0.0f,
											0.0f,		0.0f,		0.0f,		1.0f);

	Matrix mShearingMatrix = Matrix(	1.0f, 1.0f, 2.0f, 0.0f,
										3.0f, 1.0f, 4.0f, 0.0f,
										5.0f, 6.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f);
	
	// Istantiation of various points and vectors for the tests
	Vector vTwo = Vector(2.0f, 2.0f, 2.0f);
	Point pTwo = Point(2.0f, 2.0f, 2.0f);

	// Used to compare floating point values
	double epsilon;


	// Test for the compare operator between matrices
	BOOST_CHECK(m1 == m1);
	BOOST_CHECK(manualIdentity == manualIdentity);

	// Test for the generateIdentityMatrix() method
	BOOST_CHECK(manualIdentity == Matrix::generateIdentityMatrix());

	// Test for the generateTranslationMatrix() method
	BOOST_CHECK(mTranslation345 == Matrix::generateTranslationMatrix(3.0f, 4.0f, 5.0f));

	// Test for the generateScalingMatrix() method
	BOOST_CHECK(mScaling678 == Matrix::generateScalingMatrix(6.0f, 7.0f, 8.0f));

	// Test for the generateUniformScalingMatrix() method
	BOOST_CHECK(mUniformScaling9 == Matrix::generateUniformScalingMatrix(9.0f));

	// Test for the generateXRotationMatrix() method
	Matrix generatedMXRotationM90 = Matrix::generateXRotationMatrix(-90.0);

	for(unsigned short r = 0; r < 4; r++) {
		for(unsigned short c = 0; c < 4; c++) {
			epsilon = generatedMXRotationM90.get(r, c) - mXRotationM90.get(r, c);
			BOOST_CHECK_SMALL( epsilon, 0.00001 );
		}
	}

	// Test for the generateYRotationMatrix() method
	Matrix generatedMYRotation45 = Matrix::generateYRotationMatrix(45.0);

	for(unsigned short r = 0; r < 4; r++) {
		for(unsigned short c = 0; c < 4; c++) {
			epsilon = generatedMYRotation45.get(r, c) - mYRotation45.get(r, c);
			BOOST_CHECK_SMALL( epsilon, 0.00001 );
		}
	}

	// Test for the generateZRotationMatrix() method
	Matrix generatedMZRotation30 = Matrix::generateZRotationMatrix(30.0);

	for(unsigned short r = 0; r < 4; r++) {
		for(unsigned short c = 0; c < 4; c++) {
			epsilon = generatedMZRotation30.get(r, c) - mZRotation30.get(r, c);
			BOOST_CHECK_SMALL( epsilon, 0.00001 );
		}
	}

	// Test for the generateAxesRotationMatrix() method
	Matrix generatedAxisRotation = Matrix::generateAxisRotationMatrix(Vector(1.0f, 0.0f, 1.0f), 60.0f);

	for(unsigned short r = 0; r < 4; r++) {
		for(unsigned short c = 0; c < 4; c++) {
			epsilon = generatedAxisRotation.get(r, c) - mAxisRotation.get(r, c);
			BOOST_CHECK_SMALL( epsilon, 0.00001 );
		}
	}

	
	// Test for the generateQuaternionRotationMatrix() method
	Matrix generatedQuaternionRotation = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(1.0f, 0.0f, 1.0f), 60.0f));

	for(unsigned short r = 0; r < 4; r++) {
		for(unsigned short c = 0; c < 4; c++) {
			epsilon = generatedQuaternionRotation.get(r, c) - mQuaternionRotation.get(r, c);
			BOOST_CHECK_SMALL( epsilon, 0.00001 );
		}
	}
	
	// Test for the generateShearingMatrix() method
	Matrix generatedShearingMatrix = Matrix::generateShearingMatrix(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);

	for(unsigned short r = 0; r < 4; r++) {
		for(unsigned short c = 0; c < 4; c++) {
			epsilon = generatedShearingMatrix.get(r, c) - mShearingMatrix.get(r, c);
			BOOST_CHECK_SMALL( epsilon, 0.00001 );
		}
	}


	// Test for the function for returning the quaternion from a matrix
	Quaternion quaternionCheck = Quaternion(Vector(1.0f, 0.0f, 1.0f), 60.0f);
	Quaternion quateternionResult = Matrix::getQuaternion(generatedQuaternionRotation);
	BOOST_CHECK_CLOSE( quateternionResult.getX(),quaternionCheck.getX(), 0.1 );
	BOOST_CHECK_CLOSE( quateternionResult.getY(),quaternionCheck.getY(), 0.1 );
	BOOST_CHECK_CLOSE( quateternionResult.getZ(),quaternionCheck.getZ(), 0.1 );
	BOOST_CHECK_CLOSE( quateternionResult.getW(),quaternionCheck.getW(), 0.1 );


	// Test for the Matrix * Matrix operator
	BOOST_CHECK( m1 * manualIdentity == m1);

	// Test for the Matrix * scalar operator
	BOOST_CHECK( m1 * 2.0f == m2);

	// Test for the Matrix * vector/point operator
	BOOST_CHECK( mTranslation345 * vTwo == vTwo);
	BOOST_CHECK( mUniformScaling9 * vTwo == Vector(18.0f, 18.0f, 18.0f));
	BOOST_CHECK( mTranslation345 * pTwo == Point(5.0f, 6.0f, 7.0f));

	// Test for the getTranspose() method
	BOOST_CHECK( m1.getTranspose() == m1t);

	// Test for the getInverse() method
	BOOST_CHECK(Matrix::generateIdentityMatrix().getInverse() == manualIdentity);

	Matrix inverseTest = mAxisRotation * mAxisRotation.getInverse();
	for(unsigned short r = 0; r < 4; r++) {
		for(unsigned short c = 0; c < 4; c++) {
			epsilon = manualIdentity.get(r, c) - inverseTest.get(r, c);
			BOOST_CHECK_SMALL( epsilon, 0.00001 );
		}
	}

	// Test for the getInverse() method
	BOOST_CHECK(m1.getDeterminant() == 0);

	// Test for the getRowAsVector() method
	BOOST_CHECK(m1t.getRowAsVector(1) == Vector(1.0f, 5.0f, 9.0f));

	// Test for the getColumnAsVector() method
	BOOST_CHECK(m2.getColumnAsVector(0) == Vector(0.0f, 8.0f, 16.0f));

}