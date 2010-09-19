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
		int magic; 
		int version; 
		int skinWidth; 
		int skinHeight; 
		int frameSize; 
		int numSkins; 
		int numVertices; 
		int numTexCoords; 
		int numTriangles; 
		int numGlCommands; 
		int numFrames; 
		int offsetSkins; 
		int offsetTexCoords; 
		int offsetTriangles; 
		int offsetFrames; 
		int offsetGlCommands; 
		int offsetEnd; 
	};
	

	// Custom Struct Definitions
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

	struct vertex
	{
		unsigned char vert[3];
		unsigned char normalIndex;
	};

	struct frame
	{
		float scale[3];
		float translate[3];
		char  name[16];
		vertex vertices[1];
	};

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

	struct glCommandList
	{
		int num;
		glCommandVertex verts[1];
	};

protected:

	// loads a MD2 file
	bool Load(const char* filename);

	// funcs that use pointer offsets to return parts of the currently loaded MD2.

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
