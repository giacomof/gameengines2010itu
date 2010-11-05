#pragma once

#ifndef colladaSkeleton__H__
#define colladaSkeleton__H__

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glut.h>
#include "rapidxml.hpp"

#include "linearAlgebraDLL.h"

using namespace std;
using namespace rapidxml;
using namespace linearAlgebraDLL;

// library_visual_scenes - skeletal heirachy
// library_controllers - joint-vertex associations and weights
// library_animations - animations

struct Joint
{
	Matrix inversePose;			// the inverse of the joint pose
	const char * jName;			// joint name
	const char * jBoneID;		// joint bone ID, if any
	int jParentIndex;			// the index of the parent
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

private:
	void parseChildJoint(xml_node<>* currentNode, int parentIndex);

	vector<Joint> JointArray;
};

#endif