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

struct poseJoint
{
	Matrix jointTransform;
	int jointIndex;
	const char * jointID;
	vector<poseJoint *> jointChildren;
};

struct JointKeyframeChannel
{
	int jkKeyframes;
	float * jkTime;
	float * jkValues;
};

struct Joint
{
	Matrix jBindPose;						// the bind pose of this joint
	const char * jName;						// joint name
	const char * jID;						// joint ID, if any
	int jParentIndex;						// the index of the parent
	bool jAnimated;							// Does this joint have animation data
	JointKeyframeChannel jChannel[4][4];	// Animation data for this joint
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
	poseJoint * buildSkeleton();

	// takes a poseJoint representing the root of a built skeleton, and an animation delta and updates the current animation (more parameters later)
	void updateSkeleton(poseJoint * currentJoint, float timeCurrent);

	// takes a poseJoint and traces the skeleton on the screen
	void traceSkeletonJoint(poseJoint * rootJoint);

private:
	void parseChildJoint(xml_node<>* currentNode, int parentIndex);
	void buildSkeletonJoint(poseJoint * currentJoint, int currentIndex);

	vector<Joint> JointArray;
};

#endif