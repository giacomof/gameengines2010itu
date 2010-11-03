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
	JointNumber = 0;

	// Vars
	bool isFinished = false;
	string rootNodeName = "";

	// Open file
	xml_document<> doc; 
	xml_node<>* tempNode;

	doc.parse<0>(strdup(str.c_str()));    // 0 means default parse flags
	xml_node<>* rootNode = doc.first_node("COLLADA");
	tempNode = rootNode->first_node();
	string tempNodeName = tempNode->name();

	while(!isFinished)
	{
		// load the skeleton
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

			// If node wasn't found, abort
			if (!nodeFound)
				break;

			// reset variable for reuse
			nodeFound = false;
			currentNode = visualSceneNode->first_node("node");
			// find the root node
			while (!nodeFound)
			{
				// Replace name check with an id check against rootNodeName later
				if ( currentNode->first_node("node")->first_attribute("id")->value() == rootNodeName)
				{
					// We've found the node
					rootNode = currentNode->first_node("node");
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

			// If node wasn't found, abort
			if (!nodeFound)
				break;

			// We should have the root node of the skeleton stored in currentNode at this point
			// Start building skeleton
			bool skeletonDone = false;

			// Repeat for every child node of root
			currentNode = rootNode;
		}

		if(!isFinished)  {
			// check if there are more libraries to search
			if (tempNode->next_sibling() != 0) {
				tempNode = tempNode->next_sibling();
				tempNodeName = tempNode->name();
			} else {
				isFinished = true;
			}
		}
	}

	return false;
}

void ColladaSkeleton::parseChildJoint(xml_node<>* currentNode, int parentIndex)
{
	// Make the joint
	Joint currentJoint;
	currentJoint.jParentIndex = parentIndex;
	currentJoint.jName = currentNode->name();

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

	// Search children of this node and call this function on them too

}

void ColladaSkeleton::render(void) const
{
	
}

unsigned int ColladaSkeleton::getDataSize() const 
{
	return static_cast<unsigned int>(/*vertexCount*sizeof(float) + */sizeof(ColladaSkeleton));
}