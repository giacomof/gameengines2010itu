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
		//~Transformation();

		// Return the transpose of the transformation matrix
		Matrix getTransformation(void);
		// Return the transpose of the inverse transformation matrix
		Matrix getInverseTransformation(void);

		void addQuaternionRotation(Quaternion q);
		void addTranslation(Vector v);
		void addScaling(float p_sX, float p_sY, float p_sZ);
		void addShearing(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy);

		Vector getTranslation();
		Quaternion getOrientationQuaternion();

		/*
		float getTranslation();
		float getOrientationVector();
		float getOrientationAngle();
		float getScaling();
		float getShearing();
		*/
		

	private:
		Matrix transformationMatrix;					// Cointains the actual transformation matrix

		// Values of the transformation contained inside the transformation matrix
		Quaternion rotation;
		Vector translation;
		float sX, sY, sZ;
		float shXY, shXZ, shYX, shYZ, shZX, shZY;
};

