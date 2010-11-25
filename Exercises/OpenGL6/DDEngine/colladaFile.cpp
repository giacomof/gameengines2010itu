#include "colladaFile.h"

ColladaFile::ColladaFile(void)
{
	textureName = "";
	hasTexture = false;
	hasSkeletonWeights = false;
}


ColladaFile::~ColladaFile(void)
{
	MemoryManager::newFree(vertex);
	MemoryManager::newFree(normal);
	//MemoryManager::newFree(vertexSkinned);
	//MemoryManager::newFree(normalSkinned);
	if(hasTexture) MemoryManager::newFree(map);
	MemoryManager::newFree(index);
	/*if(hasSkeletonWeights)
	{
		MemoryManager::newFree(skinWeightsArray);
		MemoryManager::newFree(skinBoneIndexArray);
	}*/
}

char * ColladaFile::load(std::string & str)
{
	// resets
	vertexCount = 0;
	normalCount = 0;
	mapCount = 0;
	indexCount = 0;

	vertexStride = 0;
	normalStride = 0;
	mapStride = 0;
	indexStride = 0;

	string vertexArray, normalArray, mapArray, indexArray, stringBindShapeMatrix, stringBoneIDArray, stringInvMatrixArray, stringSkinWeightsArray, stringVertexInfluenceCount, stringVertexInfluenceIndex;
	unsigned int countBoneIDArray, countInvMatrixArray, countSkinWeightsArray;


	offset = 6;

	doc.parse<0>(strdup(str.c_str()));    // 0 means default parse flags

	xml_node<>* rootNode = doc.first_node("COLLADA");
	
	tempNode = rootNode->first_node();
	string tempNodeName = tempNode->name();

	bool isFinished = false;
	while(!isFinished)
	{
		// loop the COLLADA file
		// and load textures, vertex, normals, uvs, and so on
		// and exit the loop when parsing is finished

		if (tempNodeName == "library_images") 
		{
			// texture data
			xml_node<>* textureNode = tempNode->first_node("image")->first_node("init_from");
			textureName = textureNode->value();
			offset = 9;
			hasTexture = true;

		}
		else if (tempNodeName == "library_geometries")
		{
			// Mesh
			xml_node<>* geometryNode = tempNode->first_node("geometry")->first_node("mesh")->first_node("source");
			vertexArray = geometryNode->first_node("float_array")->value();
			vertexCount = atoi(geometryNode->first_node("float_array")->first_attribute("count")->value());
			vertexStride = atoi(geometryNode->first_node("technique_common")->first_node("accessor")->first_attribute("stride")->value());
				
			// Normals
			xml_node<>* normalNode = tempNode->first_node("geometry")->first_node("mesh")->first_node("source")->next_sibling("source");
			normalArray = normalNode->first_node("float_array")->value();
			normalCount = atoi(normalNode->first_node("float_array")->first_attribute("count")->value());
			normalStride = atoi(normalNode->first_node("technique_common")->first_node("accessor")->first_attribute("stride")->value());
				
			// Get the maps data
			xml_node<>* mapNode = tempNode->first_node("geometry")->first_node("mesh")->first_node("source")->next_sibling("source")->next_sibling("source");
			mapArray = mapNode->first_node("float_array")->value();
			mapCount = atoi(mapNode->first_node("float_array")->first_attribute("count")->value());
			mapStride = atoi(mapNode->first_node("technique_common")->first_node("accessor")->first_attribute("stride")->value());

			// Indices
			xml_node<>* indexNode = tempNode->first_node("geometry")->first_node("mesh")->first_node("triangles");
			indexCount += atoi(indexNode->first_attribute("count")->value());
			indexArray += indexNode->first_node("p")->value();
				
			// check if the collada file is a multinode mesh
			bool moreMeshes = true;
			while(moreMeshes)
			{
					
				if (indexNode->next_sibling() != 0)
				{
					indexNode = indexNode->next_sibling();
					moreMeshes = true;
						
					indexCount += atoi(indexNode->first_attribute("count")->value());	
					indexArray += " ";
					indexArray += indexNode->first_node("p")->value();
						
					
				}
				else
				{ 
					moreMeshes = false;

				}
			}
		}
		else if (tempNodeName == "library_controllers")
		{
			xml_node<>* weightsNode = tempNode->first_node("controller")->first_node("skin");

			// Read the bind shape matrix
			stringBindShapeMatrix = weightsNode->first_node("bind_shape_matrix")->value();

			// Read in the BoneID array
			stringBoneIDArray = weightsNode->first_node("source")->first_node("Name_array")->value();
			countBoneIDArray = atoi( weightsNode->first_node("source")->first_node("Name_array")->first_attribute("count")->value() );

			// Read in the inverse pose matrix array
			stringInvMatrixArray = weightsNode->first_node("source")->next_sibling("source")->first_node("float_array")->value();
			countInvMatrixArray = atoi( weightsNode->first_node("source")->next_sibling("source")->first_node("float_array")->first_attribute("count")->value() );

			// Read in the skin weight array
			stringSkinWeightsArray = weightsNode->first_node("source")->next_sibling("source")->next_sibling("source")->first_node("float_array")->value();
			countSkinWeightsArray = atoi( weightsNode->first_node("source")->next_sibling("source")->next_sibling("source")->first_node("float_array")->first_attribute("count")->value() );

			// Read in the vertex influence arrays
			stringVertexInfluenceCount = weightsNode->first_node("vertex_weights")->first_node("vcount")->value();
			stringVertexInfluenceIndex = weightsNode->first_node("vertex_weights")->first_node("v")->value();

			// Vertex weights loaded here
			hasSkeletonWeights = true;
		}

		if(!isFinished)  {
			// check if there are other meshes to load
			if (tempNode->next_sibling() != 0) {
				tempNode = tempNode->next_sibling();
				tempNodeName = tempNode->name();
			} else {
				isFinished = true;
			}
		}
	}

	// Memory reservation for arrays of float that will contain the data
	vertex = (float *)MemoryManager::newMalloc(vertexCount*sizeof(float), GEOMETRY);
	normal = (float *)MemoryManager::newMalloc(normalCount*sizeof(float), GEOMETRY);
	//vertexSkinned = (float *)MemoryManager::newMalloc(vertexCount*sizeof(float), GEOMETRY);
	//normalSkinned = (float *)MemoryManager::newMalloc(normalCount*sizeof(float), GEOMETRY);
	if(hasTexture) map = (float *)MemoryManager::newMalloc(mapCount*sizeof(float), TEXTURE);
	index = (unsigned long *)MemoryManager::newMalloc(indexCount*offset*sizeof(unsigned long), UTILITY);

	vertexSkinned.resize(vertexCount,0.0f);
	normalSkinned.resize(normalCount,1.0f);

	// Token used for splitting strings
	string token;
	// Int used as index of the array
	int i = 0;

	// Tokenize and convert the vertex string
	std::istringstream isV(vertexArray);
	while ( getline(isV, token, ' ') )
	{
		vertex[i] = atof ( token.c_str() );
		i++;
	}

	// Tokenize and convert the normal string
	i = 0;
	std::istringstream isN(normalArray);
	while ( getline(isN, token, ' ') )
	{
		normal[i] = atof ( token.c_str() );
		i++;
	}

	if(hasTexture)
	{
		// Tokenize and convert the map string
		i = 0;
		std::istringstream isM(mapArray);
		while ( getline(isM, token, ' ') )
		{
			map[i] = atof ( token.c_str() );
			i++;
		}
	}

	// Tokenize and convert the index string
	i = 0;
	std::istringstream isI(indexArray);
	while ( getline(isI, token, ' ') )
	{
		index[i] = (unsigned int) atoi ( token.c_str() );
		i++;
	}

	indexStride = offset / 3;

	if (hasSkeletonWeights)
	{
		// Prepare for data
		skinWeightsArray.resize(countSkinWeightsArray, 0.0f);
		//(float *)MemoryManager::newMalloc(countSkinWeightsArray*sizeof(float), UTILITY);

		// Parse Bind Shape Matrix
		string token;
		int i = 0, row = 0, col = 0;
		std::istringstream streamBindShape(stringBindShapeMatrix);
		while ( getline(streamBindShape, token, ' ') )
		{
			row = (int) (i % 4);
			col = (int) (i / 4);
			skinBindShapeMatrix.set(row, col, atof ( token.c_str() ) );
			i++;
		}

		// Parse Bone ID array
		skinBoneIDArray.reserve(countBoneIDArray);
		skinBoneIndexArray.resize(countBoneIDArray);
		//skinBoneIndexArray = (int *)MemoryManager::newMalloc(countBoneIDArray*sizeof(int), UTILITY);
		std::istringstream streamBoneIDArray(stringBoneIDArray);
		while ( getline(streamBoneIDArray, token, ' ') )
		{
			stringContainer newString;
			newString.BoneID = token;
			skinBoneIDArray.push_back(newString);
		}

		// Parse inverse pose matrix array
		int matrixcount = (int) (countInvMatrixArray/16);
		skinInvMatrixArray.resize(matrixcount);
		std::istringstream streamInvMatrixArray(stringInvMatrixArray);
		for (int j=0; j<matrixcount; j++)
		{
			for (i=0; i<16; i++)
			{
				getline(streamInvMatrixArray, token, ' ');
				row = (int) (i % 4);
				col = (int) (i / 4);
				skinInvMatrixArray[j].InvMatrix.set(row, col, atof ( token.c_str() ) );
			}
		}

		// Parse skin weights
		std::istringstream streamSkinWeightsArray(stringSkinWeightsArray);
		i = 0;
		while ( getline(streamSkinWeightsArray, token, ' ') )
		{
			skinWeightsArray[i] = atof ( token.c_str() );
			i++;
		}

		// Parse vertex influence
		skinVertexInfluence.reserve(vertexCount);
		std::istringstream streamVertexInfluenceCount(stringVertexInfluenceCount);
		std::istringstream streamVertexInfluenceIndex(stringVertexInfluenceIndex);
		for (i=0; i<vertexCount; i++)
		{
			vertexInfluence currentInfluence;

			// Get the influence count
			getline(streamVertexInfluenceCount, token, ' ');
			currentInfluence.vInfluenceCount = atoi( token.c_str() );

			// Size the index array
			currentInfluence.vInfluenceIndex.resize(currentInfluence.vInfluenceCount*2,0);
			//(unsigned long *)MemoryManager::newMalloc(currentInfluence.vInfluenceCount*2*sizeof(unsigned long), UTILITY);

			// Parse the indices
			for (int j=0; j<currentInfluence.vInfluenceCount*2; j++)
			{
				getline(streamVertexInfluenceIndex, token, ' ');
				currentInfluence.vInfluenceIndex[j] = atoi( token.c_str() );
			}

			skinVertexInfluence.push_back(currentInfluence);
		}
	}

	if(hasTexture) return (char*)textureName.c_str();
	else return "";
}

