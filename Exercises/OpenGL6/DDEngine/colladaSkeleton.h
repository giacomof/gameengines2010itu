#ifdef __DDEngine
#    define colladaSkeleton_D __declspec(dllexport)
#else
#    define colladaSkeleton_D __declspec(dllimport)
#endif

#ifndef colladaSkeleton__H
#define colladaSkeleton__H

#include "rapidxml.hpp"
#include "linearAlgebraDLL.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glut.h>

using namespace std;
using namespace rapidxml;
using namespace linearAlgebraDLL;

struct stringContainer
{
	string BoneID;
};

struct matrixContainer
{
	Matrix InvMatrix;
};

struct skelPoseJoint
{
	string pjID;

	Matrix pjJointTransform;
	Matrix pjTempTransform;
};

struct skelPose
{
	vector<skelPoseJoint> skelPoseJoints;
};

struct skelJoint
{
	string jName;								// Joint name
	string jID;									// Joint ID, if any

	int jParentIndex;							// The index of the parent

	Matrix jBindPose;							// The bind pose matrix of this joint
	vector<float> jKeyframeTime;				// Keyframe times for this joint
	vector<matrixContainer> jKeyframeMatrix;	// Keyframe matrices for this joint
};

class colladaSkeleton_D ColladaSkeleton
{
public:

	// Constructor
	ColladaSkeleton(void);
	// Destructor
	~ColladaSkeleton(void);

	// load the skeleton
	bool load(std::string & str);

	// returns the total size of the skeleton in bytes
	unsigned int getDataSize();

	// builds a skeleton of poseJoints and returns pointer to the root
	skelPose * buildSkeleton();

	// takes a poseJoint representing the root of a built skeleton, and an animation delta and updates the current animation (more parameters later)
	void updateSkeleton(skelPose * currentPose, float currentTime);

	// trace the skeletal pose
	void traceSkeleton(skelPose * currentPose);

private:
	// Variables
	bool skelAnimated;				// Is there any animation at all for this skeleton?
	vector<skelJoint> skelJoints;	// Array of the skeleton joins in the bind pose.

	void loadSkelJoint(xml_node<>* currentNode, int parentIndex);
};

#endif