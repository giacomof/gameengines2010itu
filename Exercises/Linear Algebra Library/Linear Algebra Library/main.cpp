#include <iostream>
#include "linearAlgebra.h"
using namespace std;
using namespace linearAlgebra;


int main()
{

	Vector testVector(7,8,9);
	Point testPoint(3,3,3);
	cout << "12 * testVector = " << testVector * 12 << endl;

    cout << "-------------------------------------" << endl;
    Matrix m(1, 0, 0, 1,
             0, 1, 0, 2,
             0, 0, 1, 3,
             0, 0, 0, 1);

    Matrix t(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);

	cout << "Multiplication of matrix and vector m * testVector = " << m * testVector << endl; 

    system("Pause");
    return 0;
}
