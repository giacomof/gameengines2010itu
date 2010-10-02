#include <list>
#include "linearAlgebraDLL.h"			// Header File for our math library

using namespace linearAlgebraDLL;


using namespace std;

class Transformation 
{
	public:
		Transformation() { };
		Transformation(	Vector p_translation,
						Vector p_axis, float p_angle );

		// Return the transpose of the transformation matrix
		Matrix getTransformation(void);
		// Return the transpose of the inverse transformation matrix
		Matrix getInverseTransformation(void);

		// Functions for adding transformations
		void addQuaternionRotation(Quaternion q);
		void addTranslation(Vector v);
		void addScaling(float p_sX, float p_sY, float p_sZ);
		void addShearing(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy);

		// Functions for getting values of the actual transformation
		Vector getTranslation();
		Quaternion getOrientation();
		void getScaling(float * scaleValuesArray);
		void getShearing(float * shearValuesArray);

		void setTranslation(Vector t);
		void setOrientation(Quaternion q);
		

	private:
		Matrix transformationMatrix;					// Cointains the actual transformation matrix

		// Values of the transformation used to calculate the transformation matrix
		Quaternion rotation;
		Vector translation;
		float sX, sY, sZ;
		float shXY, shXZ, shYX, shYZ, shZX, shZY;
};

