#include <list>
#include <stdlib.h>

using namespace std;

class Transformation 
{
	public:
		Transformation(void);
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
		TraslationTransformation(void);
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
		RotationTransformation(void);
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
		ScalingTransformation(void);
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

class ShearingTransformation : public Transformation
{
	public:
		ShearingTransformation(void);
		~ShearingTransformation();

		void applyTransformation();
		void addTransformation();
		void removeTransformation();
		ShearingTransformation* retrieveTransformation();
		virtual list<Transformation*> retrieveTransformationStack();
		virtual void setTransformationStack(list<Transformation*>);

	private:
		list<Transformation> transformationStack;

}

