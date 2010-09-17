#include <string>
#include <list>

#include <stdlib.h>
#include <SDL.h>						// Header File for the SDL library
#include <SDL_opengl.h>					// Header File for OpenGL through SDL
#include <SDL_thread.h>

#include "sceneNode.h"

static int nodeCount=0;


SceneNode::SceneNode(	SceneNode * parentNode, string str, 
						float p_tX,		float p_tY,		float p_tZ,
						float p_angleX, float p_angleY, float p_angleZ ) 
{
	// variables inizialization
	nodeName = str;
	parentNode = parentNode;
	tX		= p_tX;
	tY		= p_tY;
	tZ		= p_tZ;
	angleX	= p_angleX;
	angleY	= p_angleY;
	angleZ	= p_angleZ;
	sX = sY = sZ = 1;
	sxy = sxz = syx = syz = szx = szy = 0;

	// ********************* TEST ************************
	transformationMatrix = Matrix::generateIdentityMatrix();

	// set parent node
	parentNode->addChild(*this);

	// set the position of the SceneNnode
	translate( tX, tY, tZ );
	// and the orientation
	rotate( angleX, angleY, angleZ );

	// sets the unique id of the sceneNode
	id = nodeCount;
	nodeCount++;
	
}

void SceneNode::update(SceneNode * parentNode, string str, 
						float p_tX,		float p_tY,		float p_tZ,
						float p_angleX, float p_angleY, float p_angleZ) 
{
	// set parent node
	parentNode->addChild(*this);

	// set the position of the SceneNnode
	translate( tX, tY, tZ );
	// and the orientation
	rotate( angleX, angleY, angleZ );

	// sets the unique id of the sceneNode
	id = nodeCount;
	nodeCount++;

}

void SceneNode::destroy(void) 
{
		
	list<SceneNode>::iterator i;
	for(i=childList.begin(); i != childList.end(); ++i) 
	{ 
		i->destroy(); 
	}
		
	this->release();

}

	
void SceneNode::addChild( SceneNode & cNode ) 
{
	cNode.setParent(*this);
	childList.push_back(cNode);
}
		
	
void SceneNode::detachChild( SceneNode & cNode ) 
{
	// setParent Root
	// delete the SceneNode from the childList
}

void SceneNode::setParent( SceneNode & cNode ) 
{
	parentNode = &cNode;
}

SceneNode* SceneNode::getParent(void)
{
	return parentNode;
}

void SceneNode::setName(string name) 
{
	nodeName = name;
}

string SceneNode::getName(void) 
{
	return nodeName;
}

void SceneNode::rotate(float p_angleX, float p_angleY, float p_angleZ)
{
	angleX += p_angleX;
	angleY += p_angleY;
	angleZ += p_angleZ;
	
	//transformationMatrix = Matrix::generateXRotationMatrix(angleX) * transformationMatrix;
	//transformationMatrix = Matrix::generateYRotationMatrix(angleY) * transformationMatrix;
	//transformationMatrix = Matrix::generateZRotationMatrix(angleZ) * transformationMatrix;
}

void SceneNode::translate(float p_tX, float p_tY, float p_tZ) 
{	
	tX += p_tX;
	tY += p_tY;
	tZ += p_tZ;

	//transformationMatrix = transformationMatrix * Matrix::generateTranslationMatrix(tX, tY, tZ);
}

void SceneNode::scale(float p_sX, float p_sY, float p_sZ)
{
	sX = p_sX;
	sY = p_sY;
	sZ = p_sZ;

	//transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ) * transformationMatrix;
}

void SceneNode::shear(float p_sxy, float p_sxz, float p_syx, float p_syz, float p_szx, float p_szy)
{
	sxy += p_sxy;
	sxz += p_sxz;
	syx += p_syx;
	syz += p_syz;
	szx += p_szx;
	szy += p_szy;

	//transformationMatrix = Matrix::generateShearingMatrix(sxy, sxz, syx, syz, szx, szy) * transformationMatrix;
}

Matrix SceneNode::getTransformation() 
{
	return transformationMatrix;
}

void SceneNode::drawGeometry()
{
	applyTransformation();

	float y = -10.0f;
	glColor3f(0.0f, 1.0f, 0.0f);
	float red, green, blue = 0;
	for(float z = -50; z <= 50; z += 1) {
		green = 0.01 * (50-z);
		for(float x = -50; x <= 50; x += 1) {
			blue = red = 0.01 * (50-x);
			glColor3f(red, green, blue);
			glBegin(GL_TRIANGLES);
				glVertex3f(x+1, y, z);
				glVertex3f(x, y, z+1);
				glVertex3f(x+1, y, z+1);
			glEnd();
		}
    }

	list<SceneNode>::iterator itS;
	for(itS = childList.begin(); itS != childList.end(); ++ itS) {
			itS->drawGeometry();
	}
	
	glPopMatrix();
}

void SceneNode::applyTransformation()
{
	/*
	glPushMatrix();
	float tranM[16];
	list<SceneNode>::iterator itS;

	//glLoadIdentity();
	//transformationMatrix = getTransformation();
	
	transformationMatrix.getMatrix(&tranM[0]);
	glMultMatrixf(&tranM[0]);
	glPopMatrix();

	for(itS = childList.begin(); itS != childList.end(); ++ itS) {
			itS->drawGeometry();
	}
	*/
	float tranM[16];
	Matrix transformationMatrix = Matrix::generateTranslationMatrix(tX, tY, tZ).getTranspose();
	transformationMatrix = Matrix::generateXRotationMatrix(angleX).getTranspose() * transformationMatrix;
	transformationMatrix = Matrix::generateYRotationMatrix(angleY).getTranspose() * transformationMatrix;
	transformationMatrix = Matrix::generateZRotationMatrix(angleZ).getTranspose() * transformationMatrix;
	transformationMatrix = Matrix::generateScalingMatrix(sX, sY, sZ).getTranspose() * transformationMatrix;
	transformationMatrix = Matrix::generateShearingMatrix(sxy, sxz, syx, syz, szx, szy).getTranspose() * transformationMatrix;

	transformationMatrix.getMatrix(&tranM[0]);

	glMultMatrixf(&tranM[0]);

}

Root::Root()
{
	nodeName = "Root";
	id = nodeCount;
	nodeCount++;
	//transformationMatrix = Matrix::generateIdentityMatrix();
	/*
	tX, tY, tZ	= 0;
	angleX, angleY, angleZ	= 0;
	sX = sY = sZ = 1;
	sxy = sxz = syx = syz = szx = szy = 0;
	*/
}


Root::~Root(void)
{
}

void Root::drawGeometry()
{
	list<SceneNode>::iterator itS;

	for(itS = childList.begin(); itS != childList.end(); ++ itS) {
			itS->drawGeometry();
	}
}