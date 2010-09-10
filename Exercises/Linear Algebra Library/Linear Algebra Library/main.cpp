#include <iostream>
#include "linearAlgebra.h"
using namespace std;
using namespace linearAlgebra;


int main()
{

	Vector testVector(7,8,9);
	Point testPoint(7,8,9);

	
	
    Matrix m(2, 3, 4, 5,
             0, -1, 2, 1,
             0, 0, 2, 4,
             0, 3, -6, 0);

    Matrix t(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);

	Matrix prova = Matrix::generateXRotationMatrix(90);

	cout << " m  = " << m << endl; 
    cout << "-------------------------------------" << endl;
	cout << " Det(m)  = " << m.getDeterminant() << endl; 
	cout << "-------------------------------------" << endl;
	cout << " mT  = " << m.getTranspose() << endl; 
	cout << "-------------------------------------" << endl;
	cout << " m-1  = " << m.getInverse() << endl;
	cout << "-------------------------------------" << endl;
	cout << " m*m-1  = " << m.getInverse()*m << endl;
	cout << "-------------------------------------" << endl;
	cout << "-------------------------------------" << endl;
	cout << "-------------------------------------" << endl;
	cout << " rotationMatrix X  = " << Matrix::generateXRotationMatrix(90) << endl; 

    system("Pause");
    return 0;
}
