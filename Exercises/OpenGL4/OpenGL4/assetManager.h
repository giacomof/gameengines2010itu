#pragma once

#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <iostream>
#include <fstream>
#include <map>
#include "md5wrapper.h"
#include "sceneObject.h"

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

using namespace std;

class AssetManager
{

public:

	static AssetManager _instance;

	AssetManager(void) { &getInstance(); }
	~AssetManager(void);
	AssetManager(const AssetManager &getInstance()); 

	AssetManager & operator=(AssetManager &getInstance());
	AssetManager &getInstance();

	void loadTexture(char * fileDirectory, char * textureName);
	unsigned int getTexture(char * textureName);

	void loadMd2(char * filePath, char * md2NameChar);
	md2File * getMd2Mesh(char * md2NameChar);
	
	char * loadCollada(char * filePath, char * colladaNameChar);
	ColladaFile * getColladaMesh(char * colladaNameChar);

private:
	std::map <char *, textureContainer> texture_list;
	std::map <char *, md2InterfaceContainer> md2_list;
	std::map <char *, colladaInterfaceContainer> collada_list;
};