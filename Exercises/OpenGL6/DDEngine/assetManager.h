#define assetManager_D __declspec(dllimport)

#ifndef assetManager__H 
#define assetManager__H

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

struct colladaSkelInterfaceContainer
{
	ColladaSkeleton * colladaSkel;
	std::string skelMD5;
};

using namespace std;

class assetManager__H AssetManager
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

	// light methods
	//Light * newLight(void);

	// mutex methods
	static void lockMutex( SDL_mutex * m );
	static void unlockMutex( SDL_mutex * m );

private:
	std::map <char *, textureContainer> texture_list;
	std::map <char *, md2InterfaceContainer> md2_list;
	std::map <char *, colladaInterfaceContainer> collada_list;
	std::map <char *, colladaSkelInterfaceContainer> colladaskel_list;
};

#endif