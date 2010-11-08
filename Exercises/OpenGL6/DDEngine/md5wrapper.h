
/*
 *	This is my wrapper-class to create
 *	a MD5 Hash from a string and a file.
 *
 *	This code is completly free, you 
 *	can copy it, modify it, or do 
 *	what ever you want with it.
 *
 *	Feb. 2005
 *	Benjamin Gr�delbach
 */

//include protection
#ifdef __DDEngine
#    define MD5WRAPPER_D __declspec(dllexport)
#else
#    define MD5WRAPPER_D __declspec(dllimport)
#endif

#ifndef MD5WRAPPER_H
#define MD5WRAPPER_H

//basic includes
#include <string>

//forwards
class MD5;

class MD5WRAPPER_D md5wrapper
{
	private:
		MD5 *md5;
	
		/*
		 * internal hash function, calling
		 * the basic methods from md5.h
		 */	
		std::string hashit(std::string text);

		/*
		 * converts the numeric giets to
		 * a valid std::string
		 */
		std::string convToString(unsigned char *bytes);
	public:
		//constructor
		md5wrapper();

		//destructor
		~md5wrapper();
		
		/*
		 * creates a MD5 hash from
		 * "text" and returns it as
		 * string
		 */	
		std::string getHashFromString(std::string text);

		/*
		 * creates a MD5 hash from
		 * a file specified in "filename" and 
		 * returns it as string
		 */	
		std::string getHashFromFile(std::string filename);


		std::string getHashFromFilePtr(FILE * filePtr, int size);
};


//include protection
#endif

/*
 * EOF
 */
