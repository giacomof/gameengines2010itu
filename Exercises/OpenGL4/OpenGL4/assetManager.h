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

using namespace std;

class AssetManager
{

public:
	AssetManager(void);
	~AssetManager(void);

	void loadTexture(char * fileDirectory, char * textureName);
	unsigned int getTexture(char * textureName);

	void loadMd2(char * filePath, char * md2NameChar);
	md2File* getMesh(char * md2NameChar);
	
private:
	std::map <char *, textureContainer> texture_list;
	std::map <char *, md2InterfaceContainer> md2_list;

};

