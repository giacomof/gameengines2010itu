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
		virtual void setTransformationStack(list<Transformation*>);
		virtual Transformation* retrieveTransformation();
		virtual list<Transformation*> retrieveTransformationStack();
	private:
		list<Transformation> transformationStack;
}

class TraslationTransformation : public Transformation
{
	public:
		TraslationTransformation();
		~TraslationTransformation();

		void applyTransformation();
		void addTransformation();
		void removeTransformation();
		TraslationTransformation* retrieveTransformation();
		virtual list<Transformation*> retrieveTransformationStack();
		virtual void setTransformationStack(list<Transformation*>);

	private:
		list<Transformation> transformationStack;

}

class RotationTransformation : public Transformation
{
	public:
		RotationTransformation();
		~RotationTransformation();

		void applyTransformation();
		void addTransformation();
		void removeTransformation();
		RotationTransformation* retrieveTransformation();
		virtual list<Transformation*> retrieveTransformationStack();
		virtual void setTransformationStack(list<Transformation*>);

	private:
		list<Transformation> transformationStack;

}

class ScalingTransformation : public Transformation
{
	public:
		ScalingTransformation();
		~ScalingTransformation();

		void applyTransformation();
		void addTransformation();
		void removeTransformation();
		ScalingTransformation* retrieveTransformation();
		virtual list<Transformation*> retrieveTransformationStack();
		virtual void setTransformationStack(list<Transformation*>);

	private:
		list<Transformation> transformationStack;

}