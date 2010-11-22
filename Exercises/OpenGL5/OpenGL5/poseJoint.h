#pragma once

#ifndef poseJoint__H__
#define poseJoint__H__

#include <vector>
#include "linearAlgebraDLL.h"

using namespace std;
using namespace linearAlgebraDLL;

struct poseJoint
{
	Matrix jointInversePose;
	int jointIndex;
	const char * jointID;
	vector<poseJoint *> jointChildren;
};

#endif