#include "assetManager.h"

// static parameter for passing arguments to shaders
int AssetManager::loc;
bool AssetManager::firstTimeDebugShader = true;
std::map <char *, GLuint> AssetManager::shadingProgram_list;

AssetManager AssetManager::_instance;

AssetManager::~AssetManager(void)
{
	for ( std::map<char *, md2InterfaceContainer>::const_iterator iter = md2_list.begin(); iter != md2_list.end(); ++iter ) {
		MemoryManager::newFree(iter->second.md2Mesh);
	}
}

AssetManager & AssetManager::getInstance()
{
  return _instance;
}

void AssetManager::loadTexture(char * filePath, char * textureName)
{
	// Initialization of DevIL
	ilInit(); 
	
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
		file = fopen(filePath, "rb");
		if(file == NULL)
			file = fopen("defaultAssets/defaultTexture.jpg", "rb");
		fseek(file, 0, SEEK_END);
		size = ftell(file);

		lump = (ILubyte*)MemoryManager::newMalloc(size, Globals::TEXTURE);
		fseek(file, 0, SEEK_SET);
		fread(lump, 1, size, file);
		fclose(file);
		// This will store the md5 hash of a file called test.txt in a string hash1
		hash = md5.getHashFromFilePtr((FILE *)lump, (int) size);
	} catch (exception&) {
		MemoryManager::newFree(lump);
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

	MemoryManager::newFree(lump);

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
	FILE * file;

	try {
		file = fopen(filePath,"rb");
		if(file == NULL) 
		{
			file = fopen("defaultAssets/defaultMd2.md2","rb");
		}
		fseek(file,0,SEEK_END);
		data_size = ftell(file);
		m_data = new unsigned char[data_size];
		//assert(m_data);

		rewind(file);

		fread(m_data,sizeof(unsigned char),data_size,file);
		fclose(file);
	} catch (exception&) {
		return;
	}

	
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

		md2File * temp;
		temp = new md2File();
		temp->Load(m_data, data_size);

		md2_list[md2NameChar].md2Mesh = temp;
		md2_list[md2NameChar].meshMD5 = hash;

	}

	delete m_data;

}

char * AssetManager::loadCollada(char * filePath, char * colladaNameChar)
{
	bool exist = false;
	unsigned char * m_data;
	std::string hash;
	md5wrapper md5;
	FILE * file;

	try {
		file = fopen(filePath,"rb");
		if(file == NULL) 
			filePath = "defaultAssets/defaultCollada.dae";
		else
			fclose(file);
	} catch (exception&) {
		return "";
	}
	std::ifstream ifs(filePath);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	hash = md5.getHashFromFilePtr((FILE *)str.c_str(), (int) str.length());

	// use const_iterator to walk through elements of pairs
	for ( std::map<char *, colladaInterfaceContainer>::const_iterator iter = collada_list.begin(); iter != collada_list.end(); ++iter ) {

		if (iter->second.meshMD5 == hash) {
			if (iter->first == colladaNameChar) {
				exist = true;
				break;
			} else {
				collada_list[colladaNameChar].colladaMesh = iter->second.colladaMesh;
				collada_list[colladaNameChar].meshMD5 = iter->second.meshMD5;
				exist = true;
				break;
			}
		}
	}

	if (exist == false) {
		ColladaFile * tempCollada;
		tempCollada = new ColladaFile();
		char * tempName = tempCollada->load(str);
		collada_list[colladaNameChar].colladaMesh = tempCollada;
		collada_list[colladaNameChar].meshMD5 = hash;
	
		if(tempName!="") {
			string path = "include/" + (string) tempName;
			// Had to comment this out so that program doesnt crash if file doesnt exist
			//loadTexture( (char*) path.c_str(), tempName );
			return tempName;
	
		} else return "";
	}
}

bool AssetManager::loadColladaSkeleton(char * filePath, char * colladaNameChar)
{
	bool exist = false;
	unsigned char * m_data;
	std::string hash;
	md5wrapper md5;

	std::ifstream ifs(filePath);
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	hash = md5.getHashFromFilePtr((FILE *)str.c_str(), (int) str.length());

	// use const_iterator to walk through elements of pairs
	for ( std::map<char *, colladaSkelInterfaceContainer>::const_iterator iter = colladaskel_list.begin(); iter != colladaskel_list.end(); ++iter )
	{
		if (iter->second.skelMD5 == hash) {
			if (iter->first == colladaNameChar) {
				exist = true;
				break;
			} else {
				colladaskel_list[colladaNameChar].colladaSkel = iter->second.colladaSkel;
				colladaskel_list[colladaNameChar].skelMD5 = iter->second.skelMD5;
				exist = true;
				break;
			}
		}
	}

	if (exist == false) {
		ColladaSkeleton * tempCollada;
		tempCollada = new ColladaSkeleton();
		bool success = tempCollada->load(str);
		if (success) {
			colladaskel_list[colladaNameChar].colladaSkel = tempCollada;
			colladaskel_list[colladaNameChar].skelMD5 = hash;
			return true;
		}
		else
		{
			return false;
		}
	}
}

