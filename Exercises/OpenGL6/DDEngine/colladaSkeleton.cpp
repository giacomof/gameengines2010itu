#include "colladaSkeleton.h"

ColladaSkeleton::ColladaSkeleton(void)
{

}

ColladaSkeleton::~ColladaSkeleton(void)
{

}

bool ColladaSkeleton::load(std::string & str)
{
	// Clear the array for loading
	skelJoints.clear();

	// Variables to parse xml doc with
	xml_document<> doc;
	xml_node<> * rootNode, * libraryNode;

	// Try to open the xml file
	doc.parse<0>(strdup(str.c_str()));    // 0 means default parse flags

	// Check if the file opened succesfully
	if (doc.first_node("COLLADA") != 0)
	{
		rootNode = doc.first_node("COLLADA");
	}
	else
	{
		std::cout << "colladaSkeleton - Could not open xml file!\n";
		return false;
	}

	// Check if there is a skeleton library in the file
	if (rootNode->first_node("library_visual_scenes") != 0)
	{
		libraryNode = rootNode->first_node("library_visual_scenes")->first_node("visual_scene");

		// We need to find the name of the root joint node first
		string skelRootName;
		xml_node<>* skelRootNode;


		// The name is in a subnode of the instance_controller node. We look for that
		bool nodeFound = false;
		xml_node<>* currentNode = libraryNode->first_node("node");
		while (!nodeFound)
		{
			if (currentNode->first_node("instance_controller") != 0)
			{
				skelRootName = currentNode->first_node("instance_controller")->first_node("skeleton")->value();
				nodeFound = true;
			}
			
			// Continue to next node if we haven't found it yet
			if ( !nodeFound && currentNode->next_sibling() != 0)
			{
				currentNode = currentNode->next_sibling();
			}
			else if ( !nodeFound )
			{
				std::cout << "colladaSkeleton - Could not identify root joint!\n";
				return false;
			}
		}

		// Now we need to find the node matching the name we've found
		nodeFound = false;
		currentNode = libraryNode->first_node("node");
		while (!nodeFound)
		{
			if ( currentNode->first_node("node") != 0 )
			{
				if ( currentNode->first_node("node")->first_attribute("id") != 0 )
				{
					string tempString = currentNode->first_node("node")->first_attribute("id")->value();
					if ( skelRootName.compare(1, skelRootName.size() - 1, tempString) == 0)
					{
						skelRootNode = currentNode->first_node("node");
						nodeFound = true;
					}
				}
			}
			
			// Continue to next node if we haven't found it yet
			if ( !nodeFound && currentNode->next_sibling() != 0)
			{
				currentNode = currentNode->next_sibling();
			}
			else if ( !nodeFound )
			{
				std::cout << "colladaSkeleton - Could not find skeleton root node!\n";
				return false;
			}
		}

		// We have the root skeleton node now. We can now parse the tree and load it into our vector
		loadSkelJoint(skelRootNode, -1);
	}
	else
	{
		std::cout << "colladaSkeleton - Could not find library_visual_scenes node!\n";
		return false;
	}

	// Check if there is an animation library in the file
	if (rootNode->first_node("library_animations") != 0)
	{
		libraryNode = rootNode->first_node("library_animations");

		// Iterate through the joints we have loaded and load animations for each if possible
		for (int i=0; i<skelJoints.size(); i++)
		{
			// First we need to find the animation node match our current joint
			xml_node<>* currentAnimationNode = libraryNode->first_node("animation");

			// Get the current joint's ID.
			string currentJointName = skelJoints[i].jName;

			// Search for a joint animation matching this joint ID
			bool matchFound = false;
			while (!matchFound)
			{
				string tempJointName = currentAnimationNode->first_attribute("id")->value();

				// this assumes tempJointID is jointID with "-transform" appended. We could make this more robust I guess
				if (tempJointName.compare(0, tempJointName.size() - 10, currentJointName) == 0)
				{
					matchFound = true;
				}

				if ( !matchFound && currentAnimationNode->next_sibling("animation") != 0)
				{
					currentAnimationNode = currentAnimationNode->next_sibling("animation");
				}
				else if ( !matchFound )
				{
					break;
				}
			}

			// Only continue if we found the match
			if (matchFound)
			{
				// Prepare variables for storing data
				unsigned int currentCount;
				string currentTimeArray;
				string currentMatrixArray;

				// Get the time array
				xml_node<>* currentSource = currentAnimationNode->first_node("source");
				currentTimeArray = currentSource->first_node("float_array")->value();
				currentCount = atoi(currentSource->first_node("float_array")->first_attribute("count")->value());

				// Utility variable
				string token;

				// Put in the time data
				skelJoints[i].jKeyframeTime.reserve(currentCount);
				std::istringstream streamTimeArray(currentTimeArray);
				while ( getline(streamTimeArray, token, ' ') )
				{
					skelJoints[i].jKeyframeTime.push_back( atof(token.c_str()) );
				}

				// Generate empty matrices
				skelJoints[i].jKeyframeMatrix.resize(skelJoints[i].jKeyframeTime.size());

				// Go down through the matrix channels and put them into the vector
				currentSource = currentSource->next_sibling("source");
				for (int channel=0; channel<16; channel++)
				{
					currentMatrixArray = currentSource->first_node("float_array")->value();
					currentCount = atoi(currentSource->first_node("float_array")->first_attribute("count")->value());

					int row = (int) (channel % 4);
					int col = (int) (channel / 4);

					// Last channel is special case. we want to make sure it's 1
					if (channel == 15)
					{
						for (int j=0; j<skelJoints[i].jKeyframeMatrix.size(); j++)
						{
							skelJoints[i].jKeyframeMatrix[j].InvMatrix.set( row, col, 1 );
						}
					}
					// If matrix channel doesnt have a value for each frame, assume the first value is constant
					else if (currentCount != skelJoints[i].jKeyframeTime.size())
					{
						std::istringstream streamMatrixArray(currentMatrixArray);
						getline(streamMatrixArray, token, ' ');
						float value = atof(token.c_str());
						for (int j=0;j<skelJoints[i].jKeyframeTime.size();j++)
						{
							skelJoints[i].jKeyframeMatrix[j].InvMatrix.set( row, col, value);
						}
					}
					else
					{
						std::istringstream streamMatrixArray(currentMatrixArray);
						int j = 0;
						while ( getline(streamMatrixArray, token, ' ') )
						{
							skelJoints[i].jKeyframeMatrix[j].InvMatrix.set( row, col, atof(token.c_str()) );
							j++;
						}
					}

					if (channel < 15)
					{
						currentSource = currentSource->next_sibling("source")->next_sibling("source")->next_sibling("source");
					}
				}


			}

			// Animation Node has been loaded at this point
		}

		// All joints have had animations loaded at this point
	}
	else
	{
		std::cout << "colladaSkeleton - Could not find library_animations node!\n";
	}

	return true;
}

