#define colladaFile_D __declspec(dllimport)

#ifndef colladaFile_H
#define colladaFile_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <glut.h>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class colladaFile_D ColladaFile
{
public:

	// Constructor
	ColladaFile(void);
	// Destructor
	~ColladaFile(void);
	// releases the current model
	void release(void);
	// loads the collada file
	// and returns the name of the texture
	char * load(std::string & str);
	// renders the collada model
	void render(void) const;
	// returns the total size of the model in bytes
	unsigned int getDataSize() const;

	// *****************************************************
	// ******************* Variables ***********************
	// *****************************************************

	// character type defaults to char
	xml_document<> doc; 
	xml_node<>* tempNode;
	// Texture file name
	string textureName;
	// int for containing the number of vertices, normals and maps
	unsigned int vertexCount, normalCount, mapCount, indexCount, vertexStride, normalStride, mapStride, offset;
	// string for containing vertices, normals and maps
	string vertexArray, normalArray, mapArray, indexArray;
	float * vertex, * normal, * map;
	unsigned long * index;
	bool hasTexture, hasMoreNodes, isFinished;

};

#endif