#include <list>
#define X_AXES = 0;
#define Y_AXES = 1;
#define Z_AXES = 2;

using namespace std;

class Transformation 
{
	public:
		Transformation();
		~Transformation();

		void applyTransformation(void);
		void addTransformation(Transformation t);
		void removeTransformation(void);
		void setTransformationStack(list<Transformation>);
		Transformation retrieveTransformation(void);
		list<Transformation> retrieveTransformationStack(void);

	private:
		list<Transformation> transformationStack;
};

class TraslationTransformation : public Transformation
{
	public:
		TraslationTransformation();
		~TraslationTransformation();

		void applyTransformation(void) { Transformation::applyTransformation(); }
		void addTransformation(TraslationTransformation t) { Transformation::addTransformation(t); }
		void removeTransformation(void) { Transformation::removeTransformation(); }
		Transformation retrieveTransformation(void) { Transformation::retrieveTransformation(); }
		list<Transformation> retrieveTransformationStack(void) { Transformation::retrieveTransformationStack(); }
		void setTransformationStack(list<Transformation> stack) { Transformation::setTransformationStack(stack); }

	private:
		list<Transformation> tempTransformationStack;

		float tX, tY, tZ;

};

class RotationTransformation : public Transformation
{
	public:
		RotationTransformation();
		~RotationTransformation(void);

		void applyTransformation(void) { Transformation::applyTransformation(); }
		void addTransformation(RotationTransformation t) { Transformation::addTransformation(t); }
		void removeTransformation(void) { Transformation::removeTransformation(); }
		Transformation retrieveTransformation(void) { Transformation::retrieveTransformation(); }
		list<Transformation> retrieveTransformationStack(void) { Transformation::retrieveTransformationStack(); }
		void setTransformationStack(list<Transformation> stack) { Transformation::setTransformationStack(stack); }

	private:
		list<Transformation> tempTransformationStack;

		float angle;
		short axes;

};

class ScalingTransformation : public Transformation
{
	public:
		ScalingTransformation(void);
		~ScalingTransformation(void);

		void applyTransformation(void) { Transformation::applyTransformation(); }
		void addTransformation(ScalingTransformation t) { Transformation::addTransformation(t); }
		void removeTransformation(void) { Transformation::removeTransformation(); }
		Transformation retrieveTransformation(void) { Transformation::retrieveTransformation(); }
		list<Transformation> retrieveTransformationStack(void) { Transformation::retrieveTransformationStack(); }
		void setTransformationStack(list<Transformation> stack) { Transformation::setTransformationStack(stack); }

	private:
		list<Transformation> tempTransformationStack;

		float sX, sY, sZ;

};

class ShearingTransformation : public Transformation
{
	public:
		ShearingTransformation(void);
		~ShearingTransformation(void);

		void applyTransformation(void) { Transformation::applyTransformation(); }
		void addTransformation(ShearingTransformation t) { Transformation::addTransformation(t); }
		void removeTransformation(void) { Transformation::removeTransformation(); }
		Transformation retrieveTransformation(void) { Transformation::retrieveTransformation(); }
		list<Transformation> retrieveTransformationStack(void) { Transformation::retrieveTransformationStack(); }
		void setTransformationStack(list<Transformation> stack) { Transformation::setTransformationStack(stack); }

	private:
		list<Transformation> tempTransformationStack;

		float shX, shY, shZ;

};

