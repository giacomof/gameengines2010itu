#pragma once

#ifndef poseJoint__H__
#define poseJoint__H__

#include <vector>
#include "linearAlgebraDLL.h"

using namespace std;
using namespace linearAlgebraDLL;

struct poseJoint
{
	Matrix jointTransform;
	int jointIndex;
	const char * jointID;
	const char * jointName;
	vector<poseJoint *> jointChildren;
};

#endif