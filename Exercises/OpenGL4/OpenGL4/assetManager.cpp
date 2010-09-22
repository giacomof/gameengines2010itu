#include "assetManager.h"


AssetManager::AssetManager(void)
{
	// Initialization of DevIL
	ilInit(); 
}


AssetManager::~AssetManager(void)
{

}

void AssetManager::loadTexture(char * fileName, char * textureName)
{
	ILuint texid;
	ILboolean success;
	GLuint imageT;

	// Generation of one image name 
	ilGenImages(1, &texid); 
	// Binding of image name
	ilBindImage(texid);
	// Loading of image 
	success = ilLoadImage(fileName);

	if (success)
	{
		// Convert colour (for alpha channel images change to IL_RGBA)
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); 

		if (!success)
		{
			// Error occured 
			//SDL_Quit();
			std::cout << "ERROR" << std::endl;
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

	texture_list[textureName] = imageT;

}

unsigned int AssetManager::getTexture(char * textureName)
{
	return texture_list[textureName];
}