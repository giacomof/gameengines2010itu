#ifndef MD2LOADER__H__
#define MD2LOADER__H__


	#include <vector>
	#include <string>
	#include <iostream>
	#include <string.h>
	#include <assert.h>
	#include <stdlib.h>
	#include <glut.h>
	#include "md2File.h"

	// 1 to force the renderer to use GL_TRIANGLES only, 0 otherwise
	#define MD2_ALWAYS_GL_TRIANGLES 0

	// sets the frames per second for the animations
	#define MD2_FRAMES_PER_SEC 5

	class md2Loader : private md2File {
	public:

		// Constructor
		md2Loader() ;

		// Destructor
		~md2Loader() ;
		
		// releases the current model
		void Release();

		// loads the specified md2 model
		// returns true if OK
		bool Load(const char* filename) ;

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
		unsigned int GetDataSize() const ;

	protected:

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
