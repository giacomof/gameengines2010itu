#include "DDEngine.h"
					
static int const thread_delay = 1;								// Minimum time between loops
static float const PI = 3.14159f;								// PI definition
SDL_mutex * mutex_StackAllocator = SDL_CreateMutex();			// Mutex for Stack Thread Synchronization
SDL_mutex * mutex_SingleFrameAllocator = SDL_CreateMutex();		// Mutex for Single Frame Thread Synchronization
static MemoryManager memMgr;

/* ********************************************* */
/* ******* operator new global overload ******** */
/* ******** just remember to use flags ********* */
/* ************* from globals.h **************** */
/* ********************************************* */

void * operator new(size_t size, unsigned short typeFlag, unsigned short allocatorFlag)
{
	void * storage = NULL;

	switch(allocatorFlag) {

		case STACK_ALLOCATOR:

			MutexManager::lockMutex(mutex_StackAllocator);

			if(verbosityLevel>=3) cout << "NEW WITH FLAG: " << typeFlag <<  " AND USING STACK ALLOCATOR" << endl;
			if(verbosityLog>=1) {
				Log::addToLog(MEMORYMANAGER_LOGFILE, "STACK ALLOCATOR,");
		
				char * sizeAllocated = new char();
				char * flag			 = new char();
				itoa(size, sizeAllocated, 10);
				itoa(typeFlag, flag, 10);
				
				Log::addToLog(MEMORYMANAGER_LOGFILE, sizeAllocated);
				Log::addToLog(MEMORYMANAGER_LOGFILE, ",");
				Log::addToLog(MEMORYMANAGER_LOGFILE, flag);
				Log::addToLog(MEMORYMANAGER_LOGFILE, "\n");
			}

			storage = memMgr.allocateOnStack(size);
			if(NULL == storage)
				throw "allocation fail : no free memory";
    
			MutexManager::unlockMutex(mutex_StackAllocator);

			break;

		case SINGLE_FRAME_ALLOCATOR:

			MutexManager::lockMutex(mutex_SingleFrameAllocator);
			
			if(verbosityLevel>=3) cout << "NEW WITH FLAG: " << typeFlag <<  " AND USING THE SINGLE FRAME ALLOCATOR" << endl;

			if(verbosityLog>=1) {
				Log::addToLog(MEMORYMANAGER_LOGFILE, "SINGLE FRAME ALLOCATOR,");
		
				char * sizeAllocated = new char();
				char * flag			 = new char();
				itoa(size, sizeAllocated, 10);
				itoa(typeFlag, flag, 10);

				Log::addToLog(MEMORYMANAGER_LOGFILE, sizeAllocated);
				Log::addToLog(MEMORYMANAGER_LOGFILE, ",");
				Log::addToLog(MEMORYMANAGER_LOGFILE, flag);
				Log::addToLog(MEMORYMANAGER_LOGFILE, "\n");
			}

			storage = memMgr.allocateOnSingleFrameAllocator(size);
			if(NULL == storage)
				throw "allocation fail : no free memory";

			MutexManager::unlockMutex(mutex_SingleFrameAllocator);
			break;

	}

	return storage;
	
}

void operator delete(void * ptr, unsigned short flag) 
{
	if(verbosityLevel>=3) cout << "DELETE WITH FLAG: " << flag << endl;
	memMgr.freeToLastStackMarker();
}

void operator delete(void * ptr) 
{
	MemoryManager::newFree(ptr);
}

