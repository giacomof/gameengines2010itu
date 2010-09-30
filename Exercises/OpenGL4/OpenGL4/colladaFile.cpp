#include "colladaFile.h"

ColladaFile::ColladaFile(void)
{
}


ColladaFile::~ColladaFile(void)
{
	//delete this;
}

bool ColladaFile::load(const char* filename)
{
	std::ifstream ifs(filename);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	doc.parse<0>(strdup(str.c_str()));    // 0 means default parse flags

	xml_node<>* rootNode = doc.first_node("COLLADA");
	xml_node<>* textureNode = rootNode->first_node("library_images")->first_node("image")->first_node("init_from");
    textureName = textureNode->value();

	// Get the vertices data
	xml_node<>* geometryRootNode = rootNode->first_node("library_geometries")->first_node("geometry")->first_node("mesh");
	xml_node<>* tempNode = geometryRootNode->first_node("source");
	vertexCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	vertexArray = tempNode->first_node("float_array")->value();

	// Get the normals data
	tempNode = tempNode->next_sibling();
	normalCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	normalArray = tempNode->first_node("float_array")->value();

	// Get the maps data
	tempNode = tempNode->next_sibling();
	mapCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	mapArray = tempNode->first_node("float_array")->value();

	// get the indices data
	tempNode = rootNode->first_node("library_geometries")->first_node("geometry")->first_node("mesh")->first_node("triangles");
	indexCount = atoi(tempNode->first_attribute("count")->value());	
	indexArray = tempNode->first_node("p")->value();
	


	// Memory reservation for arrays of float that will contain the data
	vertex = (float *)malloc(vertexCount*sizeof(float));
	normal = (float *)malloc(normalCount*sizeof(float));
	map = (float *)malloc(mapCount*sizeof(float));
	index = (unsigned int *)malloc(indexCount*9*sizeof(unsigned int));

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

	// Tokenize and convert the map string
	i = 0;
	std::istringstream isM(mapArray);
	while ( getline(isM, token, ' ') )
	{
		map[i] = atof ( token.c_str() );
		i++;
	}

	// Tokenize and convert the index string
	i = 0;
	std::istringstream isI(indexArray);
	while ( getline(isI, token, ' ') )
	{
		index[i] = (unsigned int) atoi ( token.c_str() );
		i++;
	}

	std::cout << i << endl;

	return true;
}

void ColladaFile::render(void) const
{
		int firstVertex;
		int secondVertex;
		int thirdVertex;

	for(unsigned int i=0; i<indexCount*9; i+=9) 
	{
		firstVertex = index[i]*3;
		secondVertex = index[i+3]*3;
		thirdVertex = index[i+6]*3;

		glBegin(GL_TRIANGLES);
		glVertex3f( vertex[firstVertex], vertex[firstVertex+1], vertex[firstVertex+2]);
		glVertex3f( vertex[secondVertex], vertex[secondVertex+1], vertex[secondVertex+2]);
		glVertex3f( vertex[thirdVertex], vertex[thirdVertex+1], vertex[thirdVertex+2]);
		glEnd();
	}
	
	



	////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////OLD STUFF/////////////////////////////////////////////////////////////
	//// clean out arrays
	//unsigned int ind = indexCount/2;
	//unsigned int * cleanedIndex = new unsigned int[indexCount];

	//for(unsigned int i=0; i<indexCount*2; i += 2) cleanedIndex[i/2] = index[i];

	//// actual drawing
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, (void*) &vertex[0]);
	//glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, cleanedIndex);
	//glDisableClientState(GL_VERTEX_ARRAY); 


	/*unsigned int c=0;

	for(unsigned int i=0; i<indexCount; i+=3) 
	{
		
		if(c%3==0)  {
			glBegin(GL_TRIANGLES);
			c = 0;
		}
		glVertex3f( vertex[ cleanedIndex[i]*2 ], vertex[ (cleanedIndex[i]*2) +1], vertex[ (cleanedIndex[i]*2) +2] ); 
		c++;
		if(c%3==0) glEnd();

	}*/
	
	//for(unsigned int i=0; i<indexCount; i+=2) 
	//{
	//	glBegin(GL_TRIANGLES);
	//	glVertex3f( vertex[cleanedIndex[i]*2], vertex[cleanedIndex[i]*2+1], vertex[cleanedIndex[i]*2+2]);
	//	glVertex3f( vertex[cleanedIndex[i+1]*2], vertex[cleanedIndex[i+1]*2+1], vertex[cleanedIndex[i+1]*2+2]);
	//	glVertex3f( vertex[cleanedIndex[i+2]*2], vertex[cleanedIndex[i+2]*2+1], vertex[cleanedIndex[i+2]*2+2]);
	//	glEnd();
	//}

	/*std::cout << "index 0: " << index[0] << " cleaned: " << cleanedIndex[0] << endl;
	std::cout << "index 1: " << index[3] << " cleaned: " << cleanedIndex[1] << endl;
	std::cout << "index 2: " << index[6] << " cleaned: " << cleanedIndex[2] << endl;*/

	////////////////////////////////////////////////////////////////////////////////////////////////////
}

unsigned int ColladaFile::getDataSize() const 
{
	return static_cast<unsigned int>(vertexCount*sizeof(float) + normalCount*sizeof(float) + mapCount*sizeof(float) + sizeof(ColladaFile));
}

