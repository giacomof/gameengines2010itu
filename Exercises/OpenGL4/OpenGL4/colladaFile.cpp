#include "colladaFile.h"

ColladaFile::ColladaFile(void)
{
	textureName = "";
	isFinished = false;
	hasTexture = false;
	offset = 6;
}


ColladaFile::~ColladaFile(void)
{
}

bool ColladaFile::load(const char* filename)
{
	std::ifstream ifs(filename);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	doc.parse<0>(strdup(str.c_str()));    // 0 means default parse flags

	xml_node<>* rootNode = doc.first_node("COLLADA");
	
	tempNode = rootNode->first_node();
	string tempNodeName = tempNode->name();

	while(!isFinished)
	{
		// loop the COLLADA file
		// and load textures, vertex, normals, uvs, and so on
		// and exit the loop when parsing is finished

		if(tempNodeName == "library_images") 
		{
			// texture data
			xml_node<>* textureNode = tempNode->first_node("image")->first_node("init_from");
			textureName = textureNode->value();
			offset = 9;
			hasTexture = true;

		} else {
			if(tempNodeName == "library_geometries")
			{
				// Mesh
				xml_node<>* geometryNode = tempNode->first_node("geometry")->first_node("mesh")->first_node("source")->first_node("float_array");
				vertexCount += atoi(geometryNode->first_attribute("count")->value());
				vertexArray += geometryNode->value();
				
				// Normals
				xml_node<>* normalNode = tempNode->first_node("geometry")->first_node("mesh")->first_node("source");
				normalNode = normalNode->next_sibling();
				normalCount += atoi(normalNode->first_node("float_array")->first_attribute("count")->value());
				normalArray += normalNode->first_node("float_array")->value();
				
				// Indices
				xml_node<>* indexNode = tempNode->first_node("geometry")->first_node("mesh")->first_node("triangles");
				indexCount += atoi(indexNode->first_attribute("count")->value());	
				indexArray += indexNode->first_node("p")->value();

			}
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

	// Get the vertices data
	//xml_node<>* geometryRootNode = rootNode->first_node("library_geometries")->first_node("geometry")->first_node("mesh");
	//xml_node<>* tempNode = geometryRootNode->first_node("source");
	//vertexCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	//vertexArray = tempNode->first_node("float_array")->value();

	// Get the normals data
	//tempNode = tempNode->next_sibling();
	//normalCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	//normalArray = tempNode->first_node("float_array")->value();

	//if(hasTexture)
	//{
	//	// Get the maps data
	//	tempNode = tempNode->next_sibling();
	//	mapCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	//	mapArray = tempNode->first_node("float_array")->value();
	//}

	// get the indices data
	//tempNode = rootNode->first_node("library_geometries")->first_node("geometry")->first_node("mesh")->first_node("triangles");
	//indexCount = atoi(tempNode->first_attribute("count")->value());	
	//indexArray = tempNode->first_node("p")->value();
	
	// Memory reservation for arrays of float that will contain the data
	vertex = (float *)malloc(vertexCount*sizeof(float));
	normal = (float *)malloc(normalCount*sizeof(float));
	if(hasTexture) map = (float *)malloc(mapCount*sizeof(float));
	index = (unsigned int *)malloc(indexCount*offset*sizeof(unsigned int));

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

	return true;
}

void ColladaFile::render(void) const
{
		int firstVertex;
		int secondVertex;
		int thirdVertex;

	for(unsigned int i=0; i<indexCount*offset; i+=offset) 
	{
		firstVertex = index[i]*3;
		secondVertex = index[i+(offset/3)]*3;
		thirdVertex = index[i+((offset/3)*2)]*3;

		glBegin(GL_TRIANGLES);
		glVertex3f( vertex[firstVertex], vertex[firstVertex+1], vertex[firstVertex+2]);
		glVertex3f( vertex[secondVertex], vertex[secondVertex+1], vertex[secondVertex+2]);
		glVertex3f( vertex[thirdVertex], vertex[thirdVertex+1], vertex[thirdVertex+2]);
		glEnd();
	}
	
	
}

unsigned int ColladaFile::getDataSize() const 
{
	return static_cast<unsigned int>(vertexCount*sizeof(float) + sizeof(ColladaFile));
}

