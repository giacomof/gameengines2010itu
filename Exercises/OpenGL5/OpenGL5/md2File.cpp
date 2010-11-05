#include "md2File.h"


#define MD2_FRAME_RATE (1.0f/MD2_FRAMES_PER_SEC)


vec3 md2File::anorms[] = {
	#include "anorms.h"
};

// LERP Interpolation
template< typename InType >
inline void LERP(float out[], const InType a[], const InType b[], const float interp_t) {
	float inv_interp_t = 1.0f - interp_t;
	out[0] = a[0]*inv_interp_t + b[0]*interp_t;
	out[1] = a[1]*inv_interp_t + b[1]*interp_t;
	out[2] = a[2]*inv_interp_t + b[2]*interp_t;
}

// constructor
md2File::md2File() 
{
	m_AnimTime=0;
	m_Verts=0;
	m_CurrentAnim=0;
	m_NumAnims=0;
	currentFrame = 0;
	interp_t = 0; 

}

// destructor
md2File::~md2File() {
	delete [] m_Verts;
	delete [] m_data;
}

// resets all variables and clears animation buffer
void md2File::Clear() 
{
	m_Anims.clear();
	m_AnimTime=0;
	m_Verts=0;
	m_CurrentAnim=0;
	m_NumAnims=0;
	m_data=0;
	currentFrame = 0;
	interp_t = 0; 
}

// returns the 3d mesh
const md2File::model* md2File::GetModel() {
	if(!m_data) return 0;
	void *p=m_data;
	return reinterpret_cast<model*>( p );
}

// returns the data of a single frame of the animation
const md2File::frame* md2File::GetFrame(unsigned int num) {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetFrames + (num*pm->frameSize);
	return reinterpret_cast<frame*>(ptr);
}

// returns the Triangles List
const md2File::triangle* md2File::GetTriangles() {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetTriangles;
	return reinterpret_cast<triangle*>(ptr);
}

// returns the uv coordinates of the texture (mapped inside the md2)
const md2File::uv* md2File::GetTexCoords() {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetTexCoords;
	return reinterpret_cast<uv*>(ptr);
}


const char*	md2File::GetSkin(unsigned int num) {
	if(!m_data) return 0;
	void* p = m_data + GetModel()->offsetSkins + num*64;
	return reinterpret_cast<char*>(p);
}

// loads the md2 model and allocate proper memory
bool md2File::Load(unsigned char * p_data, unsigned int p_size ) {
	Clear();

	data_size = p_size;
	m_data = new unsigned char[data_size];

	for(unsigned int i = 0; i < data_size; i++) {
		m_data[i] = p_data[i];
	}

	if (!checkFile())
		return false;

	// Read triangle data
	triangles = reinterpret_cast<triangle *>(m_data + GetModel()->offsetTriangles);

	// allocate memory for vertex data
	m_Verts = new float[ 3 * GetNumVerts() ];
			
	// get number of frames
	unsigned int nf = GetModel()->numFrames;

	// name of the first frame
	std::string firstName="";

	// references to animation cycles
	AnimRef ref;
	ref.m_FrameStart=0;
	ref.m_FrameCount=0;

	// loop through all frames to find counts
	for( unsigned int i=0 ; i != nf; ++i )
	{
		const frame* thisFrame = GetFrame(i);

		if(firstName=="" || strncmp(firstName.c_str(),thisFrame->name,firstName.size()) != 0 )
		{
			firstName = thisFrame->name;
			if(firstName[firstName.size()-2]=='0')
				firstName.resize(firstName.size()-2);
			else 
				firstName.resize(firstName.size()-1);

			if(ref.m_FrameCount!=0) {
				m_Anims.push_back(ref);
				ref.m_FrameStart=i;
				ref.m_FrameCount=0;
			}
		}
		++ref.m_FrameCount;
	}

	// make sure we don't miss the last cycle
	if(ref.m_FrameCount!=0) 
		m_Anims.push_back(ref);

	return true;
}

bool md2File::checkFile(void) {

	// ensure format is valid
	if( GetModel()->magic != 0x32504449 ||
		GetModel()->version != 8 ) {
			delete [] m_data;
			m_data  = 0;
			std::cout << "[ERROR] the MD2 does not contain 'IDP2' flag"
					  << std::cout;
			return false;
	}

	return true;	
}

