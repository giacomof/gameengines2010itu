#include "DDEngine.h"
					
static int const thread_delay = 1;				// Minimum time between loops
static float const PI = 3.14159f;				// PI definition

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
		//dynamicsWorld->stepSimulation(1/120.f, 10);

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
	assetManagerPtr = AssetManager();
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
}

DDEngine::~DDEngine(void)
{
	if(hasPhysics)
	{
		dynamicsWorld->~btDiscreteDynamicsWorld();
		dispatcher->~btCollisionDispatcher();
	}
}


void DDEngine::run(void)
{
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
		
		// Actual frame rendering happens here
		if (window.getActive() )
		{
			renderClock.frameUpdate();

			//drawGL( frameDelta );
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
	broadphase = btDbvtBroadphase();
	// Collision Configuration
    collisionConfiguration = btDefaultCollisionConfiguration();
	// Collision Dispatcher
    dispatcher = new btCollisionDispatcher(&collisionConfiguration);
	// Constraint Solver
    solver = btSequentialImpulseConstraintSolver();
	// Initialise the physic world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, &broadphase, &solver, &collisionConfiguration);
	// Set gravity
	dynamicsWorld->setGravity(btVector3(physicGravity.getX(),physicGravity.getY(),physicGravity.getZ()));

	// Initialise the debugDrawer
	debugger.setDebugMode( btIDebugDraw::DBG_DrawWireframe ); 
	// Bind the debug drawer to the physic world
	dynamicsWorld->setDebugDrawer(&debugger); 

	return 1;
}