void ColladaSkeleton::loadSkelJoint(xml_node<>* currentNode, int parentIndex)
{
	// Make the joint
	skelJoint currentSkelJoint;

	// Save the name and ID, if applicable
	currentSkelJoint.jName = currentNode->first_attribute("id")->value();
	if ( currentNode->first_attribute("sid") != 0 )
		currentSkelJoint.jID = currentNode->first_attribute("sid")->value();
	else
		currentSkelJoint.jID = "";

	// Save the parent's index
	currentSkelJoint.jParentIndex = parentIndex;

	// Get the bind pose matrix
	string token;
	string matrixArray = currentNode->first_node("matrix")->value();

	std::istringstream isM(matrixArray);
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 4; row++)
		{
			if ( getline(isM, token, ' ') )
			{
				currentSkelJoint.jBindPose.set(row, col, atof(token.c_str()) );
			}
			else
				break;
		}
	}

	// Joint should be complete at this point, add it to the array
	skelJoints.push_back(currentSkelJoint);

	// Search children of this node and call this function on them too
	int currentIndex = skelJoints.size() - 1;
	if (currentNode->first_node("node") != 0)
	{
		bool didChildren = false;
		xml_node<>* childNode = currentNode->first_node("node");

		do {
			loadSkelJoint(childNode, currentIndex);

			if ( childNode->next_sibling("node") != 0)
				childNode = childNode->next_sibling("node");
			else
				didChildren = true;

		} while (!didChildren);
	}
}

