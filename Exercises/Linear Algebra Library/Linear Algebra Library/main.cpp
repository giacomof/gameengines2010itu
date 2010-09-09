#include <iostream>
#include "linearAlgebra.h"
using namespace std;
using namespace linearAlgebra;


int main()
{

	Vector testVector(7,8,9);
	Point testPoint(7,8,9);

	
	
    Matrix m(1, 0, 0, 1,
             0, 1, 0, 2,
             0, 0, 1, 3,
             0, 0, 0, 1);

    Matrix t(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);

	Matrix prova = Matrix::generateXRotationMatrix(90);

	cout << " prova  = " << prova << endl; 
    cout << "-------------------------------------" << endl;
	cout << " provaT  = " << prova.getTranspose() << endl; 


    system("Pause");
    return 0;
}
