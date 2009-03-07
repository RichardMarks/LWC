
// CODESTYLE: v2.0

// Engine.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: the game engine class is to be used as a singleton; without having a misbehaving static instance singleton pattern

/**
 * @file Engine.cpp
 * @brief Core Engine - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

#define PROJECT_WINDOW_CAPTION "CCPS Solutions Presents: LWC v2.3"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
	BitmapFont* Engine::GetDefaultBitmapFont() const
	{
		return defaultFont_;
	}
	
	////////////////////////////////////////////////////////////////////////////

	GameState* Engine::GetGameState() const
	{
		return gameState_;
	}

	////////////////////////////////////////////////////////////////////////////

	SDL_Surface* Engine::LoadImageResource(const char* filePath)
	{
		// attempt to pre-load the image
		SDL_Surface* preLoad = IMG_Load(filePath);

		// if it failed to load
		if (!preLoad)
		{
			// log the error
			WriteLog(stderr, "Failed to load ImageResource from \"%s\"!\n", filePath);

			// return null
			return 0;
		}

		// set the color key to "magic pink" RGB(255, 0, 255) HTML Color: #FF00FF
		//SDL_SetColorKey(preLoad, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(preLoad->format, 255, 0, 255));
		
		// Scrim's art uses alpha which gets turned into black, so we set the colorkey to black
		SDL_SetColorKey(preLoad, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(preLoad->format, 0, 0, 0));

		// convert the image to the proper format and set the image resource surface
		SDL_Surface* surface = SDL_DisplayFormat(preLoad);

		// free the pre-loaded surface
		Engine::UnloadImageResource(preLoad);

		// the conversion failed or we ran out of memory
		if (!surface)
		{
			// log the error
			WriteLog(stderr, "SDL_DisplayFormat() failed or we have run out of memory!\n");

			// return null
			return 0;
		}

		// return success
		return surface;
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Engine::UnloadImageResource(SDL_Surface* image)
	{
		if (image)
		{
			SDL_FreeSurface(image);
			
			// de-init the pointer
			image = 0;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Engine::Blit(SDL_Surface* source, SDL_Surface* target, int srcX, int srcY, int destX, int destY, int width, int height)
	{
		// we cannot blit a surface that does not exist!
		if (!source)
		{
			// log the error
			WriteLog(stderr, "Cannot blit a surface that has not been initialized!\n");

			// return
			return;
		}

		// we cannot blit to surface that does not exist!
		if (!target)
		{
			// log the error
			WriteLog(stderr, "Cannot blit to a surface that has not been initialized!\n");

			// return
			return;
		}

		// setup the blitting rects
		SDL_Rect sourceRect;
		SDL_Rect targetRect;
		
		sourceRect.x = srcX;
		sourceRect.y = srcY;
		sourceRect.w = width;
		sourceRect.h = height;
		
		targetRect.x = destX;
		targetRect.y = destY;
		
		// blit
		SDL_BlitSurface(source, &sourceRect, target, &targetRect);
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Engine::BlitSprite(SDL_Surface* source, SDL_Surface* target, int destX, int destY)
	{
		// we cannot blit a surface that does not exist!
		if (!source)
		{
			// log the error
			WriteLog(stderr, "Cannot blit a surface that has not been initialized!\n");

			// return
			return;
		}

		// we cannot blit to surface that does not exist!
		if (!target)
		{
			// log the error
			WriteLog(stderr, "Cannot blit to a surface that has not been initialized!\n");

			// return
			return;
		}

		// setup the blitting rect
		SDL_Rect targetRect;
		
		targetRect.x = destX;
		targetRect.y = destY;
		
		// blit
		SDL_BlitSurface(source, 0, target, &targetRect);
	}
	
	////////////////////////////////////////////////////////////////////////////

	Engine::Engine() :
		engineIsRunning_(false),
		screen_(0),
		mainScreen_(0),
		event_(0),
		artManager_(0),
		mapView_(0),
		gameState_(0),
		defaultFont_(0)
	{
	}

	////////////////////////////////////////////////////////////////////////////

	Engine::~Engine()
	{
	}

	////////////////////////////////////////////////////////////////////////////

	Engine* Engine::GetInstance()
	{
		return globalEngineInstance;
	}

	////////////////////////////////////////////////////////////////////////////

	bool Engine::Initialize(int args, char* argv[])
	{
		// initialize the external libraries
		if (!this->InitializeLibraries())
		{
			// return failure
			return false;
		}

		// initialize the screen
		if (!this->InitializeScreen())
		{
			// return failure
			return false;
		}
		
		// set the window caption
		SDL_WM_SetCaption(PROJECT_WINDOW_CAPTION, 0);

		// create the SDL event handler instance
		event_ = new SDL_Event;
		if (!event_)
		{
			// log the error
			WriteLog(stderr, "Unable to create event handler instance!\n");

			// return failure
			return false;
		}
		
		defaultFont_ = new BitmapFont();
		defaultFont_->Load("resources/fonts/font8x8white.png", 8, 8, 1);
		
		// create the art manager
		artManager_ = new ArtManager();
		mapView_ = new MapView(artManager_);
		gameState_ = new GameState();

		// start our engines ^-^
		engineIsRunning_ = true;
		
		gameState_->StartNewGame();

		// return success
		return true;
	}

	////////////////////////////////////////////////////////////////////////////

	bool Engine::InitializeLibraries()
	{
		// initialize SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			// log the error
			WriteLog(stderr, "SDL Library Initialization Failed!\n\tSDL Error: %s\n", SDL_GetError());

			// return failure
			return false;
		}

		// return success
		return true;
	}

	////////////////////////////////////////////////////////////////////////////

	bool Engine::InitializeScreen()
	{
		// initialize the screen
		mainScreen_ = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);
		
		if (!mainScreen_)
		{
			// log the error
			WriteLog(stderr, "SDL Screen Initialization Failed!\n\tSDL Error: %s\n", SDL_GetError());

			// return failure
			return false;
		}
		
		// create the game screen
		screen_ = SDL_CreateRGBSurface(SDL_SRCCOLORKEY, 300, 400, mainScreen_->format->BitsPerPixel, 0, 0, 0, 0);
		if (!screen_)
		{
			// log the error
			WriteLog(stderr, "SDL Game Screen Initialization Failed!\n\tSDL Error: %s\n", SDL_GetError());

			// return failure
			return false;
		}

		// return success
		return true;
	}

	////////////////////////////////////////////////////////////////////////////

	void Engine::Execute()
	{
		// we don't have a state stack processor system yet
		// so lets just get a basic while loop running for testing
		
		
		Position playerPosition(0, 0, 0); 
		char hudActionMessage[0x100];
		char compassMessage[0x32];
		bool requestUpdateDisplay = true;
		bool requestClearActionMessage = false;
		int actionMessageClearDelay = 50;
		int actionMessageClearCounter = actionMessageClearDelay;
		
				
		sprintf(hudActionMessage, "%s", "Starting Out...");
		
		int actionMessageX = ((screen_->w / 2) - ((strlen(hudActionMessage) * 9) / 2));
		
		playerPosition.Copy(gameState_->GetPlayerPosition());
		
		int playerX = playerPosition.x_;
		int playerZ = playerPosition.y_;
		int compass = playerPosition.facing_;
		
		sprintf(compassMessage, "You are facing %s.",
			(0x0 == playerPosition.facing_) ? "North" :
			(0x1 == playerPosition.facing_) ? "East" :
			(0x2 == playerPosition.facing_) ? "South" :
			(0x3 == playerPosition.facing_) ? "West" : "<Invalid Direction>");

		int gameScreenX = 40;
		int gameScreenY = mainScreen_->h / 2 - screen_->h / 2;
		
		
		SDL_Surface* mainScreenOverlay = Engine::LoadImageResource("resources/overlays/mainscreen.png");
		
		if (!mainScreenOverlay)
		{
			this->Stop();
			return;
		}
		
		// small compass overlay images
		SDL_Surface** smallCompassOverlay = new SDL_Surface* [4];
		
		smallCompassOverlay[0] = Engine::LoadImageResource("resources/overlays/sm_compass_n.png");
		smallCompassOverlay[1] = Engine::LoadImageResource("resources/overlays/sm_compass_e.png");
		smallCompassOverlay[2] = Engine::LoadImageResource("resources/overlays/sm_compass_s.png");
		smallCompassOverlay[3] = Engine::LoadImageResource("resources/overlays/sm_compass_w.png");


		

		// better input handling
		const int MOTIONBUTTON_UP 			= 0x0;
		const int MOTIONBUTTON_DOWN 		= 0x1;
		const int MOTIONBUTTON_STRAFELEFT 	= 0x2;
		const int MOTIONBUTTON_STRAFERIGHT 	= 0x3;
		bool motionButtonDown[4] = { false, false, false, false };
		
		// slow the fucking player down!
		int playerMotionDelay = 10;
		int playerMotionCounter = playerMotionDelay;
		
		
		// a minimap
		MiniMap miniMap(gameState_->GetCurrentMap(), 140, 140);

		// while the engine is running
		while(engineIsRunning_)
		{
			// process the events
			while(SDL_PollEvent(event_))
			{
				switch(event_->type)
				{
					// the window was closed
					case SDL_QUIT:
					{
						// stop the engine
						this->Stop();
					} break;

					// a key was pressed
					case SDL_KEYDOWN:
					{
						// what key is down
						switch(event_->key.keysym.sym)
						{
							case SDLK_ESCAPE:
							{
								// stop the engine
								this->Stop();
							} break;
							
							case 'w':
							case 'W':
							case SDLK_UP:
							{
								motionButtonDown[MOTIONBUTTON_UP] = true;
							} break;
							
							case 's':
							case 'S':
							case SDLK_DOWN:
							{
								motionButtonDown[MOTIONBUTTON_DOWN] = true;
							} break;
							
							case 'q':
							case 'Q':
							case SDLK_COMMA:
							case SDLK_LESS:
							{
								motionButtonDown[MOTIONBUTTON_STRAFELEFT] = true;
							} break;
							
							case 'e':
							case 'E':
							case SDLK_PERIOD:
							case SDLK_GREATER:
							{
								motionButtonDown[MOTIONBUTTON_STRAFERIGHT] = true;
							} break;
							
							default: break;
						} // end switch
					} break;
					
					// a key was released
					case SDL_KEYUP:
					{
						// what key is up
						switch(event_->key.keysym.sym)
						{
							case 'w':
							case 'W':
							case SDLK_UP:
							{
								motionButtonDown[MOTIONBUTTON_UP] = false;
							} break;
							
							case 's':
							case 'S':
							case SDLK_DOWN:
							{
								motionButtonDown[MOTIONBUTTON_DOWN] = false;
							} break;
							
							case 'q':
							case 'Q':
							case SDLK_COMMA:
							case SDLK_LESS:
							{
								motionButtonDown[MOTIONBUTTON_STRAFELEFT] = false;
							} break;
							
							case 'e':
							case 'E':
							case SDLK_PERIOD:
							case SDLK_GREATER:
							{
								motionButtonDown[MOTIONBUTTON_STRAFERIGHT] = false;
							} break;
							
							case 'a':
							case 'A':
							case SDLK_LEFT:
							{
								gameState_->TurnPlayerLeft();
								sprintf(hudActionMessage, "%s", "Turned Left...");
								requestUpdateDisplay = true;
							} break;
							
							case 'd':
							case 'D':
							case SDLK_RIGHT:
							{
								gameState_->TurnPlayerRight();
								sprintf(hudActionMessage, "%s", "Turned Right...");
								requestUpdateDisplay = true;
							} break;
							default: break;
						}
					} break;

					default: break;
				} // end switch
			} // end while
			
////////////////////////////////////////////////////////////////////////////////
// *************************** NEW PLAYER MOTION **************************** //
////////////////////////////////////////////////////////////////////////////////
			
			// are we moving forward?
			if (motionButtonDown[MOTIONBUTTON_UP])
			{
				if (--playerMotionCounter <= 0)
				{
					playerMotionCounter = playerMotionDelay;
					
					if (gameState_->MovePlayerForward())
					{
						sprintf(hudActionMessage, "%s", "Moved Forward...");
						requestUpdateDisplay = true;
					}
					else
					{
						sprintf(hudActionMessage, "%s", "That way is blocked!");
						requestUpdateDisplay = true;
					}
				}
			}
			
			// are we moving back?
			if (motionButtonDown[MOTIONBUTTON_DOWN])
			{
				if (--playerMotionCounter <= 0)
				{
					playerMotionCounter = playerMotionDelay;
					
					if (gameState_->MovePlayerBack())
					{
						sprintf(hudActionMessage, "%s", "Moved Back...");
						requestUpdateDisplay = true;
					}
					else
					{
						sprintf(hudActionMessage, "%s", "That way is blocked!");
						requestUpdateDisplay = true;
					}
				}
			}
			
			// are we strafing left?
			if (motionButtonDown[MOTIONBUTTON_STRAFELEFT])
			{
				if (--playerMotionCounter <= 0)
				{
					playerMotionCounter = playerMotionDelay;
					
					if (gameState_->MovePlayerLeft())
					{
						sprintf(hudActionMessage, "%s", "Stepped Left...");
						requestUpdateDisplay = true;
					}
					else
					{
						sprintf(hudActionMessage, "%s", "That way is blocked!");
						requestUpdateDisplay = true;
					}
				}
			}
			
			// are we strafing right?
			if (motionButtonDown[MOTIONBUTTON_STRAFERIGHT])
			{
				if (--playerMotionCounter <= 0)
				{
					playerMotionCounter = playerMotionDelay;
					
					if (gameState_->MovePlayerRight())
					{
						sprintf(hudActionMessage, "%s", "Stepped Right...");
						requestUpdateDisplay = true;
					}
					else
					{
						sprintf(hudActionMessage, "%s", "That way is blocked!");
						requestUpdateDisplay = true;
					}
				}
			}

			// is is time to request the action message be cleared?
			if (--actionMessageClearCounter <= 0)
			{
				actionMessageClearCounter = actionMessageClearDelay;
				requestClearActionMessage = true;
			}
			
			// should we clear the action message ?
			if (requestClearActionMessage)
			{
				sprintf(hudActionMessage, "%s", "Waiting...");
				requestClearActionMessage = false;
				requestUpdateDisplay = true;
			}
			
			// should we update the display?
			if (requestUpdateDisplay)
			{
				actionMessageX = ((screen_->w / 2) - ((strlen(hudActionMessage) * 9) / 2));
	
				playerPosition.Copy(gameState_->GetPlayerPosition());
	
				playerX = playerPosition.x_;
				playerZ = playerPosition.y_;
				compass = playerPosition.facing_;
	
				sprintf(compassMessage, "You are facing %s.",
					(0x0 == playerPosition.facing_) ? "North" :
					(0x1 == playerPosition.facing_) ? "East" :
					(0x2 == playerPosition.facing_) ? "South" :
					(0x3 == playerPosition.facing_) ? "West" : "<Invalid Direction>");
					
				
				mapView_->RenderMap(screen_, gameState_->GetCurrentMap(), gameState_->GetPlayerPosition());
				
				defaultFont_->Print(screen_, actionMessageX, 8, "%s", hudActionMessage);
				defaultFont_->Print(screen_, 8, screen_->h - 34, "Player X: %2d", playerX);
				defaultFont_->Print(screen_, 8, screen_->h - 25, "Player Z: %2d", playerZ);
				defaultFont_->Print(screen_, 8, screen_->h - 16, "%s", compassMessage);
			
				// blit the game screen onto the main screen
				Engine::BlitSprite(screen_, mainScreen_, gameScreenX, gameScreenY);
			
				// blit the overlays
				Engine::BlitSprite(mainScreenOverlay, mainScreen_, 0, 0);
				
				Engine::BlitSprite(smallCompassOverlay[compass], mainScreen_, 42, 42);
				
				// blit the minimap
				miniMap.Update();
				miniMap.Render(mainScreen_, 390, 290);
				
				requestUpdateDisplay = false;
			}
			
			
			
			// flip the screen
			this->FlipScreen();
			
			// reduce the cpu hoggingness of SDL ^-^
			SDL_Delay(20);
		} // end while
		
		Engine::UnloadImageResource(smallCompassOverlay[0]);
		Engine::UnloadImageResource(smallCompassOverlay[1]);
		Engine::UnloadImageResource(smallCompassOverlay[2]);
		Engine::UnloadImageResource(smallCompassOverlay[3]);
		delete [] smallCompassOverlay;
		
		Engine::UnloadImageResource(mainScreenOverlay);
		
		
	}

	////////////////////////////////////////////////////////////////////////////

	void Engine::Destroy()
	{
		#define _TMP_DELOBJ(object) if (object) { delete object; object = 0; }

		_TMP_DELOBJ(event_)
		_TMP_DELOBJ(artManager_)
		_TMP_DELOBJ(mapView_)
		_TMP_DELOBJ(gameState_)
		_TMP_DELOBJ(defaultFont_)

		#undef _TMP_DELOBJ
		
		// unload the game screen
		Engine::UnloadImageResource(screen_);
	}

	////////////////////////////////////////////////////////////////////////////

	void Engine::Stop()
	{
		// log the stop
		WriteLog(stderr, "Engine Stopped.\n");

		// stop the engine
		engineIsRunning_ = false;
	}

	////////////////////////////////////////////////////////////////////////////

	SDL_Surface* Engine::GetScreen() const
	{
		return screen_;
	}

	////////////////////////////////////////////////////////////////////////////

	void Engine::FlipScreen()
	{
		SDL_Flip(mainScreen_);
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Engine::ClearScreen(unsigned int color)
	{
		SDL_FillRect(mainScreen_, 0, color);
	}
	
} // end namespace

