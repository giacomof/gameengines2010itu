#include <iostream>
#include "mathlib.h"

using namespace std;

int main(int argc, char **argv)
{
	float matrix_array[3][4];
    cout << "Please insert your matrix separating each member with a return\n";

		for(int column=0; column<3; column++) {
			// cycle to read the matrix
			// fill in 4 rows, 3 column
			for(int row=0; row<4; row++) {

				// write what are you going to ask the person to insert
				cout << "insert the value of column " << column+1 << " and row " << row+1 << ": ";
				cin >> matrix_array[column][row];
				
			}
		
		}

		matrix * testMatrix = new matrix(&matrix_array[0][0]);

		float *primo = testMatrix->getMatrix();		
		for (int i = 0; i < 12; i++)
			cout << (*primo++) << endl;

		system("PAUSE");
    return 0;
}