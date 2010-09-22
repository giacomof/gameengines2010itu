#pragma once

#include <vector>
#include "linearAlgebraDLL.h"
#include "md2Loader.h"

using namespace std;
using namespace linearAlgebraDLL;

class SceneObject
{
	public:
		SceneObject(void);
		virtual ~SceneObject(void);
};

class Text : SceneObject 
{
	public:
		Text(char * p_text, int size);
		~Text(void);

		void setTextSize(int size);
		void setText(char p_text);
}

class Sphere : SceneObject 
{
	public:
		Sphere(float rad, int sli, int sta);
		~Sphere(void);

		float getSphereRadius(void);
		int getSphereSlices(void);
		int getSphereStacks(void);

		float radius;
		int slices, stacks;

};

class Plane : SceneObject 
{
	public:
		Plane(float w, float h);
		~Plane(void);

		void setDimensions(float w, float h);

		float width, height;
}

class Cube : SceneObject
{
	public:
		Cube(float s);
		~Cube(void);

		void setSide(float s);

		float side;
}

class Line : SceneObject
{
	public:
		Line(Vector lStart, Vector lEnd);
		~Line(void);

		void setLine(Vector lStart, Vector lEnd); 

		Vector lineStart, lineEnd;
}

class Mesh : SceneObject
{
	public:
		Mesh(char * filename);
		~Mesh(void);

}

class md2File : Mesh 
{
	public:
		md2File(char * filename);
		~md2File(void);

		md2Loader * mesh;
		char * textureFile;
		int md2Texture; 
}

class FBXfile : Mesh
{
	public:
		FBXfile(char * filename);
		~FBXfile(void);
}