// update the animation state
void md2File::Update(float dt) {
	if(!m_data)
		return;
	
	// Compute max frame index
	int maxFrame = GetModel()->numFrames - 1;

	m_AnimTime += dt;

	// get pointer to animation reference
	const AnimRef * panimref = &(m_Anims[m_CurrentAnim]);

	// get total length of the animation
	float anim_len = (panimref->m_FrameCount*MD2_FRAME_RATE);

	// ensure our anim time does not go above the max time
	while(m_AnimTime > anim_len) {
		m_AnimTime -= anim_len;
	}

	// need to find the two frames between which we are animating
	int f1 = panimref->m_FrameStart;
	float time_between_frames = m_AnimTime;

	// find the first frame id, and calculate time between frames
	while( time_between_frames > MD2_FRAME_RATE ) {
		time_between_frames -= MD2_FRAME_RATE;
		++f1;
	}


	// second frame is either first in cycle (if looping) or
	// the next frame. 
	int f2 = panimref->m_FrameStart;
	if( f1 < (panimref->m_FrameStart+panimref->m_FrameCount-1) ) {
		f2 = f1+1;
		
	}

	currentFrame = f2;

	// get pointers to frames
	const frame* pf1 = GetFrame(f1);
	const frame* pf2 = GetFrame(f2);

	// scale this to 1 to zero for calculation
	interp_t = time_between_frames/MD2_FRAME_RATE;

	// need to interpolate the scale and translation values for the keys
	float scale[3];
	float translate[3];

	// LERP scale
	LERP(scale,pf1->scale,pf2->scale,interp_t);

	// LERP translation
	LERP(translate,pf1->translate,pf2->translate,interp_t);

	// LERP each vertex and transform to final correct size
	unsigned short num_verts = GetNumVerts();
	unsigned int out_vert_idx=0;
	for(unsigned int i=0; i!= num_verts; ++i, out_vert_idx+=3 )
	{
		// temp buffer
		float temp[3];

		//LERP vertex
		LERP(temp,pf1->vertices[i].vert,pf2->vertices[i].vert,interp_t);

		// get out vertex
		float *vert = m_Verts + out_vert_idx;

		// scale vertex and flip coords
		vert[0] =    temp[0]*scale[0] + translate[0];
		vert[2] =  -(temp[1]*scale[1] + translate[1]);
		vert[1] =    temp[2]*scale[2] + translate[2];

	}			
}

// draw the mesh with OpenGL commands
void md2File::Render() {

	// ensure valid model loaded
	if(!m_data)
		return;

	// Compute max frame index
	int maxFrame = GetModel()->numFrames - 1;

	// decompress the uv data
	float uv_scale_s = 1.0f / GetModel()->skinWidth;
	float uv_scale_t = 1.0f / GetModel()->skinHeight;

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(uv_scale_s,uv_scale_t,1.0f);
	glMatrixMode(GL_MODELVIEW);

	// draw each triangle
	glBegin(GL_TRIANGLES);
		// get num of triangles in mesh
		unsigned short nTris = GetNumTriangles();

		// get point to triangle
		const triangle* pt = GetTriangles();

		// loop through each triangle 
		for(unsigned short i=0;i!=nTris;++i) 
		{
			// loop through each vertex on the triangle
			for(unsigned int j=0;j!=3;++j) 
			{
				
				/* ****************************** */
				/* *** Normals Interpolation **** */
				/* ****************************** */
				
				//frameA is the current frame
				const frame * frameA = GetFrame(currentFrame);
				const frame * frameB;
				// frameB is the next frame, or the first one
				// if this was the last frame of the animation
				if(currentFrame == 0) frameB = GetFrame(maxFrame);
				else frameB = GetFrame(currentFrame-1);

				// vertex A is the current vertex
				const vertex vertA = frameA->vertices[ triangles[i].vertexIndices[j] ];
				const vertex vertB = frameB->vertices[ triangles[i].vertexIndices[j] ];

				// normA is the current normal, normB is the next one
				const GLfloat * normA = anorms[ vertA.normalIndex ];
				const GLfloat * normB = anorms[ vertB.normalIndex ];
								
				//LERP(normal, normA, normB, interp_t);

				normal[0] = normA[0] + interp_t * (normB[0] - normA[0]);
				normal[1] = normA[1] + interp_t * (normB[1] - normA[1]);
				normal[2] = normA[2] + interp_t * (normB[2] - normA[2]);

				const float* pvertex = m_Verts + 3*pt[i].vertexIndices[j];
				const uv*    puv     = GetTexCoords() + pt[i].textureIndices[j];
				
				//const float * normal = anorms[ GetFrame(currentFrame)->vertices[ triangles[i].vertexIndices[j] ].normalIndex ];

				//normal[0] = 0;
				//normal[1] = 1;
				//normal[0] = 0;

				// set UV
				glTexCoord2sv(puv->data);
							
				// set normal
				glNormal3fv(normal);

				// draw vertex
				glVertex3fv(pvertex);
			}


		}
	glEnd();

	
		
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

// starts the specified animation
void md2File::SetAnim(unsigned short idx) {
	if(idx < GetNumAnims()) {
		m_CurrentAnim = idx;
		m_AnimTime=0;
	}
}

// returns the name of the frame of the specified animation we are playing
const char* md2File::GetAnimName(unsigned short idx) {
	if(idx<GetNumAnims())
		return GetFrame( m_Anims[idx].m_FrameStart )->name;
	return 0;
}

//returns the number of the animations in the md2 file
unsigned short md2File::GetNumAnims() {
	return static_cast<unsigned short>(m_Anims.size());
}

// returns the size of the model
unsigned int md2File::GetDataSize() {
	return static_cast<unsigned int>(
		sizeof(md2File) + data_size +
		m_Anims.size() * sizeof(AnimRef) +
		sizeof(float)*3*GetNumVerts());
}
