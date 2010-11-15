#include "extendedEngine.h"

void extendedEngine::setupScene()
{
	//// ******************************
	//// ******** LOADING POINT *******
	//// ******************************
	assetManager.loadMd2("assets/battledroid.md2", "battleDroid");
	assetManager.loadMd2("assets/Lostsoul.md2", "md2LostSoul");
	assetManager.loadMd2("assets/bosscube.md2", "md2BossCube");
	assetManager.loadCollada("assets/duck.dae", "duck");
	assetManager.loadCollada("assets/astroboy.dae", "astroboy");

	assetManager.loadTexture("assets/battledroid.png", "battleDroidTx");
	assetManager.loadTexture("assets/lostsoul.jpg", "lostSoulTx");
	assetManager.loadTexture("assets/bosscube.jpg", "bossCubeTx");
	assetManager.loadTexture("assets/duckCM.tga", "duckCM.tga");
	assetManager.loadTexture("assets/boy_10.tga", "boy_10.tga");
}
void extendedEngine::frameStarted()
{

}
void extendedEngine::frameEnded()
{

}