#include "render/SDLGraphicsProgram.hpp"


#include "render/Camera.hpp"
#include "render/Terrain.hpp"
#include "render/TextureLibrary.hpp"
#include "render/Skybox.hpp"
#include "render/GUI.hpp"
#include "render/GUINode.hpp"
#include "render/PlaceMarker.hpp"
#include "render/SevenSegmentDisplay.hpp"
#include "render/KeyHandler.hpp"

#include "simulation/World.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h){
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Final Project",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                w,
                                h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}

        // Little trick to map mouse to center of screen always.
        // Useful for handling 'mouselook'
        // This works because we effectively 're-center' our mouse at the start
        // of every frame prior to detecting any mouse motion.
        SDL_WarpMouseInWindow(m_window, w/2, h/2);
        SDL_SetRelativeMouseMode(SDL_TRUE);
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();


    // Setup our Renderer
    m_renderer = new Renderer(w,h);    
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(m_renderer!=nullptr){
        delete m_renderer;
    }


    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL(){
	//Success flag
	bool success = true;

	return success;
}



//Loops forever!
void SDLGraphicsProgram::Loop(){

    //Add textures to TextureLibrary
    TextureLibrary::GetInstance().LoadTexture("../common/textures/rock.ppm", "rock");
    TextureLibrary::GetInstance().LoadTexture("../common/textures/grass.ppm", "grass");
    TextureLibrary::GetInstance().LoadTexture("../common/textures/scoreboard.ppm", "scoreboard", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/crosshair.ppm", "crosshair");
    TextureLibrary::GetInstance().LoadTexture("../common/textures/onScreenPointer.ppm", "onPointer");
    TextureLibrary::GetInstance().LoadTexture("../common/textures/offScreenPointer.ppm", "offPointer");

    // Seven Segment Textures
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/zero.ppm", "zero", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/one.ppm", "one", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/two.ppm", "two", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/three.ppm", "three", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/four.ppm", "four", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/five.ppm", "five", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/six.ppm", "six", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/seven.ppm", "seven", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/eight.ppm", "eight", false);
    TextureLibrary::GetInstance().LoadTexture("../common/textures/seven-segment/nine.ppm", "nine", false);

    // Initialize world objects
    World world{};

    // Initialize Skybox
    Skybox* skybox = new Skybox("../common/textures/skybox/Rich_Mixed_Nebulae_right.ppm",
                                "../common/textures/skybox/Rich_Mixed_Nebulae_left.ppm",
                                "../common/textures/skybox/Rich_Mixed_Nebulae_down.ppm",
                                "../common/textures/skybox/Rich_Mixed_Nebulae_up.ppm",
                                "../common/textures/skybox/Rich_Mixed_Nebulae_front.ppm",
                                "../common/textures/skybox/Rich_Mixed_Nebulae_back.ppm");
    m_renderer->AddSkybox(skybox);

    //Initialize GUI
    GUI* gui = new GUI(m_renderer->Width(), m_renderer->Height());

    //crosshair
    gui->LoadQuadAndAddToScreen("crosshair", m_renderer->Width()/2, m_renderer->Height()/2, 10, 32, 32);

    gui->LoadQuadAndAddToScreen("scoreboard", 0, 0, 10, 460, 150);


    std::vector<std::string> digitTextureNames = {"zero", "one", "two", "three",
                                                    "four", "five", "six", "seven",
                                                    "eight", "nine"};
    glm::vec3 position = glm::vec3(173, 15, 1);//glm::vec3(287, 52, 1); // 574, 105
    glm::vec2 dimensions = glm::vec2(230, 76); //440 , 144
    SevenSegmentDisplay sevenSegDisplay = SevenSegmentDisplay(digitTextureNames, gui, position, dimensions, 4, 0);
    
    // PlaceMarker
    Object* test = new Object();
    test->MakeTexturedQuad("rock");

    PlaceMarker* marker = new PlaceMarker(test, "onPointer", "offPointer");
    gui->AddNode(marker); // add to GUI first to update width and height

    marker->UpdateTarget(world.GetNewTarget()); // Get first target

    float totalMSElapsedScoreUpdate = 0.0f;

    // Setup renderer
    m_renderer->AddGUI(gui);
    // Set Render root node to world Root
    m_renderer->setRoot(world.WorldRoot());
    // Set a default position for our camera
    m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f,0.0f,20.0f);


    PrintStartMessage();

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // For DeltaTime Physics Calculations
    Uint32 lastUpdate = SDL_GetTicks();

    // While application is running
    while(!quit){

        // Used for FPS capping
        Uint64 start = SDL_GetPerformanceCounter();


        // get seconds between last frame for physics calculations
        Uint32 current = SDL_GetTicks();        
        float deltaTime = (current - lastUpdate) / 1000.0f;
        lastUpdate = current;

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboard input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                // m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
                world.GetPlayer().MouseLook(mouseX, mouseY);
                m_renderer->GetCamera(0)->SetViewDirection(world.GetPlayer().ThrustDirection());
            }

            KeyHandler& handler = KeyHandler::GetInstance();
            handler.ProcessInputs(e);
            if(handler.KeyPressed(SDLK_w)) {
                world.GetPlayer().SpeedUp(deltaTime);
            }
            if(handler.KeyPressed(SDLK_s)) {
                world.GetPlayer().SlowDown(deltaTime);
            }
            if(handler.KeyPressed(SDLK_ESCAPE)) {
                std::cout << "Quitting..." << std::endl;
                quit = true;
            }

            // super speed
            bool enableOverdrive = handler.KeyPressed(SDLK_SPACE);
            world.GetPlayer().Overdrive(enableOverdrive);
            m_renderer->OverdriveFov(enableOverdrive);

        } // End SDL_PollEvent loop.

        // Update Camera from player movements
        m_renderer->GetCamera(0)->SetCameraEyePosition(world.GetPlayer().Location());
        skybox->UpdateLocation(world.GetPlayer().Location());

        // Update transforms of objects
        world.Update(deltaTime);

        // Update our scene through our renderer
        m_renderer->Update();
        // Render our scene using our selected renderer
        m_renderer->Render();


        glm::vec3 loc = world.GetPlayer().Location();
        // std::cout << "Player: " << loc.x << " " << loc.y << " " << loc.z << " - ";
        marker->UpdateWorldToScreenCoords(loc, m_renderer->ProjectionMatrix(), m_renderer->GetCamera(0));

        if(marker->PlayerReachedMarker()) {
            int score = marker->UpdateTarget(world.GetNewTarget());
            sevenSegDisplay.Increment(score);
        }

       
        

        // FPS calculation
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        totalMSElapsedScoreUpdate += elapsedMS;
        if(totalMSElapsedScoreUpdate > 10.0f) {

            sevenSegDisplay.Update();
            totalMSElapsedScoreUpdate = 0.0f;
        }

        float cappedFPS = 60.0f; // Target framerate of program
        float msPerFrame = 1000.0f / cappedFPS;

        // float currentRawFPS = 1.0f / (elapsedMS / 1000.0f);
        float msDelay = fmax(0.0f, floor(msPerFrame - elapsedMS));

        SDL_Delay(msDelay);

      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());

        glm::vec2 mousePosition = glm::vec2(m_renderer->Width()/2, m_renderer->Height()/2);
        SDL_WarpMouseInWindow(m_window, mousePosition.x, mousePosition.y);
        world.GetPlayer().OverridePreviousMousePosition(mousePosition);
	}
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void SDLGraphicsProgram::PrintStartMessage() {
    std::cout << "\nWelcome!"
              << "\nYour goal is to deliver packages to as many planets/moons as you can."
              << "\nThere is a marker on your screen that will point you to your destination."
              << "\nFly through the target to deliver the package and get your next assignment!"
              << "\nGood luck, pilot!"
              << "\n\n Controls:"
              << "\n[W] - Forward Thrust"
              << "\n[S] - Brakes"
              << "\n[Spacebar] - Overdrive (How to make the Kessel run in less than 12 parsecs!)"
              << "\n[Mouse] - Look around to steer the ship"
              << "\n[Esc] - Resign from the delivery service (Quit the game)"
              << std::endl;
}