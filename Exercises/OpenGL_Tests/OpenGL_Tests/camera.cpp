/* ********* CAMERA TEST CLASS ********** */

/* It is needed the vector class that giacomo is preparing */


#include <iostream>

#include "camera.h"

using namespace std;

class Camera {

private:
	

	void Camera::positionposition(	float positionX, float positionY, float positionZ,
									float viewX,	 float viewY,	  float viewZ,
									float upX,		 float upY,		  float upZ ) 
	{
		// this method places the camera somewhere, using positionX/Y/Z vector data
		// and inizializes the three foundamental vectors we are going to use
		// in the camera views and movements

		Vector vPosition(positionX, positionY, positionZ);
		Vector vView(viewX, viewY, viewZ);
		Vector vUp(upX, upY, upZ);
	}

	void Camera::move(float speed) 
	{
	
		Vector distance = vView - vPosition; // vector that contains the information about the direction of the vector that starts in vPosition and ends in vView

		vPosition.x += distance.x * speed;
		vPosition.z += distance.z * speed;
		vView.x += distance.x * speed;
		vView.z += distance.z * speed;
	
	}

	void Camera::rotate(int rotX, int rotY, int rotZ) 
	{
		Vector distance = vView - vPosition; // vector that contains the information about the direction of the vector that starts in vPosition and ends in vView
		
		if(rotX) {
			vView.z = vPosition.z + sin_table[rotX]*distance.y + cos_table[rotX]*distance.z;
			vView.y = vPosition.y + sin_table[rotX]*distance.y + cos_table[rotX]*distance.z;
		}

		if(rotY) {
			vView.z = vPosition.z + sin_table[rotY]*distance.x + cos_table[rotY]*distance.z;
			vView.x = vPosition.x + sin_table[rotY]*distance.x + cos_table[rotY]*distance.z;
		}
	
		if(rotZ) {
			vView.x = vPosition.x + sin_table[rotZ]*distance.y + cos_table[rotZ]*distance.x;
			vView.y = vPosition.y + sin_table[rotZ]*distance.y + cos_table[rotZ]*distance.x;
		}

	}

	void Camera::doViewTransform()
	{
		gluLookAt(  vPosition.x, vPosition.y, vPosition.z,
					vView.x,	 vView.y,	  vView.z,
					vUp.x,		 vUp.y,		  vUp.z);

	}

public:


	// Build the sin and cos look up tables to get 
	// faster access to these values at runtime
	void BuilLookUpTables() 
	{

	for(int angle=0; angle<360; angle++) {
		sin_table[angle] = (float) sin((double)angle*PI/180);
		cos_table[angle] = (float) cos((double)angle*PI/180);
		}
	}






/* ************************************************* */
/* ************ THIS IS OLD CODE ******************* */
/* ************************************************* */

/*	void Camera :: rotateY(float amount)
	{
		Vector target = m_target;
		Vector right = m_right;

		amount /= 57.2957795f;

		m_target.m_xyzw[0] = (cos(1.5708f + amount) * target.m_xyzw[0]) + (cos(amount) * right.m_xyzw[0]);

		m_target.m_xyzw[1] = (cos(1.5708f + amount) * target.m_xyzw[1]) + (cos(amount) * right.m_xyzw[1]);

		m_target.m_xyzw[2] = (cos(1.5708f + amount) * target.m_xyzw[2]) + (cos(amount) * right.m_xyzw[2]);

		m_right.m_xyzw[0]  = (cos(amount) * target.m_xyzw[0]) + (cos(1.5708f - amount) * right.m_xyzw[0]);

		m_right.m_xyzw[1]  = (cos(amount) * target.m_xyzw[1]) + (cos(1.5708f - amount) * right.m_xyzw[1]);

		m_right.m_xyzw[2]  = (cos(amount) * target.m_xyzw[2]) + (cos(1.5708f - amount) * right.m_xyzw[2]);

		m_target.normalize();
		m_right.normalize();
	}

	void Camera :: lookAt(Vector target)
	{
		Vector projectedTarget;
	
		target = target - m_position;
		projectedTarget = target;
        
		if(fabs(target.m_xyzw[0]) < 0.00001f && fabs(target.m_xyzw[2]) < 0.00001f) {	// YZ plane
    
			projectedTarget.m_xyzw[0] = 0.0f;
			projectedTarget.normalize();
        
			m_right = Vector(1.0f, 0.0f, 0.0f);
			m_up = cross(projectedTarget, m_right);
            
			m_target = target;
			m_right = -cross(m_target, m_up);
		}
        
		else {										// XZ plane
            
			projectedTarget.m_xyzw[1] = 0.0f;
			projectedTarget.normalize();
        
			m_up = Vector(0.0f, 1.0f, 0.0f);
			m_right = -cross(projectedTarget, m_up);
	
			m_target = target;
			m_up = cross(m_target, m_right);
		}
	
		m_target.normalize();
		m_right.normalize();
		m_up.normalize();
	}
*/

};
