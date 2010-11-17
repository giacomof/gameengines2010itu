#include "DDEngine.h"
					
static int const thread_delay = 1;				// Minimum time between loops
static float const PI = 3.14159f;				// PI definition
SDL_mutex * mutex_allocator;					// Mutex for Thread Synchronization

/* ********************************************* */
/* ******* operator new global overload ******** */
/* ******** just remember to use flags ********* */
/* ************* from globals.h **************** */
/* ********************************************* */

void * operator new(size_t size, unsigned short typeFlag, unsigned short allocatorFlag)
{
	AssetManager::lockMutex(mutex_allocator);

	cout << "NEW WITH FLAG: " << typeFlag <<  " AND USING ALLOCATOR: " << allocatorFlag << endl;

	void * storage = MemoryManager::allocate(size);
	if(NULL == storage) {
            throw "allocation fail : no free memory";
    }
	AssetManager::unlockMutex(mutex_allocator);

	return storage;
}

void operator delete(void * ptr) 
{
	free(ptr);
}

void operator delete(void * ptr, unsigned short flag) 
{
	cout << "DELETE WITH FLAG: " << flag << endl;
	MemoryManager::freeToLastMarker();
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

	int testsoundint = 500;
	while ( !engine->getController()->quit )
	{
		testsoundint++;
		if (testsoundint > 600)
		{
			soundPlayFile("assets/MENULOOP.WAV");
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
		// Runs the update method here

		// physics simulation
		dynamicsWorld->stepSimulation(1/120.f, 10);

		// Delay the thread to make room for others on the CPU
		SDL_Delay(thread_delay);
	}

	return 0;
}

DDEngine::DDEngine(int screenWidth, int screenHeight, int colorDepth, bool physics)
{
	screenW = screenWidth;							// Window Width
	screenH = screenHeight;							// Window Height
	screenCD = colorDepth;							// Color Depth

	hasPhysics = physics;
	if(hasPhysics)
		physicGravity = Vector(0, -10, 0);			// Gravity Vector

	// Create the asset manager
	assetManager = AssetManager();
	// Create the input pump
	InputPump = MessagePump();
	// Create the window manager
	WindowManager window = WindowManager();
	// Create the input abstraction layer
	controller = Controller();

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


void DDEngine::run(void)
{
	// Set up camera and spectator
	player = new(SCENEGRAPH, STACK_ALLOCATOR) entitySpectator();
	playercamera = new(SCENEGRAPH, STACK_ALLOCATOR) entityCamera();
	player->setCamera(playercamera);
	controller.setPlayerObject(player);

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
	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat diffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat shine = 100.0;
	glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT, ambient[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR, specular[0]);
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

			frameStarted( frameDelta );

			drawGL( frameDelta );

			frameEnded( frameDelta );
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
	glEnable(GL_TEXTURE_2D);
	// enables smooth shading (garaud)
	glShadeModel(GL_SMOOTH);
	// frontfacing culling
	glEnable(GL_CULL_FACE);
	// enables lighting
	glEnable(GL_LIGHTING);
	// Define material properties
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glEnable(GL_COLOR_MATERIAL);

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
	float *CamTransform = getCamera();

	if(drawDebug) dynamicsWorld->debugDrawWorld();

	// draw the animation
	//AssetManager::lockMutex( rootNodePtr->mutex_node );
	rootNode.update(frameDelta);
	//AssetManager::unlockMutex( rootNodePtr->mutex_node );

	//Root::drawGeometry();
	//AssetManager::lockMutex( rootNodePtr->mutex_node );
	rootNode.drawGeometry();
	//AssetManager::unlockMutex( rootNodePtr->mutex_node );

	// Swaps the buffers
	SDL_GL_SwapBuffers();
}

float* DDEngine::getCamera()
{		
	glMatrixMode(GL_MODELVIEW);

	entityCamera *currentCamera = controller.playerObject->getCamera();

	float tranM[16];

	Matrix::generateIdentityMatrix().getMatrix(&tranM[0]);

	if ( currentCamera != NULL )
	{
		AssetManager::lockMutex( currentCamera->mutex_object );
		Matrix transformationMatrix;

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

		AssetManager::unlockMutex( currentCamera->mutex_object );
	}

	glMultMatrixf(&tranM[0]);
	return &tranM[0];
}


btRigidBody * DDEngine::createPhysicalBox(Vector dimension, Vector position, Quaternion orientation, float mass, bool neverSleep)
{
	btCollisionShape* cubeShape = new btBoxShape(btVector3(dimension.getX(), dimension.getY(), dimension.getZ()));

	btDefaultMotionState* cubeMotionState;

	btScalar cubeMass = mass;
	btVector3 cubeInertia(0,0,0);
	cubeShape->calculateLocalInertia(cubeMass,cubeInertia);

	btRigidBody::btRigidBodyConstructionInfo * cubeRigidBodyCI;

	cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(orientation.getX(), orientation.getY(), orientation.getZ()),orientation.getW()),btVector3(position.getX(), position.getY(), position.getZ())));
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

	btDefaultMotionState* sphereMotionState;

	btScalar sphereMass = mass;
	btVector3 sphereInertia(0,0,0);
	sphereShape->calculateLocalInertia(sphereMass,sphereInertia);

	btRigidBody::btRigidBodyConstructionInfo * sphereRigidBodyCI;

	sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(orientation.getX(), orientation.getY(), orientation.getZ()),orientation.getW()),btVector3(position.getX(), position.getY(), position.getZ())));
	sphereRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(sphereMass,sphereMotionState,sphereShape,sphereInertia);

	btRigidBody* sphereRigidBody = new btRigidBody(*sphereRigidBodyCI);

	if(neverSleep)
		sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(sphereRigidBody);

	return sphereRigidBody;
}

