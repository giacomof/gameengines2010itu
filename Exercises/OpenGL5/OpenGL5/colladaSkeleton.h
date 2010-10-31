#pragma once

#ifndef colladaSkeleton__H__
#define colladaSkeleton__H__


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

	// not done yet.
	void render(void) const;

	// returns the total size of the skeleton in bytes
	unsigned int getDataSize() const;

	// *****************************************************
	// ******************* Variables ***********************
	// *****************************************************

	// character type defaults to char
	xml_document<> doc; 
	xml_node<>* tempNode;
};

#endif