#ifndef MD2FILE__H__
#define MD2FILE__H__

class md2File {

protected:	

	unsigned short GetNumVerts() const {
		return GetModel()->numVertices;
	}
	unsigned short GetNumTriangles() const {
		return GetModel()->numTriangles;
	}

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
	
	// *****************************************************
	// ************* Custom Struct Definitions *************
	// *****************************************************

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

	// embeded list of OpenGL commands
	struct glCommandList
	{
		int num;
		glCommandVertex verts[1];
	};

protected:

	// loads a MD2 file
	bool Load(const char* filename);

	// functions that use pointer offsets to return parts of the currently loaded MD2.
	const model*		 GetModel() const;
	const frame*		 GetFrame(unsigned int num) const;
	const char*			 GetSkin(unsigned int num) const;
	const triangle*		 GetTriangles() const;
	const uv*			 GetTexCoords() const;
	const glCommandList* GetCommands() const;

	unsigned char* m_data;
	unsigned int data_size;

	md2File() : m_data(0) {}
	~md2File() {
		delete [] m_data;
		m_data=0;
	}
};

#endif