btRigidBody * DDEngine::createRigidBody(btCollisionShape * collisionShape, Vector position, Quaternion orientation, float mass, bool neverSleep)
{
	btDefaultMotionState* shapeMotionState;

	btScalar shapeMass = mass;
	btVector3 shapeInertia(0,0,0);
	collisionShape->calculateLocalInertia(shapeMass,shapeInertia);

	btRigidBody::btRigidBodyConstructionInfo * shapeRigidBodyCI;

	shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(orientation.getX(), orientation.getY(), orientation.getZ()),orientation.getW()),btVector3(position.getX(), position.getY(), position.getZ())));
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
	md2Interface * md2Model = new md2Interface(model, texture);
	return (SceneObject*) md2Model;
}

SceneObject * DDEngine::createCollada(ColladaFile * model, unsigned int texture, ColladaSkeleton * skeleton)
{
	ColladaInterface * colladaModel = new ColladaInterface(model, texture, skeleton);
	return (SceneObject*) colladaModel;
}

SceneObject * DDEngine::createSphere(float radius, int slices, int stacks, bool wireframe)
{
	Sphere * sphere = new Sphere(radius, slices, stacks, wireframe);
	return (SceneObject*) sphere;
}

SceneObject * DDEngine::createPlane(float width, float height, int sideSubdivisions)
{
	SceneObject * plane = new Plane(width, height, sideSubdivisions);
	return  plane;
}

SceneObject * DDEngine::createCube(float side)
{
	Cube * cube = new Cube(side);
	return (SceneObject*) cube;
}

SceneObject * DDEngine::createLine(Vector start, Vector end)
{
	Line * line = new Line(start, end);
	return (SceneObject*) line;
}

SceneObject * DDEngine::createLight(	bool enabled, bool directional,
										float ambientR, float ambientG, float ambientB,
										float diffuseR, float diffuseG, float diffuseB,
										float specularR, float specularG, float specularB)
{
	Light * light = new Light(	enabled, directional, 
								ambientR, ambientG, ambientB,
								diffuseR, diffuseG, diffuseB,
								specularR, specularG, specularB);
	return (SceneObject*) light;
}

SceneNode * DDEngine::addSceneNode(SceneNode * father, char * name, SceneObject * geometry, Vector position, Vector quaternionVector, float quaternionRotation, btRigidBody * physicGeometry)
{
	SceneNode * sceneNode = new SceneNode(father, name, geometry, position, quaternionVector, quaternionRotation, physicGeometry);
	return sceneNode;
}