#include <list>
#include "linearAlgebraDLL.h"			// Header File for our math library

using namespace linearAlgebraDLL;


using namespace std;

class Transformation 
{
	public:
		Transformation(){};
		Transformation(	float p_tX, float p_tY, float p_tZ,
						float p_angleX, float p_angleY, float p_angleZ);
		//~Transformation();

		Matrix getTransformation(void);
		void addRotation(float p_angleX, float p_angleY, float p_angleZ);
		void addTranslation(float p_tX, float p_tY, float p_tZ);
		void addScaling(float p_sX, float p_sY, float p_sZ);
		void addShearing(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy);

	private:
		Matrix transformationMatrix;
		float angleX, angleY, angleZ;
		float tX, tY, tZ;
		float sX, sY, sZ;
		float sxy, sxz, syx, syz, szx, szy;

};

