#ifdef __DDEngine
#    define colladaFile_D __declspec(dllexport)
#else
#    define colladaFile_D __declspec(dllimport)
#endif

#ifndef colladaFile_H
#define colladaFile_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <omp.h>

#include "linearAlgebra.h"
#include "rapidxml.hpp"
#include "memoryManager.h"
#include "globals.h"
#include "colladaSkeleton.h"

#include "glut.h"

using namespace std;
using namespace rapidxml;

struct vertexInfluence
{
	unsigned long vInfluenceCount;
	vector<unsigned long> vInfluenceIndex;
};

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
	void render(void);
	void render(skelPose * currentPose);
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

	// Variables for geometry
	unsigned int vertexCount, normalCount, mapCount, indexCount, vertexStride, normalStride, mapStride, indexStride, offset;
	float * vertex, * normal, * map;
	unsigned long * index;

	// Variables for skinning info
	Matrix skinBindShapeMatrix;
	vector<matrixContainer> skinInvMatrixArray;
	vector<stringContainer> skinBoneIDArray;
	vector<vertexInfluence> skinVertexInfluence;
	//float * skinWeightsArray, * vertexSkinned, *normalSkinned;
	//int * skinBoneIndexArray;
	vector<float> skinWeightsArray;
	vector<float> vertexSkinned;
	vector<float> normalSkinned;
	vector<unsigned int> skinBoneIndexArray;

	bool hasTexture, hasSkeletonWeights;
};

#endif