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

					// this assumes tempJointID is jointID with "-transform" appended. We could make this more robust I guess
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
							JointArray[i].jAnimated = false;
							break;
						}
					}
				}

				// Only continue if we found the match
				if (matchFound)
				{
					JointArray[i].jAnimated = true;

					// Go through the channels to find the associated sampler
					xml_node<>* currentChannel = currentAnimationJoint->first_node("channel");

					bool channelsDone = false;
					while (!channelsDone)
					{
						// Get source (sampler) and target IDs
						string targetID = currentChannel->first_attribute("target")->value();
						string samplerID = currentChannel->first_attribute("source")->value();

						// Extract matrix position from target ID (I hope all files have it in this format)
						int x, y;
						x = atoi( targetID.substr(targetID.size()-5,1).c_str() );
						y = atoi( targetID.substr(targetID.size()-2,1).c_str() );
						//std::cout << targetID << " reads as ( " << x << " , " << y << " )\n";

						// Constrain x and y just to be sure
						if ( x > 3 || x < 0 ) x = 0;
						if ( y > 3 || y < 0 ) y = 0;

						// Now get the sample and use it to load the data
						xml_node<>* currentSampler = currentAnimationJoint->first_node("sampler");

						bool samplerFound = false;
						while (!samplerFound)
						{
							string tempString = currentSampler->first_attribute("id")->value();
							if ( samplerID.compare(1, samplerID.size() - 1, tempString) == 0 )
							{
								samplerFound = true;
							}
							else
							{
								if ( currentSampler->next_sibling("sampler") != 0 )
								{
									currentSampler = currentSampler->next_sibling("sampler");
								}
								else
								{
									// Uh oh! Here be dragons.
									std::cout << "Couldn't find sampler \"" << samplerID << "\"\n";
									JointArray[i].jAnimated = false;
									break;
								}
							}
						}

						// We have a sampler now, right? We should have if samplerFound is true...
						if (samplerFound)
						{
							// Let's take this guy apart and see where our arrays are
							xml_node<>* currentInput = currentSampler->first_node("input");

							string inputID = "", outputID = "", interID = "";

							while (true)
							{
								string semantic = currentInput->first_attribute("semantic")->value();

								if ( semantic == "INPUT" )
									inputID = currentInput->first_attribute("source")->value();
								else if ( semantic == "OUTPUT" )
									outputID = currentInput->first_attribute("source")->value();
								else if ( semantic == "INTERPOLATION" )
									interID = currentInput->first_attribute("source")->value();
								else
								{
									// uhm, whoops. What does this do?
									std::cout << "Unrecognized input for \"" << samplerID << "\"\n";
								}

								if (currentInput->next_sibling("input") != 0)
									currentInput = currentInput->next_sibling("input");
								else
									break;
							}

							if (inputID != "" && outputID != "" && interID != "")
							{
								xml_node<>* currentArrayNode = currentAnimationJoint->first_node("source");

								// Let's find and load our arrays
								string inputString, outputString, interString;
								int inputCount = 0, outputCount = 0, interCount = 0;
								while (!inputCount || !outputCount || !interCount)
								{
									string tempArrayID = currentArrayNode->first_attribute("id")->value();
									if ( inputID.compare(1, inputID.size() - 1, tempArrayID) == 0 )
									{
										inputCount = atoi(currentArrayNode->first_node("float_array")->first_attribute("count")->value());
										inputString = currentArrayNode->first_node("float_array")->value();
									}
									else if ( outputID.compare(1, outputID.size() - 1, tempArrayID) == 0 )
									{
										outputCount = atoi(currentArrayNode->first_node("float_array")->first_attribute("count")->value());
										outputString = currentArrayNode->first_node("float_array")->value();
									}
									else if ( interID.compare(1, interID.size() - 1, tempArrayID) == 0 )
									{
										interCount = atoi(currentArrayNode->first_node("Name_array")->first_attribute("count")->value());
										interString = currentArrayNode->first_node("Name_array")->value();
									}
									if (currentArrayNode->next_sibling("source") != 0)
										currentArrayNode = currentArrayNode->next_sibling("source");
									else
										break;
								}

								// Get the smallest count
								int count;
								if (inputCount < outputCount)
									count = inputCount;
								else
									count = outputCount;

								if (interCount < count)
									count = interCount;

								// Avast! Prepare to be parsed!
								string token;
								float * inputArray, * outputArray;
								int * interArray;
								
								inputArray = (float *)malloc(inputCount*sizeof(float));
								outputArray = (float *)malloc(outputCount*sizeof(float));
								interArray = (int *)malloc(interCount*sizeof(int));

								// parse the input array
								std::istringstream inputStream(inputString);
								while ( getline(inputStream, token, ' ') )
								{
									int j = 0;
									inputArray[j] = atof ( token.c_str() );
									j++;
								}

								// parse the output array
								std::istringstream outputStream(outputString);
								while ( getline(outputStream, token, ' ') )
								{
									int j = 0;
									outputArray[j] = atof ( token.c_str() );
									j++;
								}

								// parse the interpolation
								std::istringstream interStream(interString);
								while ( getline(interStream, token, ' ') )
								{
									int j = 0;
									if ( token == "LINEAR" )
									{
										interArray[j] = 1;
										//std::cout << "Interpolation type is \"" << token << "\"\n";
									}
									else
										interArray[j] = 1;

									j++;
								}

								JointArray[i].ChannelMatrix[x][y].ArraySize = count;
								JointArray[i].ChannelMatrix[x][y].inputArray = inputArray;
								JointArray[i].ChannelMatrix[x][y].outputArray = outputArray;
								JointArray[i].ChannelMatrix[x][y].interpolationArray = interArray;
								//std::cout << "Filled a channel!\n";
								// Phew. And that was just one channel, for one joint!
							}
							else
							{
								JointArray[i].jAnimated = false;
							}
						}

						if ( currentChannel->next_sibling("channel") != 0 )
						{
							currentChannel = currentChannel->next_sibling("channel");
						}
						else
						{
							channelsDone = true;
						}
					}
				}
				//std::cout << "jAnimated on Joint " << i << " is " << JointArray[i].jAnimated << "\n";
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
	currentJoint.jAnimated = false;

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
				currentJoint.inversePose.set(col, row, atof(token.c_str()) );
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
	unsigned int size = 0;

	size = size + sizeof(ColladaSkeleton);

	for (int i = 0; i < JointArray.size(); i++)
	{
		size = size + sizeof(Joint);

		size = size + sizeof(Matrix);
		size = size + sizeof(int);
		size = size + sizeof(bool);
		size = size + 32 * sizeof(char);

		if ( JointArray[i].jAnimated )
		{
			int arraysize = JointArray[i].ChannelMatrix[0][0].ArraySize;

			size = size + 16 * ( arraysize * 2 * sizeof(float) );
			size = size + 16 * ( (arraysize + 1) * sizeof(int) );
		}

	}

	return static_cast<unsigned int> (size);
}

