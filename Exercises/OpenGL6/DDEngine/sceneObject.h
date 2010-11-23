#ifdef __DDEngine
#    define sceneObject_D __declspec(dllexport)
#else
#    define sceneObject_D __declspec(dllimport)
#endif

#ifndef sceneObject__H__
#define sceneObject__H__

#include "colladaFile.h"
#include "colladaSkeleton.h"
#include "md2File.h"
#include "linearAlgebraDLL.h"
#include "mutexManager.h"

#include <SDL.h>
#include <il.h>
#include <ilu.h>
#include <ilut.h>
#include <vector>

using namespace std;
using namespace linearAlgebraDLL;

// father class
class sceneObject_D SceneObject
{
	public:
		SDL_mutex * mutex_object;

		SceneObject(void);
		virtual ~SceneObject(void);
		virtual void drawGeometry() {};
		virtual void update(void) {};

};

class sceneObject_D Sphere : public SceneObject
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

class sceneObject_D Plane : public SceneObject 
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

class sceneObject_D Cube : public SceneObject
{
	public:
		Cube(float s);
		~Cube(void);

		void drawGeometry(void);
		void update(void) {};

		void setSide(float s);

		float side;
};

class sceneObject_D Line : public SceneObject
{
	public:
		Line(Vector lStart, Vector lEnd);
		
		void update(void) {};
		void drawGeometry(void);
		
		void setLine(Vector lStart, Vector lEnd); 
		Vector getLine();

		Vector lineStart, lineEnd, lineVector;
};

class sceneObject_D Teapot : public SceneObject
{
	public:
		Teapot(float size, bool wireframe);
		
		void update(void) {};
		void drawGeometry(void);
		
		void setDimension(float size); 
		float getSize();

		float dimension;
		bool wire;
};

class sceneObject_D md2Interface : public SceneObject 
{
	public:
		md2Interface(md2File * m, unsigned int texture);
		~md2Interface(void);

		void update(void);
		void drawGeometry(void);

		md2File * mesh;
		int md2Texture; 
};

class sceneObject_D ColladaInterface : public SceneObject
{
	public:
		ColladaInterface(ColladaFile * c, unsigned int texture, ColladaSkeleton * s = 0);
		~ColladaInterface(void);

		void update(void);
		void drawGeometry(void);

		ColladaSkeleton * skeleton;
		ColladaFile * mesh;
		int colladaTexture;
		poseJoint * currentPose;

		float animationProgress;
		float animationRate;
};

class sceneObject_D Light : public SceneObject
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