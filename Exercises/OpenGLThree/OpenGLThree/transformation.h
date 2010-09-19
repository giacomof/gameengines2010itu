#include <list>
#include "linearAlgebraDLL.h"			// Header File for our math library

using namespace linearAlgebraDLL;


using namespace std;

class Transformation 
{
	public:
		Transformation() { };
		Transformation(	float p_tX, float p_tY, float p_tZ,
						Vector p_axis, float p_angle );
		//~Transformation();

		// Return the transpose of the transformation matrix
		Matrix getTransformation(void);
		Matrix getInverseTransformation(void);
		//void addRotation(float p_angleX, float p_angleY, float p_angleZ);
		void addQuaternionRotation(Quaternion q);
		void addAxisRotation(Vector p_Axis, float p_Degree);
		void addTranslation(float p_tX, float p_tY, float p_tZ);
		void addScaling(float p_sX, float p_sY, float p_sZ);
		void addShearing(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy);

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
		//float angleX, angleY, angleZ;
		Quaternion rotation;
		Vector axis;
		float degree;
		float tX, tY, tZ;
		float sX, sY, sZ;
		float shXY, shXZ, shYX, shYZ, shZX, shZY;
};

