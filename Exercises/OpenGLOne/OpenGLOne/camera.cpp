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
		
		// computate the distance vector starting from the origin
		// to the camera position vector

		Vector origin = Vector(0.0f, 0.0f, 0.0f); 
		Vector distance = origin - vPosition;
		Vector yAxes = Vector (0.0f, 1.0f, 0.0f);

		Matrix yAxesTranslationMatrix = Matrix::generateTranslationMatrix(distance.get(0), distance.get(1), distance.get(2)); 

		yAxes = yAxesTranslationMatrix * yAxes;





		// *******************************************
		// ************* TEST METHOD *****************
		// ******* DON'T TRY THIS AT HOME, KIDS ******
		// *******************************************

		//// creating the origin vector
		//Vector origin = Vector(0.0f, 0.0f, 0.0f); 
		//// compute the distance between the origin and my view vector
		//Vector distance = vView - origin;
		//// traslating my view vector to the origin
		//// creating the traslation matrix
		//Matrix cameraTranslationMatrix = Matrix::generateTranslationMatrix(distance.get(0), distance.get(1), distance.get(2)); 
		//// make it happen
		//Vector originCamera = cameraTranslationMatrix * vView;
		//// create the rotation matrix
		//Matrix rotYmatrix = Matrix::generateYRotationMatrix(angle);
		//// create the rotated vector
		//Vector rotatedViewVector = rotYmatrix*originCamera;
		//// traslate the rotated vector back to the position
		//distance =  origin - vView;
		//// creating the traslation matrix
		//cameraTranslationMatrix = Matrix::generateTranslationMatrix(distance.get(0), distance.get(1), distance.get(2)); 
		//// make it happen
		//Vector rotatedTraslatedViewVector = cameraTranslationMatrix * rotatedViewVector;

		//// set the rotated vector as the view vector
		//vView.set( 0, rotatedTraslatedViewVector.get(0));
		//vView.set( 1, rotatedTraslatedViewVector.get(1));
		//vView.set( 2, rotatedTraslatedViewVector.get(2));
		


		// *******************************************
		// **************** OLD CODE *****************
		// *******************************************

		//// create the rotation matrix
		//Matrix rotYmatrix = Matrix::generateYRotationMatrix(angle);
		//// create the rotated vector
		//Vector rotatedViewVector = rotYmatrix*vView;
		//// set the rotated vector as the view vector
		//vView.set( 0, rotatedViewVector.get(0));
		//vView.set( 1, rotatedViewVector.get(1));
		//vView.set( 2, rotatedViewVector.get(2));

		//Vector rotatedPositionVector = rotYmatrix*vPosition;
		//// set the rotated vector as the position vector
		//vPosition.set( 0, rotatedPositionVector.get(0));
		//vPosition.set( 1, rotatedPositionVector.get(1));
		//vPosition.set( 2, rotatedPositionVector.get(2));

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
