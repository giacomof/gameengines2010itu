#ifndef colladaFile__H__
#define colladaFile__H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <glut.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class ColladaFile
{
public:

	// Constructor
	ColladaFile(void);
	// Destructor
	~ColladaFile(void);
	// releases the current model
	void release(void);
	// loads the collada file
	bool load(const char* filename);
	// renders the collada model
	void render(void) const;
	// returns the total size of the model in bytes
	unsigned int getDataSize() const;

	// *****************************************************
	// ******************* Variables ***********************
	// *****************************************************

	// character type defaults to char
	xml_document<> doc;   
	// Texture file name
	string textureName;
	// int for containing the number of vertices, normals and maps
	unsigned int vertexCount, normalCount, mapCount, indexCount, vertexStride, normalStride, mapStride;
	// string for containing vertices, normals and maps
	string vertexArray, normalArray, mapArray, indexArray;
	float * vertex, * normal, * map;
	unsigned int * index;

};

#endif