unsigned int ColladaSkeleton::getDataSize()
{
	// Incomplete, obviously
	return static_cast<unsigned int> (0);
}

skelPose * ColladaSkeleton::buildSkeleton()
{
	skelPose * currentSkelPose = new skelPose;

	currentSkelPose->skelPoseJoints.reserve(skelJoints.size());

	// Create a skelPoseJoint for each skelJoint
	for (int i=0; i<skelJoints.size(); i++)
	{
		skelPoseJoint currentSkelPoseJoint;

		currentSkelPoseJoint.pjID = skelJoints[i].jID;
		currentSkelPoseJoint.pjParentIndex = skelJoints[i].jParentIndex;
		currentSkelPoseJoint.pjJointTransform = skelJoints[i].jBindPose;
		if (skelJoints[i].jParentIndex >= 0)
		{
			currentSkelPoseJoint.pjJointTransform = currentSkelPoseJoint.pjJointTransform * currentSkelPose->skelPoseJoints[skelJoints[i].jParentIndex].pjJointTransform;
		}

		currentSkelPose->skelPoseJoints.push_back(currentSkelPoseJoint);
	}

	return currentSkelPose;
}

void ColladaSkeleton::updateSkeleton(skelPose * currentPose, float currentTime)
{
	for (int i=0; i<currentPose->skelPoseJoints.size(); i++)
	{
		if (skelJoints[i].jKeyframeTime.size() != 0)
		{
			int currentFrame = 0;

			for (int j=0; j<skelJoints[i].jKeyframeTime.size(); j++)
			{
				if (currentTime < (skelJoints[i].jKeyframeTime[j]) )
				{
					currentFrame = j - 1;
					break;
				}
			}

			if (currentFrame < 0)
				currentFrame = 0;
			
			// Interpolate
			if (true)
			{
				Matrix tempMatrix;

				int nextFrame = (currentFrame  + 1);
				if (nextFrame >= skelJoints[i].jKeyframeTime.size())
					nextFrame = 1;

				float between = currentTime - (skelJoints[i].jKeyframeTime[currentFrame]);
				between = between / ( (skelJoints[i].jKeyframeTime[nextFrame]) - (skelJoints[i].jKeyframeTime[currentFrame]) );

				for (int h = 0; h<15; h++)
				{
					tempMatrix.set(h, ( skelJoints[i].jKeyframeMatrix[currentFrame].InvMatrix.get(h) + between * (skelJoints[i].jKeyframeMatrix[nextFrame].InvMatrix.get(h) - skelJoints[i].jKeyframeMatrix[currentFrame].InvMatrix.get(h))) );
				}
				tempMatrix.set(15, 1);
			
				currentPose->skelPoseJoints[i].pjJointTransform = tempMatrix;
			}
			else
			{
				currentPose->skelPoseJoints[i].pjJointTransform = skelJoints[i].jKeyframeMatrix[currentFrame].InvMatrix;
			}
		}
		else
		{
			currentPose->skelPoseJoints[i].pjJointTransform = skelJoints[i].jBindPose;
		}

		if (skelJoints[i].jParentIndex >= 0)
		{
			currentPose->skelPoseJoints[i].pjJointTransform = currentPose->skelPoseJoints[i].pjJointTransform * currentPose->skelPoseJoints[skelJoints[i].jParentIndex].pjJointTransform;
		}
	}

}

void ColladaSkeleton::traceSkeleton(skelPose * currentPose)
{
	float tranM[16];

	for (int i=0; i<currentPose->skelPoseJoints.size(); i++)
	{
		currentPose->skelPoseJoints[i].pjJointTransform.getMatrix(&tranM[0]);

		glPushMatrix();
		glMultMatrixf(&tranM[0]);

		glPointSize(5.0f);
		glBegin(GL_POINTS);
			glVertex3f(0.0f,0.0f,0.0f);
		glEnd();

		glPopMatrix();
	}
}