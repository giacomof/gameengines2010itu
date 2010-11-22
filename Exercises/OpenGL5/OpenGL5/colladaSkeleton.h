#pragma once

#ifndef colladaSkeleton__H__
#define colladaSkeleton__H__

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glut.h>
#include "rapidxml.hpp"
#include "poseJoint.h"

#include "linearAlgebraDLL.h"

using namespace std;
using namespace rapidxml;
using namespace linearAlgebraDLL;

struct JointAnim
{
	int ArraySize;
	float * inputArray;
	float * outputArray;
	int * interpolationArray;
};

struct Joint
{
	Matrix inversePose;						// the inverse of the joint pose
	const char * jName;						// joint name
	const char * jBoneID;					// joint bone ID, if any
	int jParentIndex;						// the index of the parent
	bool jAnimated;							// Is this animated?
	JointAnim ChannelMatrix[4][4];			// Animation data for this joint
};

class ColladaSkeleton
{
public:

	// Constructor
	ColladaSkeleton(void);
	// Destructor
	~ColladaSkeleton(void);

	// load the skeleton
	bool load(std::string & str);

	// returns number of joints (for anticipating array sizes)
	unsigned int getJointCount();

	// returns the total size of the skeleton in bytes
	unsigned int getDataSize();

	// builds a skeleton of poseJoints and returns pointer to the root
	poseJoint * buildSkeleton();

	// takes a poseJoint representing the root of a built skeleton, and an animation delta and updates the current animation (more parameters later)
	void updateSkeleton(poseJoint * rootJoint);

	// takes a poseJoint and traces the skeleton on the screen
	void traceSkeletonJoint(poseJoint * rootJoint);

private:
	void parseChildJoint(xml_node<>* currentNode, int parentIndex);
	void buildChildJoint(poseJoint * currentJoint, int currentIndex);

	vector<Joint> JointArray;
};

#endif