/* This thread handles input */
int threadInput(void *data)
{
	DDEngine * engine;
	engine = (DDEngine*) data;
	inputManager * input = engine->getInputManager();

	// Disable the Windows Cursor
	SDL_ShowCursor(SDL_DISABLE); 
	
	// Binds mouse and keyboard input to the OpenGL window
	SDL_WM_GrabInput(SDL_GRAB_ON); 

	while ( !engine->getController()->quit ) {
		input->update();

		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	SDL_ShowCursor(SDL_ENABLE);

	return 0;
}

///* This thread handles audio */
int threadSound(void *data)
{
	DDEngine * engine;
	engine = (DDEngine*) data;
	inputManager * input = engine->getInputManager();

	soundInit();

	int testsoundint = 4000;
	while ( !engine->getController()->quit )
	{
		testsoundint++;
		if (testsoundint > 7000)
		{
			//soundPlayFile("assets/MENULOOP.WAV");
			testsoundint = 0;
		}
		SDL_Delay(thread_delay);
	}

	soundExit();

	return 0;
}

/* This thread handles physic */
int threadPhysics(void *data)
{
	DDEngine * engine;
	engine = (DDEngine*) data;
	btDiscreteDynamicsWorld * dynamicsWorld = engine->getDynamicWorld();

	while ( !engine->getController()->quit ) {

		// physics simulation
		dynamicsWorld->stepSimulation(	engine->renderClock.getFrameDelta(), 5);

		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	return 0;
}

DDEngine::DDEngine(int screenWidth, int screenHeight, int colorDepth, bool physics)
{
	if(verbosityLog>=1) Log::clearLog(MEMORYMANAGER_LOGFILE);

	screenW  = screenWidth;							// Window Width
	screenH  = screenHeight;						// Window Height
	screenCD = colorDepth;							// Color Depth

	hasPhysics = physics;
	if(hasPhysics)
		physicGravity = Vector(0, -10, 0);			// Gravity Vector

	// Create the asset manager
	assetManager = AssetManager();
	// Create the input pump
	InputPump = MessagePump();
	// Create the window manager
	window = WindowManager();
	// Create the input abstraction layer
	controller = Controller();
	// Create the script handler
	scriptHandler = ScriptHandler();

	// SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO)<0)
	{
		exit(1);
	}

	// Create the window
	window.createWindow(screenW, screenH, screenCD);

	// Set rendering parameters for OpenGL
	if (initGL()==FALSE)
	{
		exit(1);
	}

	// Resize OpenGL window
	window.resizeWindow(screenWidth, screenHeight);

	// Create the root node
	rootNode = Root();
	rootNode.setName("root");

	// Define the exit of the program in relation to SDL variables
	atexit(SDL_Quit);

	// Call the function that initialise the physics
	if(hasPhysics)
		initPhysics();

	renderClock = frameClock();

	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0"))
		exit(1);
	
	//setShaders();
	
	if(verbosityLevel>=1) printDebugInfo();
}

DDEngine::~DDEngine(void)
{
	if(hasPhysics)
	{
		dynamicsWorld->~btDiscreteDynamicsWorld();
		solver->~btSequentialImpulseConstraintSolver();
		dispatcher->~btCollisionDispatcher();
		collisionConfiguration->~btDefaultCollisionConfiguration();
		broadphase->~btDbvtBroadphase();
	}
}

void DDEngine::printDebugInfo(void) 
{
	cout << "*************************************" << endl;
	cout << "********** WINDOW INFO **************" << endl;
	cout << "*************************************" << endl << endl;
	cout << "Width: " << screenW << " - Height: " << screenH << " - Color Depth: " << screenCD << endl << endl;

	cout << "*************************************" << endl;
	cout << "********** ENGINE INFO **************" << endl;
	cout << "*************************************" << endl << endl;
	cout << "Memory Manager: Active - MultiCore: Active - Available Cores: " << omp_get_num_procs() << endl;
	cout << "Resident Threads: Render, Physics, Input, Sound" << endl;
	cout << "Number of automatically parallelized Threads: 2" << endl << endl;


	cout << "*************************************" << endl;
	cout << "************ GAME INFO **************" << endl;
	cout << "*************************************" << endl << endl;
	cout << "Physics: " << hasPhysics;
	if(hasPhysics) 
		cout	<< " - Gravity: " << physicGravity << endl << endl;
	else
		cout << endl << endl;
}

void DDEngine::run(void)
{
	// Set up camera and spectator
	player = new(SCENEGRAPH, STACK_ALLOCATOR) entitySpectator();
	playercamera = new(SCENEGRAPH, STACK_ALLOCATOR) entityCamera();
	player->setCamera(playercamera);
	controller.setPlayerObject(player);

	// this method will be overloaded to setup the scene
	setupScene();

	// Create the thread handles and assign names
	SDL_Thread *id1;
	SDL_Thread *id2;
	SDL_Thread *id3;

	// Create the threads
	if(hasPhysics)
		id1 = SDL_CreateThread ( threadPhysics, this );
	id2 = SDL_CreateThread ( threadSound, this );
	id3 = SDL_CreateThread ( threadInput, this );

	char title[80];
	
	SDL_Event currentEvent;

	// Set up default material
	GLfloat ambient[] = {1.0, 1.0, 1.0, 1};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1};
	GLfloat shine = 64.0;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	
	int frameDelta;

	while(!controller.quit)
	{
		
		frameDelta = renderClock.getFrameDelta();

		sprintf_s(title, "Name Here Engine | %i FPS", renderClock.getFPS() );
		window.setTitle( title, "include/nhe.ico" );
		

		// Time to take care of the SDL events we have recieved
		while(SDL_PollEvent(&currentEvent))
		{
			switch(currentEvent.type)
			{
			// Some general events to handle immediately
			case SDL_ACTIVEEVENT:
				InputPump.sendMessage(currentEvent);
				if (currentEvent.active.state & SDL_APPACTIVE)
				{
					if (currentEvent.active.gain==0)
						window.setActive(FALSE);
					else
						window.setActive(TRUE);
				}
				break;
			case SDL_VIDEORESIZE:
				surface = SDL_SetVideoMode(	currentEvent.resize.w, currentEvent.resize.h, screenCD, window.getVideoFlags());
				if (!surface)
				{
					exit(1);
				}
				window.resizeWindow( currentEvent.resize.w, currentEvent.resize.h);
				break;
			case SDL_QUIT:
				controller.quit = true;
				break;
			// Input events coming below. They are all just passed on to the input pump
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEMOTION:
				InputPump.sendMessage(currentEvent);
				break;
			// Anything else we don't care about
			default:
				break;
			}
		}

		controller.playerObject->update();
		
		// Actual frame rendering happens here
		if (window.getActive() )
		{
			renderClock.frameUpdate();

			memMgr.clearSingleFrameAllocator();

			frameStarted( frameDelta );

			//glEnable(GL_LIGHT0);
			//// Create light components
			//GLfloat ambientLight[] = { 1, 1, 1, 1 };
			//GLfloat diffuseLight[] = { 1, 1, 1, 1 };
			//GLfloat specularLight[] = { 1, 1, 1, 1 };
			//GLfloat position[] = { -1.5f, 100.0f, -100.0f, 1.0f };

			//// Assign created components to GL_LIGHT0
			//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
			//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
			//glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
			//glLightfv(GL_LIGHT0, GL_POSITION, position);


			drawGL( frameDelta );

			frameEnded( frameDelta );

			// Uncomment the next line if you want to test
			// the Single Frame Allocator
			//SceneNode * temp = new(UTILITY, SINGLE_FRAME_ALLOCATOR) SceneNode();

		}		
	}

	//wait for the threads to exit
	if(hasPhysics)
		SDL_WaitThread ( id1, NULL );
	SDL_WaitThread ( id2, NULL );
	SDL_WaitThread ( id3, NULL );
}

int DDEngine::initGL(void)
{
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	// Clears color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// sets the matrix stack as the projection matrix stack
	glMatrixMode(GL_PROJECTION);
	// creates the viewport
	glViewport(0, 0, screenW, screenH);
	// Reset the matrix
	glLoadIdentity();
	// sets the matrix stack as the modelview matrix stack
	glMatrixMode(GL_MODELVIEW);
	gluPerspective(60.0f, screenW/screenH, 0.1f, 5000.0f);
	// enables the Z-buffer
	glEnable(GL_DEPTH_TEST);
	// enables the texture rendering
	//glEnable(GL_TEXTURE_2D);
	// enables smooth shading (garaud)
	glShadeModel(GL_SMOOTH);
	// frontfacing culling
	glEnable(GL_CULL_FACE);
	// enables lighting
	glEnable(GL_LIGHTING);
	// Define material properties
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glColorMaterial(GL_FRONT, GL_SPECULAR);
	//glEnable(GL_COLOR_MATERIAL);

	return 1;
}

int DDEngine::initPhysics(void)
{
	// Broadphase
	broadphase = new btDbvtBroadphase();
	// Collision Configuration
    collisionConfiguration = new btDefaultCollisionConfiguration();
	// Collision Dispatcher
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
	// Constraint Solver
    solver = new btSequentialImpulseConstraintSolver();
	// Initialise the physic world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	// Set gravity
	dynamicsWorld->setGravity(btVector3(physicGravity.getX(),physicGravity.getY(),physicGravity.getZ()));
	// Initialise the debugDrawer
	debugger.setDebugMode( btIDebugDraw::DBG_DrawWireframe ); 
	// Bind the debug drawer to the physic world
	dynamicsWorld->setDebugDrawer(&debugger); 

	return 1;
}

void DDEngine::drawGL(int frameDelta)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	glLoadIdentity();

	// Set the camera
	float * CamTransform = getCamera();

	if(hasPhysics)
		if(drawDebug) 
			dynamicsWorld->debugDrawWorld();

	// update the animation
	rootNode.update(frameDelta);
	// draw everything
	rootNode.drawGeometry();

	// Swaps the buffers
	SDL_GL_SwapBuffers();
}

