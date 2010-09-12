#include <list>
#include <stdlib.h>

using namespace std;

class Transformation 
{
	public:
		Transformation();
		virtual ~Transformation();

		virtual void applyTransformation();
		virtual void addTransformation();
		virtual void removeTransformation();
		virtual Transformation* retrieveTransformation();

	private:
		list<Transformation> transformationStack;
}