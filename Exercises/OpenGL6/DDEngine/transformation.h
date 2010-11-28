#ifdef __DDEngine
#    define transformation_D __declspec(dllexport)
#else
#    define transformation_D __declspec(dllimport)
#endif

#ifndef transformation__H
#define transformation__H

#include <list>
#include "linearAlgebra.h"

#pragma warning(disable: 4251)


using namespace std;

class transformation_D Transformation 
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

		Vector getBBTranslation(void);
		Quaternion getBBOrientation(void);
		
		void setTranslation(Vector t);
		void setOrientation(Quaternion q);

		void setBBTranslation(Vector t);
		void setBBOrientation(Quaternion q);
		

	private:
		// Cointains the actual transformation matrix
		Matrix transformationMatrix;					

		// Values of the transformation used to calculate the transformation matrix
		Quaternion rotation;
		Vector translation;
		float sX, sY, sZ;
		float shXY, shXZ, shYX, shYZ, shZX, shZY;

		Vector bbTranslation;
		Quaternion bbRotation;

		void calculateMatrix();
};

#endif