void ColladaFile::render(void)
{
	unsigned long firstVertex=0;
	unsigned long secondVertex=0;
	unsigned long thirdVertex=0;

	unsigned long firstNormal=0;
	unsigned long secondNormal=0;
	unsigned long thirdNormal=0;

	unsigned long firstMap=0;
	unsigned long secondMap=0;
	unsigned long thirdMap=0;

	for(unsigned int i=0; i<indexCount*offset; i+=offset) 
	{

		firstVertex		= index[i					]*vertexStride;
		secondVertex	= index[i+indexStride		]*vertexStride;
		thirdVertex		= index[i+indexStride*2		]*vertexStride;
		
		firstNormal		= index[i+1					]*normalStride;
		secondNormal	= index[i+indexStride	+1	]*normalStride;
		thirdNormal		= index[i+indexStride*2	+1	]*normalStride;
		
		if (hasTexture)
		{
			firstMap	= index[i+2					]*mapStride;
			secondMap	= index[i+indexStride	+2	]*mapStride;
			thirdMap	= index[i+indexStride*2	+2	]*mapStride;
		}

		glBegin(GL_TRIANGLES);
			glTexCoord2f( map[firstMap], map[firstMap+1] );
			glNormal3f( normal[firstNormal], normal[firstNormal+1], normal[firstNormal+2]); 
			glVertex3f( vertex[firstVertex]*0.2f, vertex[firstVertex+1]*0.2f, vertex[firstVertex+2]*0.2f);
			glTexCoord2f( map[secondMap], map[secondMap+1] );
			glNormal3f( normal[secondNormal], normal[secondNormal+1], normal[secondNormal+2]);
			glVertex3f( vertex[secondVertex]*0.2f, vertex[secondVertex+1]*0.2f, vertex[secondVertex+2]*0.2f);
			glTexCoord2f( map[thirdMap], map[thirdMap+1] );
			glNormal3f( normal[thirdNormal], normal[thirdNormal+1], normal[thirdNormal+2]);
			glVertex3f( vertex[thirdVertex]*0.2f, vertex[thirdVertex+1]*0.2f, vertex[thirdVertex+2]*0.2f);
		glEnd();
	}
	
}

