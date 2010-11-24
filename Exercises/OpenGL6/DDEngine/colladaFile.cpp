#include "colladaFile.h"

ColladaFile::ColladaFile(void)
{
	textureName = "";
	isFinished = false;
	hasTexture = false;
}


ColladaFile::~ColladaFile(void)
{
	MemoryManager::newFree(vertex);
	MemoryManager::newFree(normal);
	if(hasTexture) MemoryManager::newFree(map);
	MemoryManager::newFree(index);
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

	offset = 6;

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
				while(moreMeshes){
					
					if (indexNode->next_sibling() != 0) {
						indexNode = indexNode->next_sibling();
						moreMeshes = true;
						
						indexCount += atoi(indexNode->first_attribute("count")->value());	
						indexArray += " ";
						indexArray += indexNode->first_node("p")->value();
						
					
					} else { 
						moreMeshes = false;

					}
				}
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

	// Memory reservation for arrays of float that will contain the data
	vertex = (float *)MemoryManager::newMalloc(vertexCount*sizeof(float), GEOMETRY);
	normal = (float *)MemoryManager::newMalloc(normalCount*sizeof(float), GEOMETRY);
	if(hasTexture) map = (float *)MemoryManager::newMalloc(mapCount*sizeof(float), TEXTURE);
	index = (unsigned long *)MemoryManager::newMalloc(indexCount*offset*sizeof(unsigned long), UTILITY);

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

	if(hasTexture) return (char*)textureName.c_str();
	else return "";
}

void ColladaFile::render(void) const
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

unsigned int ColladaFile::getDataSize() const 
{
	return static_cast<unsigned int>(vertexCount*sizeof(float) + sizeof(ColladaFile));
}

