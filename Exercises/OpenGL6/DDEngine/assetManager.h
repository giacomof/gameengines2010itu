#ifdef __DDEngine
#    define assetManager_D __declspec(dllexport)
#else
#    define assetManager_D __declspec(dllimport)
#endif

#ifndef assetManager__H 
#define assetManager__H

#include <glew.h>								// Header File for GLEW library
#include <iostream>
#include <fstream>
#include <map>
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include "md5wrapper.h"
#include "sceneObject.h"
#include "memoryManager.h"
#include "globals.h"
#include "md2File.h"
#include "colladaFile.h"
#include "colladaSkeleton.h"

struct textureContainer 
{
	unsigned int textureID;
	std::string textureMD5;
};

struct md2InterfaceContainer
{
	md2File * md2Mesh;
	std::string meshMD5;
};

struct colladaInterfaceContainer
{
	ColladaFile * colladaMesh;
	std::string meshMD5;
};

struct colladaSkelInterfaceContainer
{
	ColladaSkeleton * colladaSkel;
	std::string skelMD5;
};

using namespace std;

class assetManager_D AssetManager
{

public:
	// Singleton Statements
	static AssetManager _instance;

	AssetManager(void) { & getInstance(); }
	~AssetManager(void);
	AssetManager(const AssetManager & getInstance()); 

	AssetManager & operator=(AssetManager & getInstance());
	AssetManager & getInstance();

	// texture methods
	void loadTexture(char * fileDirectory, char * textureName);
	unsigned int getTexture(char * textureName);

	// MD2 methods
	void loadMd2(char * filePath, char * md2NameChar);
	md2File * getMd2Mesh(char * md2NameChar);
	
	// COLLADA methods
	char * loadCollada(char * filePath, char * colladaNameChar);
	ColladaFile * getColladaMesh(char * colladaNameChar);
	bool loadColladaSkeleton(char * filePath, char * colladaNameChar);
	ColladaSkeleton * getColladaSkeleton(char * colladaNameChar);

	// Shaders methods
	void createShadingProgram(char * vertexShaderPath, char * fragmentShaderPath, char * programName);
	char * textFileRead(char * filePath);
	static void activateShadingProgram(char * shadingProgramName);
	static void deactivateShadingProgram();

	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);

private:
	std::map <char *, textureContainer> texture_list;
	std::map <char *, md2InterfaceContainer> md2_list;
	std::map <char *, colladaInterfaceContainer> collada_list;
	std::map <char *, colladaSkelInterfaceContainer> colladaskel_list;
	static std::map <char *, GLuint> shadingProgram_list;
};

#endif