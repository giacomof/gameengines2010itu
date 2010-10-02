#include "assetManager.h"


AssetManager::AssetManager(void)
{
	// Initialization of DevIL
	ilInit(); 
}


AssetManager::~AssetManager(void)
{
	for ( std::map<char *, md2InterfaceContainer>::const_iterator iter = md2_list.begin(); iter != md2_list.end(); ++iter ) {
		free (iter->second.md2Mesh);
	}
	//delete &texture_list;
	//delete &md2_list;
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
	} catch (exception&) {
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
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); 

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

void AssetManager::loadMd2(char * filePath, char * md2NameChar)
{

	bool exist = false;
	unsigned char * m_data;
	unsigned int data_size;
	std::string hash;
	md5wrapper md5;

	FILE* fp = fopen(filePath,"rb");
	if(!fp) {
		std::cout << "[ERROR] \"" 
				  << filePath 
				  << "\" could not be opened"
				  << std::cout;
		return;
	}
	fseek(fp,0,SEEK_END);
	data_size = ftell(fp);
	m_data = new unsigned char[data_size];
	assert(m_data);

	rewind(fp);

	fread(m_data,sizeof(unsigned char),data_size,fp);
	fclose(fp);
	
	// This will store the md5 hash of a file called test.txt in a string hash1
	hash = md5.getHashFromFilePtr((FILE *)m_data, (int) data_size);
	
	// use const_iterator to walk through elements of pairs
	for ( std::map<char *, md2InterfaceContainer>::const_iterator iter = md2_list.begin(); iter != md2_list.end(); ++iter ) {

		if (iter->second.meshMD5 == hash) {
			if (iter->first == md2NameChar) {
				exist = true;
				break;
			} else {
				md2_list[md2NameChar].md2Mesh = iter->second.md2Mesh;
				md2_list[md2NameChar].meshMD5 = iter->second.meshMD5;
				exist = true;
				break;
			}
		}
	}

	if (exist == false) {

		md2File * temp = (md2File *)malloc(sizeof(md2File));
		temp = new md2File();
		temp->Load(m_data, data_size);

		md2_list[md2NameChar].md2Mesh = temp;
		md2_list[md2NameChar].meshMD5 = hash;

	}

	delete m_data;

}

char * AssetManager::loadCollada(char * filePath, char * colladaNameChar)
{
	ColladaFile * tempCollada = (ColladaFile*) malloc(sizeof(ColladaFile));
	tempCollada = new ColladaFile();
	char * tempName = tempCollada->load(filePath);
	collada_list[colladaNameChar].colladaMesh = tempCollada;
	
	if(tempName!="") {
		string path = "include/" + (string) tempName;
		loadTexture( (char*) path.c_str(), tempName );
		return tempName;
	
	} else return "";
}

md2File * AssetManager::getMd2Mesh(char * md2NameChar) 
{
	return md2_list[md2NameChar].md2Mesh;
}

ColladaFile * AssetManager::getColladaMesh(char * colladaNameChar) 
{
	return collada_list[colladaNameChar].colladaMesh;
}