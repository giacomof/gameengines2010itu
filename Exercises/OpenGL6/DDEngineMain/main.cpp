#include <DDEngine.h>


int main(int argc, char *argv[])
{
	DDEngine engine = DDEngine(800, 600, 32, true);
	engine.run();

	return 0;
}