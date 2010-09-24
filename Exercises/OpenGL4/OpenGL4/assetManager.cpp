#include "assetManager.h"


AssetManager::AssetManager(void)
{
	// Initialization of DevIL
	ilInit(); 
}


AssetManager::~AssetManager(void)
{

}

void AssetManager::loadTexture(char * fileDirectory, char * textureName)
{
	ILuint texid;
	ILboolean success;
	GLuint imageT;
	bool exist = false;
	std::string hash;
	md5wrapper md5;
	ILubyte * lump;
	ILuint size;
	FILE * file;

	try {
		file = fopen(fileDirectory, "rb");
		fseek(file, 0, SEEK_END);
		size = ftell(file);

		lump = (ILubyte*)malloc(size);
		fseek(file, 0, SEEK_SET);
		fread(lump, 1, size, file);
		fclose(file);
		// This will store the md5 hash of a file called test.txt in a string hash1
		hash = md5.getHashFromFilePtr((FILE *)lump, (int) size);
	} catch (exception& e) {
		free(lump);
		return;
	}

	// use const_iterator to walk through elements of pairs
	for ( std::map<char *, textureContainer>::const_iterator iter = texture_list.begin(); iter != texture_list.end(); ++iter ) {

		if (iter->second.textureMD5 == hash) {
			if (iter->first == textureName) {
				exist = true;
				break;
			} else {
				texture_list[textureName].textureID = iter->second.textureID;
				texture_list[textureName].textureMD5 = iter->second.textureMD5;
				exist = true;
				break;
			}
		}
	}

	if (exist == false) {
		// Generation of one image name 
		ilGenImages(1, &texid); 
		// Binding of image name
		ilBindImage(texid);
		// Loading of image 
		success = ilLoadL(IL_TYPE_UNKNOWN, lump, size);

		if (success)
		{
			// Convert colour (for alpha channel images change to IL_RGBA)
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); 

			if (!success)
			{
				// Error occured 
				//SDL_Quit();
				std::cout << "ERROR LOADING/CONVERTING TEXTURE" << std::endl;
			}
		}

		// Texture name generation
		glGenTextures(1, &imageT); 
		// Binding of texture name
		glBindTexture(GL_TEXTURE_2D, imageT);
		// Linear interpolation for magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Linear interpolation for minifying filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// Texture settings
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData());

		texture_list[textureName].textureID = imageT;
		texture_list[textureName].textureMD5 = hash;
	}

	free(lump);

}

unsigned int AssetManager::getTexture(char * textureName)
{
	return texture_list[textureName].textureID;
}