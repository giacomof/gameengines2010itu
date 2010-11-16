#include "colladaSkeleton.h"

ColladaSkeleton::ColladaSkeleton(void)
{

}


ColladaSkeleton::~ColladaSkeleton(void)
{

}

bool ColladaSkeleton::load(std::string & str)
{
	// Prepare array
	JointArray.clear();

	// Vars
	bool isFinishedSkel = false;
	bool isFinishedAnim = false;
	string rootNodeName = "";

	// Open file
	xml_document<> doc; 
	xml_node<>* tempNode;

	doc.parse<0>(strdup(str.c_str()));    // 0 means default parse flags
	xml_node<>* rootNode = doc.first_node("COLLADA");
	tempNode = rootNode->first_node();
	string tempNodeName = tempNode->name();

	while(!isFinishedSkel || !isFinishedAnim)
	{
		//////////////////////////////////////////
		/*			Load Skeleton				*/
		//////////////////////////////////////////
		if(tempNodeName == "library_visual_scenes") 
		{
			// set the visual scene node that we work from
			xml_node<>* visualSceneNode = tempNode->first_node("visual_scene");

			// Find the node with the instance controllers
			xml_node<>* currentNode = visualSceneNode->first_node("node");
			xml_node<>* rootNode;
			bool nodeFound = false;
			while (!nodeFound)
			{
				if (currentNode->first_node("instance_controller") != 0)
				{
					// We've found the node
					currentNode = currentNode->first_node("instance_controller")->first_node("skeleton");
					rootNodeName = currentNode->value();
					nodeFound = true;
				}
				else if (currentNode->next_sibling() != 0)
				{
					// Check the next node
					currentNode = currentNode->next_sibling();
				}
				else
				{
					// Haven't found it and we're out of nodes
					return false;
				}
			}

			// reset variable for reuse
			nodeFound = false;
			currentNode = visualSceneNode->first_node("node");
			// find the root node
			while (!nodeFound)
			{
				// Wrote this in a bit of an adhoc way. apologies
				if ( currentNode->first_node("node") != 0 )
				{
					if ( currentNode->first_node("node")->first_attribute("id") != 0 )
					{
						string tempString = currentNode->first_node("node")->first_attribute("id")->value();
						//std::cout << "Found a subnode with name \"" << tempString << "\", comparing against \"" << rootNodeName << "\"\n";
						if ( rootNodeName.compare(1, rootNodeName.size() - 1, tempString) == 0)
						{
							// We've found the node
							//std::cout << "Node matched!\n";
							rootNode = currentNode->first_node("node");
							nodeFound = true;
						}
					}
				}
				
				if ( !nodeFound && currentNode->next_sibling() != 0)
				{
					// Check the next node
					currentNode = currentNode->next_sibling();
				}
				else if ( !nodeFound )
				{
					// Haven't found it and we're out of nodes
					std::cout << "Skeleton load failed - Couldn't find a node matching root name!\n";
					return false;
				}
			}

			// Start recursively building the skeleton
			parseChildJoint(rootNode, -1);

			// JointArray should be full now
			isFinishedSkel = true;
			// std::cout << "Joint Array has " << JointArray.size() << " joints.\n";
		}

		//////////////////////////////////////////
		/*			Load Animations				*/
		//////////////////////////////////////////
		if(tempNodeName == "library_animations" && isFinishedSkel)
		{
			// Load and store animations
			// Need to set up a stringstream and append -transform to joint IDs, then search if an animation set exists for the joint.
			// If set is found, parse channel values to associate a sampler with a matrix position
			// Parse sampler to identify input, output and interpolation arrays
			// Parse those and store them
			xml_node<>* animationsNode = tempNode;

			for (int i=0; i<JointArray.size(); i++)
			{
				// Set the current animation joint to the first one under the root
				xml_node<>* currentAnimationJoint = animationsNode->first_node("animation");

				// Get the current joint's ID.
				string jointID = JointArray[i].jName;

				// Search for a joint animation matching this joint ID
				bool matchFound = false;
				while (!matchFound)
				{
					string tempJointID = currentAnimationJoint->first_attribute("id")->value();

					if (tempJointID.compare(0, tempJointID.size() - 10, jointID) == 0)
					{
						// We found the animations for this joint.
						//std::cout << "Found animation data matching \"" << jointID << "\"\n";
						matchFound = true;
					}
					else
					{
						if (currentAnimationJoint->next_sibling("animation") != 0)
						{
							// Try the next one
							currentAnimationJoint = currentAnimationJoint->next_sibling("animation");
						}
						else
						{
							// Out of nodes. No animation exists for this joint
							break;
						}
					}
				}

				// Only continue if we found the match
				if (matchFound)
				{
					// Nothing so far.
				}
			}

			// Animation data should be loaded now
			isFinishedAnim = true;
		}

		if(!isFinishedSkel || !isFinishedAnim)  {
			// check if there are more libraries to search
			if (tempNode->next_sibling() != 0)
				tempNode = tempNode->next_sibling();
			else
				tempNode = rootNode->first_node();

			tempNodeName = tempNode->name();
		}
	}

	return true;
}

void ColladaSkeleton::parseChildJoint(xml_node<>* currentNode, int parentIndex)
{
	// Make the joint
	Joint currentJoint;
	currentJoint.jParentIndex = parentIndex;
	currentJoint.jName = currentNode->first_attribute("id")->value();
	currentJoint.jAnim = NULL;

	// get the bone ID, if any
	if ( currentNode->first_attribute("sid") != 0 )
		currentJoint.jBoneID = currentNode->first_attribute("sid")->value();
	else
		currentJoint.jBoneID = "";

	// Temp string for storing float array in
	string matrixArray;
	// Get the matrix values
	matrixArray = currentNode->first_node("matrix")->value();
	// Token used for splitting matrixArray string
	string token;
	// Tokenize and convert the matrix string
	std::istringstream isM(matrixArray);
	// Parse tokens and insert value in matrix
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if ( getline(isM, token, ' ') )
			{
				currentJoint.inversePose.set(row, col, atof(token.c_str()) );
			}
			else
				break;
		}
	}

	// Joint should be complete at this point, add it to the array
	JointArray.push_back(currentJoint);
	int currentIndex = JointArray.size() - 1;

	/*if (currentJoint.jBoneID != "")
	{
		std::cout << "ID: \"" << currentJoint.jName << "\"\nType: " << currentJoint.jBoneID << ", Index: " << currentIndex << ", Parent: " << currentJoint.jParentIndex << "\n";
	}*/

	// Search children of this node and call this function on them too
	if (currentNode->first_node("node") != 0)
	{
		bool didChildren = false;
		xml_node<>* childNode = currentNode->first_node("node");

		do {
			parseChildJoint(childNode, currentIndex);

			if ( childNode->next_sibling("node") != 0)
				childNode = childNode->next_sibling("node");
			else
				didChildren = true;

		} while (!didChildren);
	}
}

unsigned int ColladaSkeleton::getJointCount()
{
	return JointArray.size();
}

unsigned int ColladaSkeleton::getDataSize()
{
	// Very inaccurate. Leaves out a lot of stuff
	return static_cast<unsigned int>( ( JointArray.size() * sizeof(Joint) ) + sizeof(ColladaSkeleton) );
}