float* DDEngine::getCamera()
{		
	glMatrixMode(GL_MODELVIEW);

	entityCamera * currentCamera = controller.playerObject->getCamera();

	float * tranM = new float[16];

	Matrix::generateIdentityMatrix().getMatrix(&tranM[0]);

	if ( currentCamera != NULL )
	{
		MutexManager::lockMutex( currentCamera->mutex_object );
		Matrix transformationMatrix = Matrix();

		if(currentCamera->isFollowingNode && currentCamera->positionNode != NULL)
		{
			
			Quaternion orientation = currentCamera->positionNode->getWorldOrientation();
			Vector position = currentCamera->positionNode->getWorldPosition();
			transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(-orientation.getX(), -orientation.getY(), -orientation.getZ(), orientation.getW())).getTranspose();
			transformationMatrix = Matrix::generateTranslationMatrix(-position.getX(), -position.getY(), -position.getZ()).getTranspose() * transformationMatrix;


		}
		else
		{

			transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(1.0,0.0,0.0),currentCamera->pitch)).getTranspose();
			transformationMatrix = Matrix::generateQuaternionRotationMatrix(Quaternion(Vector(0.0,1.0,0.0),currentCamera->yaw)).getTranspose() * transformationMatrix;
			transformationMatrix = Matrix::generateTranslationMatrix(	currentCamera->vPosition[0],
																		currentCamera->vPosition[1],
																		currentCamera->vPosition[2]).getTranspose() * transformationMatrix;			
		}

		transformationMatrix.getMatrix(&tranM[0]);

		MutexManager::unlockMutex( currentCamera->mutex_object );
	}

	glMultMatrixf(&tranM[0]);
	return &tranM[0];
}