poseJoint * ColladaSkeleton::buildSkeleton()
{
	poseJoint * rootJoint = new poseJoint;

	rootJoint->jointInversePose = JointArray[0].inversePose;
	rootJoint->jointID = JointArray[0].jBoneID;
	rootJoint->jointIndex = 0;

	for (int i = 1; i<JointArray.size(); i++)
	{
		if (JointArray[i].jParentIndex == 0)
		{
			rootJoint->jointChildren.push_back(new poseJoint);
			buildChildJoint(rootJoint->jointChildren.back(), i);
		}
	}

	return rootJoint;
}

void ColladaSkeleton::buildChildJoint(poseJoint * currentJoint, int currentIndex)
{
	currentJoint->jointInversePose = JointArray[currentIndex].inversePose;
	currentJoint->jointID = JointArray[currentIndex].jBoneID;
	currentJoint->jointIndex = currentIndex;

	for (int i = currentIndex + 1; i<JointArray.size(); i++)
	{
		if (JointArray[i].jParentIndex == currentIndex)
		{
			currentJoint->jointChildren.push_back(new poseJoint);
			buildChildJoint(currentJoint->jointChildren.back(), i);
		}
	}
}

void ColladaSkeleton::updateSkeleton(poseJoint * rootJoint)
{

}

void ColladaSkeleton::traceSkeletonJoint(poseJoint * currentJoint)
{
	float tranM[16];
	currentJoint->jointInversePose.getMatrix(&tranM[0]);

	glPushMatrix();
	glMultMatrixf(&tranM[0]);

	glColor3f(1.0f,1.0f,1.0f);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glVertex3f(0.0f,0.0f,0.0f);
	glEnd();

	for (int i = 0; i<currentJoint->jointChildren.size(); i++)
	{
		currentJoint->jointChildren[i]->jointInversePose.getMatrix(&tranM[0]);
		glBegin(GL_LINES);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(tranM[12], tranM[13], tranM[14]);
		glEnd();
		traceSkeletonJoint(currentJoint->jointChildren[i]);
	}

	glPopMatrix();
}