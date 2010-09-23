#pragma once

#include <il.h>
#include <ilu.h>
#include <ilut.h>
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
		virtual void drawGeometry() {};
		virtual void update(void) {};
};

class Text : public SceneObject 
{
	public:
		Text(char * p_text, int size);
		~Text(void);

		// delete the {} when rewrite this method
		void drawGeometry(void) {};
		void update(void) {};

		void setTextSize(int size);
		void setText(char p_text);
};

class Sphere : public SceneObject 
{
	public:
		Sphere(float rad, int sli, int sta, bool w);
		~Sphere(void);

		void drawGeometry(void);
		void update(void) {};

		float getSphereRadius(void);
		int getSphereSlices(void);
		int getSphereStacks(void);

		float radius;
		int slices, stacks;
		bool isWireframe;

};

class Plane : public SceneObject 
{
	public:
		Plane(float w, float h);
		~Plane(void);

		void drawGeometry(void);
		void update(void) {};

		void setDimensions(float w, float h);

		float width, height;
};

class Cube : public SceneObject
{
	public:
		Cube(float s);
		~Cube(void);

		void drawGeometry(void);
		void update(void) {};

		void setSide(float s);

		float side;
};

class Line : public SceneObject
{
	public:
		Line(Vector lStart, Vector lEnd);
		
		void update(void) {};
		void drawGeometry(void);
		
		void setLine(Vector lStart, Vector lEnd); 
		Vector getLine();

		Vector lineStart, lineEnd, lineVector;
};

class md2File : public SceneObject 
{
	public:
		md2File(md2Loader * m, unsigned int texture);
		~md2File(void);

		void update(void);
		void drawGeometry(void);

		md2Loader * mesh;
		int md2Texture; 
};

class FBXfile : public SceneObject
{
	public:
		FBXfile(char * filename);
		~FBXfile(void);

		void update(void) {};
		void drawGeometry(void);
};