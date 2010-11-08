#ifndef sceneObject__H__
#define sceneObject__H__

#include <SDL.h>
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <vector>
#include "linearAlgebraDLL.h"
#include "md2File.h"
#include "colladaFile.h"
#include "colladaSkeleton.h"
#include "assetManager.h"

using namespace std;
using namespace linearAlgebraDLL;

class SceneObject
{
	public:
		SDL_mutex *mutex_object;

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
		Plane(float w, float h, int sideSubdivisions = 0);
		~Plane(void);

		void drawGeometry(void);
		void update(void) {};

		void setDimensions(float w, float h);

		float width, height;
		float dividedWidth;
		float dividedHeight;
		float halfWidth;
		float halfHeight;

		int subdivisions;
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

class md2Interface : public SceneObject 
{
	public:
		md2Interface(md2File * m, unsigned int texture);
		~md2Interface(void);

		void update(void);
		void drawGeometry(void);

		md2File * mesh;
		int md2Texture; 
};

class ColladaInterface : public SceneObject
{
	public:
		ColladaInterface(ColladaFile * c, unsigned int texture, ColladaSkeleton * s = 0);
		~ColladaInterface(void);

		void update(void);
		void drawGeometry(void);

		ColladaSkeleton * skeleton;
		ColladaFile * mesh;
		int colladaTexture;
};

class Light : public SceneObject
{
	public:
		Light(void);
		Light(	bool enabled, bool directional = false,
				float ambientR = 0, float ambientG = 0, float ambientB = 0,
				float diffuseR = 1, float diffuseG = 1, float diffuseB = 1,
				float specularR = 1, float specularG = 1, float specularB = 1);
		//~Light(void);

		void update(void) {};
		void drawGeometry(void);
		void setDirection(Vector position);		
		bool isEnabled;

	protected:
		static unsigned short lightCount;

		int lightReference;

		float ambientColor[3];
		float diffuseColor[3];
		float specularColor[3];
		float direction[4];
	
};

#endif