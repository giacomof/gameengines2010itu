#include <iostream>
#include <sstream>
#include <fstream>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;


int main() {

	
	std::ifstream ifs("astroBoy_walk_Maya.dae");
	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	

	xml_document<> doc;    // character type defaults to char
	doc.parse<0>(strdup(str.c_str()));    // 0 means default parse flags

	// Texture file name
	string textureName;
	// int for containing the number of vertices, normals and maps
	int vertexCount, normalCount, mapCount;
	// string for containing vertices, normals and maps
	string vertexArray, normalArray, mapArray;

	
	//cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

	// Get the name of the texture
	xml_node<>* rootNode = doc.first_node("COLLADA");
	xml_node<>* textureNode = rootNode->first_node("library_images")->first_node("image")->first_node("init_from");
    textureName = textureNode->value();

	// Get the vertices data
	xml_node<>* geometryRootNode = rootNode->first_node("library_geometries")->first_node("geometry")->first_node("mesh");
	xml_node<>* tempNode = geometryRootNode->first_node("source");
	vertexCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	vertexArray = tempNode->first_node("float_array")->value();

	// Get the normals data
	tempNode = tempNode->next_sibling();
	normalCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	normalArray = tempNode->first_node("float_array")->value();

	// Get the maps data
	tempNode = tempNode->next_sibling();
	mapCount = atoi(tempNode->first_node("float_array")->first_attribute("count")->value());
	mapArray = tempNode->first_node("float_array")->value();

    cout << textureName.c_str() << endl;

	//cout << vertexCount << endl;
	//cout << VertexArray.c_str() << endl;

	//cout << normalCount << endl;
	//cout << normalArray.c_str() << endl;

	//cout << mapCount << endl;
	//cout << mapArray.c_str() << endl;


	//string token, text("Here:is:some:text");


	// Memory reservation for arrays of float that will contain the data
	float * vertex = (float *)malloc(vertexCount*sizeof(float));
	float * normal = (float *)malloc(normalCount*sizeof(float));
	float * map = (float *)malloc(mapCount*sizeof(float));

	// Token used for splitting strings
	string token;
	// Int used as index of the array
	int i = 0;

	// Tokenize and convert the vertex string
	std::istringstream isV(vertexArray);
	while ( getline(isV, token, ' ') )
	{
		vertex[i] = atof ( token.c_str() );
		i++;
	}


	// Tokenize and convert the normal string
	i = 0;
	std::istringstream isN(normalArray);
	while ( getline(isN, token, ' ') )
	{
		normal[i] = atof ( token.c_str() );
		i++;
	}

	// Tokenize and convert the map string
	i = 0;
	std::istringstream isM(mapArray);
	while ( getline(isM, token, ' ') )
	{
		map[i] = atof ( token.c_str() );
		i++;
	}

	cout << vertexCount << endl;
	cout << vertex[0] << endl;
	cout << vertex[1] << endl;
	cout << vertex[vertexCount-1] << endl;

	cout << normalCount << endl;
	cout << normal[0] << endl;
	cout << normal[1] << endl;
	cout << normal[normalCount-1] << endl;

	cout << mapCount << endl;
	cout << map[0] << endl;
	cout << map[1] << endl;
	cout << map[mapCount-1] << endl;

	system("PAUSE");
    return 0;
}