md2File * AssetManager::getMd2Mesh(char * md2NameChar) 
{
	return md2_list[md2NameChar].md2Mesh;
}

ColladaFile * AssetManager::getColladaMesh(char * colladaNameChar) 
{
	return collada_list[colladaNameChar].colladaMesh;
}

ColladaSkeleton * AssetManager::getColladaSkeleton(char * colladaNameChar) 
{
	return colladaskel_list[colladaNameChar].colladaSkel;
}

void AssetManager::loadSkyBoxTexture()
{
	// Initialization of DevIL
	ilInit(); 

	skyBoxTexture = (unsigned int*)MemoryManager::newMalloc(sizeof(unsigned int)*6, Globals::TEXTURE);

	char** textureNameList = (char**)MemoryManager::newMalloc(sizeof(char**)*6, Globals::TEXTURE);
	textureNameList[0] = "assets/skyBoxTop.png";
	textureNameList[1] = "assets/skyBoxFront.png";
	textureNameList[2] = "assets/skyBoxBottom.png";
	textureNameList[3] = "assets/skyBoxRear.png";
	textureNameList[4] = "assets/skyBoxRight.png";
	textureNameList[5] = "assets/skyBoxLeft.png";

	ILuint texid;
	ILboolean success;
	GLuint imageT;
	std::string hash;
	ILubyte * lump;
	ILuint size;
	FILE * file;

	for(int i = 0; i < 6; i++)
	{
		try {
			file = fopen(textureNameList[i], "rb");
			fseek(file, 0, SEEK_END);
			size = ftell(file);

			lump = (ILubyte*)MemoryManager::newMalloc(size, Globals::TEXTURE);
			fseek(file, 0, SEEK_SET);
			fread(lump, 1, size, file);
			fclose(file);
		} catch (exception&) {
			MemoryManager::newFree(lump);
			return;
		}

	
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

		skyBoxTexture[i] = imageT;
	}
}

unsigned int * AssetManager::getSkyBoxTextureList()
{
	return skyBoxTexture;
}

void AssetManager::createShadingProgram(char * vertexShaderPath, char * fragmentShaderPath, char * programName)
{

		GLuint vertexShaderReference = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderReference = glCreateShader(GL_FRAGMENT_SHADER);

		char * vertexShaderFile = readShaderFile(vertexShaderPath);
		char * fragmentShaderFile = readShaderFile(fragmentShaderPath);

		const char * vv = vertexShaderFile;
		const char * ff = fragmentShaderFile;

		glShaderSource(vertexShaderReference, 1, &vv,NULL);
		glShaderSource(fragmentShaderReference, 1, &ff,NULL);

		free(vertexShaderFile);
		free(fragmentShaderFile);

		glCompileShader(vertexShaderReference);
		glCompileShader(fragmentShaderReference);
		printShaderInfoLog(vertexShaderReference);
		printShaderInfoLog(fragmentShaderReference);

		GLuint programReference = glCreateProgram();
		glAttachShader(programReference, vertexShaderReference);
		glAttachShader(programReference, fragmentShaderReference);
		
		shadingProgram_list[programName] = programReference;
}

char * AssetManager::readShaderFile(char * filePath) {
	FILE *filePtr;
	char *content = NULL;

	int fileLength = 0;

	if (filePath != NULL) {
		filePtr = fopen(filePath,"rt");

		if (filePtr != NULL) {
      
      fseek(filePtr, 0, SEEK_END);
      fileLength = ftell(filePtr);
      rewind(filePtr);

			if (fileLength > 0) {
				content = (char *)MemoryManager::newMalloc(sizeof(char) * (fileLength + 1), Globals::SHADER);
				fileLength = fread(content, sizeof(char), fileLength, filePtr);
				content[fileLength] = '\0';
			}
			fclose(filePtr);
		}
	}
	return content;
}

void AssetManager::activateShadingProgram(char * shadingProgramName)
{
	GLuint p = shadingProgram_list[shadingProgramName];
	glLinkProgram(p);
	glUseProgram(p);

	AssetManager::loc = glGetUniformLocation(p, "flag");
}

void AssetManager::deactivateShadingProgram()
{
	glUseProgram(0);
}

void AssetManager::setShaderFlag(int flag)
{
	glUniform1i(AssetManager::loc, flag);
}

	void AssetManager::printShaderInfoLog(GLuint obj)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	    if (infologLength > 0)
	    {
			if(firstTimeDebugShader)
			{
				cout << "*************************************" << endl;
				cout << "********** SHADER INFO **************" << endl;
				cout << "*************************************" << endl << endl;
				firstTimeDebugShader = false;
			}

	        infoLog = (char *)MemoryManager::newMalloc(infologLength, Globals::SHADER);
	        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			
			cout << infoLog << endl;
	        MemoryManager::newFree(infoLog);
	    }
	}