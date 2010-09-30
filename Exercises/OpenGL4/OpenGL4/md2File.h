#ifndef md2File__H__
#define md2File__H__


	#include <vector>
	#include <string>
	#include <iostream>
	#include <string.h>
	#include <assert.h>
	#include <stdlib.h>
	#include <glut.h>

	// sets the frames per second for the animations
	#define MD2_FRAMES_PER_SEC 5

	class md2File {
	public:

		// Constructor
		md2File() ;

		// Destructor
		~md2File() ;
		
		// releases the current model
		void Release();

		// loads the specified md2 model
		// returns true if OK
		bool Load(const char* filename);

		bool loadFile(const char* filename);

		// updates the animation
		// dt is delta time from last frame
		void Update(float dt) ;

		// renders the md2 model using triangles or triangles strips 
		void Render() const;

		// use an animation
		void SetAnim(unsigned short);

		//returns the name of the first frame of the specified animation
		const char* GetAnimName(unsigned short) const;

		// returns the number of the animation cycles of the md2 file
		unsigned short GetNumAnims() const;

		// returns the total size of the model in kb
		unsigned int GetDataSize() const;

	protected:

		unsigned short GetNumVerts() const {
			return GetModel()->numVertices;
		}
		
		unsigned short GetNumTriangles() const {
			return GetModel()->numTriangles;
		}

		// *****************************************************
		// ************* Custom Struct Definitions *************
		// *****************************************************

		// Header of the MD2 file

		struct model
		{ 
			int magic;				// "Magic number" to check for valid MD2
			int version;			// specifies the version of the MD2
			int skinWidth;			// resolution of the texture (dimensions)
			int skinHeight; 
			int frameSize;			
			int numSkins;	
			int numVertices;		// total number of vertices 
			int numTexCoords;		// total number of texture coordinates
			int numTriangles;		// total number of triangles
			int numGlCommands;		// total number of OpenGL commands
			int numFrames;			// total number of animation frames
			int offsetSkins;		// offset to reach the skins definition
			int offsetTexCoords;	// offset to reach the texture coordinates
			int offsetTriangles;	// offset to reach the triangles definition
			int offsetFrames;		// offset to reach the frames definition
			int offsetGlCommands;	// offset to reach the OpenGL commands
			int offsetEnd;			// end of all offsets
		};
	
		// triangles
		struct triangle
		{
			short vertexIndices[3];
			short textureIndices[3];
		};

		// Texture Coordinates
		struct uv
		{
			union {
				struct {
					short s;
					short t;
				};
				short data[2];
			};
		};

		// vertex
		struct vertex
		{
			unsigned char vert[3];
			unsigned char normalIndex;
		};

		// frame (single frame of an animation)
		struct frame
		{
			float scale[3];
			float translate[3];
			char  name[16];
			vertex vertices[1];
		};

		// embeded OpenGL command about vertexes
		struct glCommandVertex
		{
			union {
				struct {
					float s;
					float t;
				};
				float data[2];
			};
			int vertexIndex;
		};

		/// holds a reference to an animation within the MD2 file
		struct AnimRef {

			// Constructor
			AnimRef() {}

			// copy/clone
			AnimRef(const AnimRef& ref) 
				:	m_FrameStart(ref.m_FrameStart),
					m_FrameCount(ref.m_FrameCount){}
			
			// first frame in cycle
			unsigned short m_FrameStart;

			// number of frames in the cycle
			unsigned short m_FrameCount;
		};

		// functions that use pointer offsets to return parts of the currently loaded MD2.
		const model*		 GetModel() const;
		const frame*		 GetFrame(unsigned int num) const;
		const char*			 GetSkin(unsigned int num) const;
		const triangle*		 GetTriangles() const;
		const uv*			 GetTexCoords() const;

		unsigned char* m_data;
		unsigned int data_size;

		// the time of the current animation
		float m_AnimTime;

		// the vertex coordinates
		float* m_Verts;
		
		// the current ID of the animation running
		unsigned short m_CurrentAnim;

		// the number of animations in the md2 
		unsigned short m_NumAnims;
		
		// references for the animation cycles
		std::vector<AnimRef> m_Anims;
	};

#endif