void DDEngine::setShaders() 
{	
		char *vs,*fs;
	
		GLuint v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		GLuint f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	
	
		vs = textFileRead("assets/toonf2.vert");
		fs = textFileRead("assets/toonf2.frag");
	
		const char * vv = vs;
		const char * ff = fs;
	
		glShaderSourceARB(v, 1, &vv,NULL);
		glShaderSourceARB(f, 1, &ff,NULL);
	
		MemoryManager::newFree(vs);
		MemoryManager::newFree(fs);
	
		glCompileShaderARB(v);
		glCompileShaderARB(f);
	
		GLuint p = glCreateProgramObjectARB();
		
		glAttachObjectARB(p,v);
		glAttachObjectARB(p,f);
	
		glLinkProgramARB(p);
		glUseProgramObjectARB(p);

}

char * DDEngine::textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)MemoryManager::newMalloc(sizeof(char) * (count+1), SHADER );
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}


btRigidBody * DDEngine::createPhysicalBox(Vector dimension, Vector position, Quaternion orientation, float mass, bool neverSleep)
{
	btCollisionShape * cubeShape = new btBoxShape(btVector3(dimension.getX(), dimension.getY(), dimension.getZ()));

	btScalar cubeMass = mass;
	btVector3 cubeInertia(0,0,0);
	cubeShape->calculateLocalInertia(cubeMass,cubeInertia);

	btRigidBody::btRigidBodyConstructionInfo * cubeRigidBodyCI;

	btDefaultMotionState * cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(orientation.getX(), orientation.getY(), orientation.getZ()),orientation.getW()),btVector3(position.getX(), position.getY(), position.getZ())));
	cubeRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(cubeMass,cubeMotionState,cubeShape,cubeInertia);

	btRigidBody* cubeRigidBody = new btRigidBody(*cubeRigidBodyCI);

	if(neverSleep)
		cubeRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(cubeRigidBody);

	return cubeRigidBody;
}

