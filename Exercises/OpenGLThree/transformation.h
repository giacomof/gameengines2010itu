#include <list>
#include <stdlib.h>

using namespace std;

class Transformation 
{
	public:
		Transformation(void);
		virtual ~Transformation(void);

		virtual void applyTransformation(void);
		virtual void addTransformation(Transformation t);
		virtual void removeTransformation(void);
		virtual void setTransformationStack(list<Transformation>);
		virtual Transformation retrieveTransformation(void);
		virtual list<Transformation> retrieveTransformationStack(void);
	private:
		list<Transformation> transformationStack;
}

class TraslationTransformation : public Transformation
{
	public:
		TraslationTransformation(void);
		~TraslationTransformation(void);

		void applyTransformation(void);
		void addTransformation(void);
		void removeTransformation(void);
		Transformation retrieveTransformation(void);
		virtual list<Transformation> retrieveTransformationStack(void);
		virtual void setTransformationStack(list<Transformation>);

	private:
		list<Transformation> transformationStack;

}

class RotationTransformation : public Transformation
{
	public:
		RotationTransformation(void);
		~RotationTransformation(void);

		void applyTransformation(void);
		void addTransformation(void);
		void removeTransformation(void);
		Transformation retrieveTransformation(void);
		virtual list<Transformation> retrieveTransformationStack(void);
		virtual void setTransformationStack(list<Transformation*>);

	private:
		list<Transformation> transformationStack;

}

class ScalingTransformation : public Transformation
{
	public:
		ScalingTransformation(void);
		~ScalingTransformation(void);

		void applyTransformation(void);
		void addTransformation(void);
		void removeTransformation(void);
		Transformation retrieveTransformation(void);
		virtual list<Transformation> retrieveTransformationStack(void);
		virtual void setTransformationStack(list<Transformation>);

	private:
		list<Transformation> transformationStack;

}

class ShearingTransformation : public Transformation
{
	public:
		ShearingTransformation(void);
		~ShearingTransformation(void);

		void applyTransformation(void);
		void addTransformation(void);
		void removeTransformation(void);
		Transformation retrieveTransformation(void);
		virtual list<Transformation> retrieveTransformationStack(void);
		virtual void setTransformationStack(list<Transformation>);

	private:
		list<Transformation> transformationStack;

}

