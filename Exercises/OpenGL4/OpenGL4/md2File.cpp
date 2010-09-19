#include "md2File.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>

const md2File::model* md2File::GetModel() const {
	if(!m_data) return 0;
	void *p=m_data;
	return reinterpret_cast<model*>( p );
}

const md2File::frame* md2File::GetFrame(unsigned int num) const {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetFrames + (num*pm->frameSize);
	return reinterpret_cast<frame*>(ptr);
}

const md2File::triangle* md2File::GetTriangles() const {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetTriangles;
	return reinterpret_cast<triangle*>(ptr);
}

const md2File::uv* md2File::GetTexCoords() const {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetTexCoords;
	return reinterpret_cast<uv*>(ptr);
}

const md2File::glCommandList* md2File::GetCommands() const  {
	if(!m_data) return 0;
	const model* pm = GetModel();
	void* ptr = m_data + pm->offsetGlCommands;
	return reinterpret_cast<glCommandList*>(ptr);
}

const char*	md2File::GetSkin(unsigned int num) const {
	if(!m_data) return 0;
	void* p = m_data + GetModel()->offsetSkins + num*64;
	return reinterpret_cast<char*>(p);
}

// this loads an md2File into a single buffer
bool md2File::Load(const char* filename) {
	FILE* fp = fopen(filename,"rb");
	if(!fp) {
		std::cout << "[ERROR] \"" 
				  << filename 
				  << "\" could not be opened"
				  << std::cout;
		return false;
	}
	fseek(fp,0,SEEK_END);
	data_size = ftell(fp);
	m_data = new unsigned char[data_size];
	assert(m_data);
#if 0
	fclose(fp);
	fp = fopen(filename,"rb");
#else
	rewind(fp);
#endif
	fread(m_data,sizeof(unsigned char),data_size,fp);
	fclose(fp);

	// ensure format is valid
	if( GetModel()->magic != 0x32504449 ||
		GetModel()->version != 8 ) {
			delete [] m_data;
			m_data  = 0;
			std::cout << "[ERROR] \"" 
					  << filename 
					  << "\" does not contain 'IDP2' flag"
					  << std::cout;
			return false;
	}

	return true;	
}
