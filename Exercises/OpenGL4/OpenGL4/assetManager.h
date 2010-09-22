#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <iostream>
#include <fstream>
#include <map>
#include "md5wrapper.h"

struct textureContainer 
{
	unsigned int textureID;
	std::string textureMD5;
};

using namespace std;

class AssetManager
{



public:
	AssetManager(void);
	~AssetManager(void);

	void loadTexture(char * fileDirectory, char * textureName);
	unsigned int getTexture(char * textureName);

private:
	std::map <char *, textureContainer> texture_list;

};

