#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <iostream>

#include <map>

using namespace std;

class AssetManager
{

public:
	AssetManager(void);
	~AssetManager(void);

	void loadTexture(char * fileName, char * textureName);
	unsigned int getTexture(char * textureName);

private:
	std::map <char *, unsigned int> texture_list;

};