btRigidBody * DDEngine::createPhysicalSphere(float radius, Vector position, Quaternion orientation, float mass, bool neverSleep)
{
	btCollisionShape* sphereShape = new btSphereShape(radius);

	btScalar sphereMass = mass;
	btVector3 sphereInertia(0,0,0);
	sphereShape->calculateLocalInertia(sphereMass,sphereInertia);

	btRigidBody::btRigidBodyConstructionInfo * sphereRigidBodyCI;

	btDefaultMotionState * sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(orientation.getX(), orientation.getY(), orientation.getZ()),orientation.getW()),btVector3(position.getX(), position.getY(), position.getZ())));
	sphereRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(sphereMass,sphereMotionState,sphereShape,sphereInertia);

	btRigidBody* sphereRigidBody = new btRigidBody(*sphereRigidBodyCI);

	if(neverSleep)
		sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(sphereRigidBody);

	return sphereRigidBody;
}

btRigidBody * DDEngine::createRigidBody(btCollisionShape * collisionShape, Vector position, Quaternion orientation, float mass, bool neverSleep)
{
	btScalar shapeMass = mass;
	btVector3 shapeInertia(0,0,0);
	collisionShape->calculateLocalInertia(shapeMass,shapeInertia);

	btRigidBody::btRigidBodyConstructionInfo * shapeRigidBodyCI;

	btDefaultMotionState * shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(orientation.getX(), orientation.getY(), orientation.getZ()),orientation.getW()),btVector3(position.getX(), position.getY(), position.getZ())));
	shapeRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(shapeMass,shapeMotionState,collisionShape,shapeInertia);

	btRigidBody* shapeRigidBody = new btRigidBody(*shapeRigidBodyCI);

	if(neverSleep)
		shapeRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(shapeRigidBody);

	return shapeRigidBody;
}

btCollisionShape * DDEngine::createCollisionBox(Vector dimension)
{
	btCollisionShape* cubeShape = new btBoxShape(btVector3(dimension.getX(), dimension.getY(), dimension.getZ()));
	return cubeShape;
}

btCollisionShape * DDEngine::createCollisionSphere(float radius)
{
	btCollisionShape* sphereShape = new btSphereShape(radius);
	return sphereShape;
}

SceneObject * DDEngine::createMD2(md2File * model, unsigned int texture)
{
	md2Interface * md2Model = new(GEOMETRY, STACK_ALLOCATOR) md2Interface(model, texture);
	return (SceneObject*) md2Model;
}

SceneObject * DDEngine::createCollada(ColladaFile * model, unsigned int texture, ColladaSkeleton * skeleton)
{
	ColladaInterface * colladaModel = new(GEOMETRY, STACK_ALLOCATOR) ColladaInterface(model, texture, skeleton);
	return (SceneObject*) colladaModel;
}

SceneObject * DDEngine::createSphere(float radius, int slices, int stacks, bool wireframe)
{
	Sphere * sphere = new(GEOMETRY, STACK_ALLOCATOR) Sphere(radius, slices, stacks, wireframe);
	return (SceneObject*) sphere;
}

SceneObject * DDEngine::createPlane(float width, float height, int sideSubdivisions)
{
	SceneObject * plane = new(GEOMETRY, STACK_ALLOCATOR) Plane(width, height, sideSubdivisions);
	return  plane;
}

SceneObject * DDEngine::createCube(float side)
{
	Cube * cube = new(GEOMETRY, STACK_ALLOCATOR) Cube(side);
	return (SceneObject*) cube;
}

SceneObject * DDEngine::createLine(Vector start, Vector end)
{
	Line * line = new(GEOMETRY, STACK_ALLOCATOR) Line(start, end);
	return (SceneObject*) line;
}

SceneObject * DDEngine::createTeapot(float size, bool wireframe)
{
	Teapot * teapot = new(GEOMETRY, STACK_ALLOCATOR) Teapot(size, wireframe);
	return (SceneObject*) teapot;
}

SceneObject * DDEngine::createLight(	bool enabled, bool directional,
										float ambientR, float ambientG, float ambientB,
										float diffuseR, float diffuseG, float diffuseB,
										float specularR, float specularG, float specularB)
{
	Light * light = new(GEOMETRY, STACK_ALLOCATOR) Light(	enabled, directional, 
															ambientR, ambientG, ambientB,
															diffuseR, diffuseG, diffuseB,
															specularR, specularG, specularB);
	return (SceneObject*) light;
}

SceneNode * DDEngine::addSceneNode(SceneNode * father, char * name, SceneObject * geometry, Vector position, Vector quaternionVector, float quaternionRotation, btRigidBody * physicGeometry)
{
	SceneNode * sceneNode = new(SCENEGRAPH, STACK_ALLOCATOR) SceneNode(father, name, geometry, position, quaternionVector, quaternionRotation, physicGeometry);
	return sceneNode;
}