void ColladaFile::render(skelPose * currentPose)
{
	if (currentPose != NULL && (skinBoneIDArray.size() <= currentPose->skelPoseJoints.size()) && hasSkeletonWeights)
	{
		// Multiply inverse bind-pose matrix and joint transformation matrix for each joint
		int j=0;
		for (int i=0; i<skinBoneIDArray.size(); i++)
		{
			string currentBoneID = skinBoneIDArray[i].BoneID;
			while (currentPose->skelPoseJoints[j].pjID != currentBoneID)
			{
				j++;
				if (j >= currentPose->skelPoseJoints.size())
					j=0;
			}

			skinBoneIndexArray[i] = j;

			currentPose->skelPoseJoints[j].pjTempTransform =  skinInvMatrixArray[i].InvMatrix * currentPose->skelPoseJoints[j].pjJointTransform;

			j++;
		}

		// Based on tutorial at http://www.wazim.com/Collada_Tutorial_2.htm
		for (int currentVertex=0; currentVertex<(vertexCount/3); currentVertex++)
		{
			Vector tempVertex;
			Vector influenceVertex;
			Vector tempNormal;
			Vector tempNormalTransform;

			Vector originalVertex(vertex[currentVertex*vertexStride],vertex[currentVertex*vertexStride+1],vertex[currentVertex*vertexStride+2]);
			//Vector originalNormal(normal[currentVertex*normalStride],normal[currentVertex*normalStride+1],normal[currentVertex*normalStride+2]);

			float finalWeight = 0;
			float normalizedWeight = 0;

			originalVertex = skinBindShapeMatrix * originalVertex;

			for (int currentInfluence=0; currentInfluence<skinVertexInfluence[currentVertex].vInfluenceCount; currentInfluence++)
			{
				influenceVertex = ( (currentPose->skelPoseJoints[skinBoneIndexArray[skinVertexInfluence[currentVertex].vInfluenceIndex[currentInfluence*2]]].pjTempTransform * originalVertex) * skinWeightsArray[skinVertexInfluence[currentVertex].vInfluenceIndex[currentInfluence*2+1]] );
				tempVertex = tempVertex + influenceVertex;

				/*tempNormalTransform = currentPose->skelPoseJoints[skinBoneIndexArray[skinVertexInfluence[currentVertex].vInfluenceIndex[currentInfluence*2]]].pjTempTransform * tempNormal;

				tempNormal = tempNormal + ( tempNormalTransform * skinWeightsArray[skinVertexInfluence[currentVertex].vInfluenceIndex[currentInfluence*2+1]] );*/

				finalWeight += skinWeightsArray[skinVertexInfluence[currentVertex].vInfluenceIndex[currentInfluence*2+1]];
			}

			if (finalWeight != 1.0f)
			{
				normalizedWeight = 1.0f / finalWeight;
				tempVertex = tempVertex * normalizedWeight;
				//tempNormal = tempNormal * normalizedWeight;
			}

			vertexSkinned[currentVertex*vertexStride]	= tempVertex.getX();
			vertexSkinned[currentVertex*vertexStride+1]	= tempVertex.getY();
			vertexSkinned[currentVertex*vertexStride+2]	= tempVertex.getZ();

			/*normalSkinned[currentVertex*normalStride]	= originalNormal.getX();
			normalSkinned[currentVertex*normalStride+1]	= originalNormal.getY();
			normalSkinned[currentVertex*normalStride+2]	= originalNormal.getZ();*/
		}

		unsigned long firstVertex=0;
		unsigned long secondVertex=0;
		unsigned long thirdVertex=0;

		unsigned long firstNormal=0;
		unsigned long secondNormal=0;
		unsigned long thirdNormal=0;

		unsigned long firstMap=0;
		unsigned long secondMap=0;
		unsigned long thirdMap=0;

		for(unsigned int i=0; i<indexCount*offset; i+=offset) 
		{

			firstVertex		= index[i					]*vertexStride;
			secondVertex	= index[i+indexStride		]*vertexStride;
			thirdVertex		= index[i+indexStride*2		]*vertexStride;
		
			firstNormal		= index[i+1					]*normalStride;
			secondNormal	= index[i+indexStride	+1	]*normalStride;
			thirdNormal		= index[i+indexStride*2	+1	]*normalStride;
		
			if (hasTexture)
			{
				firstMap	= index[i+2					]*mapStride;
				secondMap	= index[i+indexStride	+2	]*mapStride;
				thirdMap	= index[i+indexStride*2	+2	]*mapStride;
			}

			glBegin(GL_TRIANGLES);
				glTexCoord2f( map[firstMap], map[firstMap+1] );
				glNormal3f( normalSkinned[firstNormal], normalSkinned[firstNormal+1], normalSkinned[firstNormal+2]); 
				glVertex3f( vertexSkinned[firstVertex]*0.2f, vertexSkinned[firstVertex+1]*0.2f, vertexSkinned[firstVertex+2]*0.2f);
				glTexCoord2f( map[secondMap], map[secondMap+1] );
				glNormal3f( normalSkinned[secondNormal], normalSkinned[secondNormal+1], normalSkinned[secondNormal+2]);
				glVertex3f( vertexSkinned[secondVertex]*0.2f, vertexSkinned[secondVertex+1]*0.2f, vertexSkinned[secondVertex+2]*0.2f);
				glTexCoord2f( map[thirdMap], map[thirdMap+1] );
				glNormal3f( normalSkinned[thirdNormal], normalSkinned[thirdNormal+1], normalSkinned[thirdNormal+2]);
				glVertex3f( vertexSkinned[thirdVertex]*0.2f, vertexSkinned[thirdVertex+1]*0.2f, vertexSkinned[thirdVertex+2]*0.2f);
			glEnd();
		}
	}
	else
	{
		render();
	}
}

unsigned int ColladaFile::getDataSize() const 
{
	return static_cast<unsigned int>(vertexCount*sizeof(float) + sizeof(ColladaFile));
}

