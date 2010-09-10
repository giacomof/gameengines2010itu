/* ********* CAMERA TEST CLASS ********** */

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include <iostream>

#include "camera.h"
#include "linearAlgebra.h" // Header File for our math library

using namespace std;
using namespace linearAlgebra;

	Vector vPosition, vView, vUp;
	float sin_table[360], cos_table[360];

	void Camera::position(	float positionX, float positionY, float positionZ,
							float viewX,	 float viewY,	  float viewZ,
							float upX,		 float upY,		  float upZ ) 
	{
		// this method places the camera somewhere, using positionX/Y/Z vector data
		// and inizializes the three foundamental vectors we are going to use
		// in the camera views and movements

		vPosition.set(0, positionX);
		vPosition.set(1, positionY);
		vPosition.set(2, positionZ);

		vView.set(0, viewX);
		vView.set(1, viewY);
		vView.set(2, viewZ);

		vUp.set(0, upX);
		vUp.set(1, upY);
		vUp.set(2, upZ);
		
	}

	void Camera::move(float speed) 
	{
	
		Vector distance = vView - vPosition; // vector that contains the information about the direction of the vector that starts in vPosition and ends in vView

		float vPosSpeed = vPosition.get(0);
		vPosSpeed += distance.get(0) * speed;
		vPosition.set(0, vPosSpeed);

		vPosSpeed = vPosition.get(2);
		vPosSpeed += distance.get(2) * speed;
		vPosition.set(2, vPosSpeed);

		float vViewSpeed = vView.get(0);
		vViewSpeed += distance.get(0) * speed;
		vView.set(0, vViewSpeed);

		vViewSpeed = vView.get(2);
		vViewSpeed += distance.get(2) * speed;
		vView.set(2, vViewSpeed);
	
	}

	void Camera::rotate(float angle) 
	{
		
		// create the rotation matrix
		Matrix rotYmatrix = Matrix::generateYRotationMatrix(angle);
		// create the rotated vector
		Vector tempVector = rotYmatrix*vView;
		// set the rotated vector as the view vector
		vView.set( 0, tempVector.get(0));
		vView.set( 1, tempVector.get(1));
		vView.set( 2, tempVector.get(2));

		

	
	}

	void Camera::doViewTransform()
	{
		glLoadIdentity();
		gluLookAt(  vPosition.get(0), vPosition.get(1), vPosition.get(2),
					vView.get(0),	  vView.get(1),	    vView.get(2),
					vUp.get(0),		  vUp.get(1),		vUp.get(2));

	}

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


