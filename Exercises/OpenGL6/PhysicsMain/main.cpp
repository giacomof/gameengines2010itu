#include "extendedEngine.h"

int main(int argc, char *argv[])
{
	extendedEngine engine = extendedEngine(800, 600, 32, true);
	engine.run();

	return 0;
}