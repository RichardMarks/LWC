
// CODESTYLE: v2.0

// main.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: Project Program Entry Point

/**
 * @file main.cpp
 * @brief Project Program Entry Point
 * @author Richard Marks <ccpsceo@gmail.com>
 */

// C STANDARD LIBRARY
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <ctime>
#include <cmath>
#include <cctype>

// STL
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

////////////////////////////////////////////////////////////////////////////////

// error log handling
#if !defined(CONFIG_DISABLE_LOGGING)
	#define WriteLog fprintf
#else
	void DisabledStdErrLogFunction(FILE* fp, const char* format, ...){}
	#define WriteLog DisabledStdErrLogFunction
#endif

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
	/**
	 * @class WallSpriteSet
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.WallSpriteSet
	 */
	class WallSpriteSet
	{
	public:
		WallSpriteSet(const char* rootPath);
		SDL_Surface* GetFrontImage(int range);
		SDL_Surface* GetLeftImage(int range);
		SDL_Surface* GetRightImage(int range);
		~WallSpriteSet();
	private:
		SDL_Surface** frontImages_;
		SDL_Surface** leftImages_;
		SDL_Surface** rightImages_;
		int visibleDepth_;
	}; // end class
	
	////////////////////////////////////////////////////////////////////////////

	/**
	 * @class ArtManager
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.ArtManager
	 */
	class ArtManager
	{
	public:
		ArtManager();
		~ArtManager();
		bool DidArtLoadSuccessfully() const;
		WallSpriteSet* GetWallSetNumber(unsigned int which);
		float GetXOffsetCenter(int range, int offset);
		float GetXOffsetLeft(int range, int offset);
		float GetXOffsetRight(int range, int offset);
		float GetYOffsetCenter(int range, int offset);
		float GetYOffsetLeft(int range, int offset);
		float GetYOffsetRight(int range, int offset);
		bool LoadArt();
	private:
		bool allArtLoadedSuccessfully_;
		std::vector<WallSpriteSet*> allWallSprites_;
	}; // end class

	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * @class Position
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.Position
	 */
	class Position
	{
	public:
		Position(int x, int y, int facing);
		Position* GetLeftFacingOfThis();
		Position* GetRightFacingOfThis();
		Position* GetPositionLeftOfThis(int steps);
		Position* GetPositionRightOfThis(int steps);
		Position* GetPositionAheadOfThis(int steps);
		Position* GetPositionBehindThis(int steps);
		void Copy(Position* source);
		int x_;
		int y_;
		int facing_;
	}; // end class
	
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * @class Map
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.Map
	 */
	class Map
	{
	public:
		Map();
		~Map();
		
		void MakeMockup();
		int GetWallForCoordinate(Position* position);
		bool CanPassWallForCoordinate(Position* position);
		Position* GetStartingPoint(int which);
		void ClearMap();
	private:
		void MakeFirstMockup();
		void MakeNormalWall(Position* position, int wallID);
		int width_;
		int height_;
		int*** walls_;
		bool*** passibility_;
		Position** startingPoints_;
	}; // end class 
	
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * @class GameState
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.GameState
	 *
	 * Updated by Richard Marks to include strafing and condition checking
	 */
	class GameState
	{
	public:
		GameState();
		~GameState();
		void StartNewGame();
		
		bool MovePlayerForward();
		bool MovePlayerBack();
		
		// new strafing motion commands
		bool MovePlayerLeft();
		bool MovePlayerRight();
		
		void TurnPlayerLeft();
		void TurnPlayerRight();
		
		Map* GetCurrentMap() const;
		Position* GetPlayerPosition() const;
		
	private:
		Map* currentMap_;
		Position* playerPosition_;
	}; // end class
	
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * @class MapView
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.MapView
	 */
	class MapView
	{
	public:
		MapView(ArtManager* artManager);
		~MapView();		
		void RenderMap(SDL_Surface* target, Map* currentMap, Position* currentPosition);	
	private:
		void DrawSky(SDL_Surface* target, Map* currentMap, Position* currentPosition);
		void DrawGround(SDL_Surface* target, Map* currentMap, Position* currentPosition);
		void DrawDistRank(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset);
		void DrawLeftWall(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset);
		void DrawRightWall(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset);
		void DrawStraightWall(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset);

		ArtManager* artManager_;
		int viewWidth_;
		int viewHeight_;
	}; // end class
		
	////////////////////////////////////////////////////////////////////////////
	
	/// the maximum length for a string to be printed is 1024 characters
	const unsigned int BITFNT_MAX_STRING_LENGTH = 0x400;
	
	/**
	 * @class BitmapFont
	 * @brief a direct SDL port of my ged101 BitmapFont class
	 */
	class BitmapFont
	{
	public:
		/**
		 * default constructor
		 * Initializes the font structure with the default font from an x86 BIOS for mode 0x13\n
		 * An 8x8 font containing all 256 characters 128 ASCII and 128 extended ASCII characters
		 */
		BitmapFont();
		
		/**
		 * default destructor - calls BitmapFont::Destroy() to de-allocate memory
		 */
		~BitmapFont();
		
		/**
		 * Initializes the font structure by loading the font image from a file.
		 * @param filePath is the path to the file that holds the font image data to be loaded
		 * @param letterWidth is the width of a single letter in pixels
		 * @param letterHeight is the height of a single letter in pixels
		 * @param spacing is the spacing between the letters in pixels
		 * @return true on success, false on failure
		 */
		bool Load(const char* filePath, int letterWidth = 8, int letterHeight = 8, int spacing = 2);
		
		/**
		 * Initializes the font structure using a pre-loaded SDL_Surface.
		 * @param source is a pointer to the SDL_Surface structure that holds the font image data.
		 * @param letterWidth is the width of a single letter in pixels
		 * @param letterHeight is the height of a single letter in pixels
		 * @param spacing is the spacing between the letters in pixels
		 */
		void Create(SDL_Surface* source, int letterWidth = 8, int letterHeight = 8, int spacing = 2);
		
		/**
		 * Prints @a text on the @a destination SDL_Surface at the position @a x, @a y.
		 * @param destination is the SDL_Surface to print the text on to.
		 * @param x is the X coordinate of the upper-left corner of the first letter of the string to be printed in pixels.
		 * @param y is the Y coordinate of the upper-left corner of the first letter of the string to be printed in pixels.
		 * @param text is a printf-style C-string holding the text to be printed.
		 */
		void Print(SDL_Surface* destination, int x, int y, const char* text, ...);
		
		/**
		 * @return the width of a single letter in pixels
		 */
		int GetLetterWidth() const;
		
		/**
		 * @return the height of a single letter in pixels
		 */
		int GetLetterHeight() const;
		
		/**
		 * @return the amount of pixels of spacing between letters
		 */
		int GetLetterSpacing() const;
		
		/**
		 * @return a pointer to the SDL_Surface structure that holds the font image data
		 */
		SDL_Surface* GetFontImage() const;
		
	private:
		/**
		 * De-allocates any allocated memory
		 */
		void Destroy();
		
		/**
		 * @var fontImage_
		 * @brief the SDL_Surface structure that holds the font image data
		 */
		SDL_Surface* fontImage_;
		
		/**
		 * @var letterWidth_
		 * @brief the width of a single letter in pixels
		 */
		int letterWidth_;
		
		/**
		 * @var letterHeight_
		 * @brief the height of a single letter in pixels
		 */
		int letterHeight_;
		
		/**
		 * @var spacing_
		 * @brief the amount of pixels of spacing between letters
		 */
		int spacing_;
	}; // end class
	
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////

	/**
	 * @class Engine
	 * @brief the game engine class is to be used as a singleton; without having a misbehaving static instance singleton pattern
	 */
	class Engine
	{
	public:
		/// constructor
		Engine();

		/// destructor
		~Engine();

		/**
		 * @brief gets the engine class singleton pointer
		 * @return a pointer to the globalEngineInstance singleton that gets init in main()
		 */
		static Engine* GetInstance();

		/**
		 * @brief initializes the engine
		 * @return true on success, and false on failure of intialization of the engine
		 */
		bool Initialize(int args, char* argv[]);

		/// the engine's main loop 
		void Execute();

		/// called by main() - handles any shut-down procedures that need to be handled
		void Destroy();

		/// stops the engine main loop
		void Stop();

		/// gets a pointer to the screen surface
		SDL_Surface* GetScreen() const;
		
		/// gets the bitmap font
		BitmapFont* GetDefaultBitmapFont() const;

		/// clears the screen to a color
		void ClearScreen(unsigned int color = 0);

		/// flips the screen surface
		void FlipScreen();
		
		/// loads an image file
		static SDL_Surface* LoadImageResource(const char* filePath);
		
		/// unloads (frees) an image
		static void UnloadImageResource(SDL_Surface* image);
		
		/// blits a portion of the source surface to the target surface
		static void Blit(SDL_Surface* source, SDL_Surface* target, int srcX, int srcY, int destX, int destY, int width, int height);
		
		/// blits the entire source surface to the target surface
		static void BlitSprite(SDL_Surface* source, SDL_Surface* target, int destX, int destY);

	private:

		/**
		 * @brief initializes the external libraries that the engine depends on
		 * @return true on success, and false on failure of intialization of the libraries
		 */
		bool InitializeLibraries();

		/**
		 * @brief initialize the screen
		 * @return true on success, and false on failure of intialization of the screen
		 */
		bool InitializeScreen();

		/// hidden copy constructor
		Engine(const Engine& rhs);

		/// hidden assignment operator
		const Engine& operator=(const Engine& rhs);

	private:

		/// is the engine running
		bool engineIsRunning_;

		/// the SDL screen surface that serves as the game screen
		SDL_Surface* screen_;
		
		/// the small SDL surface that serves as the main screen
		SDL_Surface* mainScreen_;
		
		/// the SDL event data
		SDL_Event* event_;
		
		/// the art manager
		ArtManager* artManager_;
		
		// the map view
		MapView* mapView_;
		
		// the game state
		GameState* gameState_;
		
		// the default font
		BitmapFont* defaultFont_;

	}; // end class
	
	extern Engine* globalEngineInstance;
	
	////////////////////////////////////////////////////////////////////////////

	BitmapFont* Engine::GetDefaultBitmapFont() const
	{
		return defaultFont_;
	}

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
		SDL_WM_SetCaption("LofiWanderings in C++", 0);

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


		// better input handling
		const int MOTIONBUTTON_UP 			= 0x0;
		const int MOTIONBUTTON_DOWN 		= 0x1;
		const int MOTIONBUTTON_STRAFELEFT 	= 0x2;
		const int MOTIONBUTTON_STRAFERIGHT 	= 0x3;
		bool motionButtonDown[4] = { false, false, false, false };
		
		// slow the fucking player down!
		int playerMotionDelay = 10;
		int playerMotionCounter = playerMotionDelay;

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
			
				// blit the overlay
				Engine::BlitSprite(mainScreenOverlay, mainScreen_, 0, 0);
				
				requestUpdateDisplay = false;
			}
			
			
			
			// flip the screen
			this->FlipScreen();
			
			// reduce the cpu hoggingness of SDL ^-^
			SDL_Delay(20);
		} // end while
		
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
	
	////////////////////////////////////////////////////////////////////////////

	
	BitmapFont::BitmapFont() :
		fontImage_(0),
		letterWidth_(8),
		letterHeight_(8),
		spacing_(1)
	{
		// default font
		const int defaultFont[] =
		{
			0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,
			0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,0,
			1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,
			1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,1,0,0,1,1,1,1,1,1,0,0,
			0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,
			0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,
			0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,1,1,1,1,0,0,
			0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
			0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
			0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
			1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,
			1,0,1,0,0,1,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,0,1,1,0,
			0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,
			1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
			1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,
			1,0,1,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,
			1,1,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,
			1,0,0,1,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,1,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,0,0,0,0,0,1,1,0,0,
			0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,
			0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
			1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,
			1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,
			0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,
			1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,
			1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
			1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,
			1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,
			1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,
			1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,
			0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,
			0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
			1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,
			1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,
			0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,
			1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,
			0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,
			0,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,
			0,0,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,0,
			0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,
			0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,
			0,0,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,1,1,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,
			1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,
			0,0,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,1,1,
			0,1,1,1,1,1,0,0,1,1,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,
			0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,
			1,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,
			0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,
			0,1,1,1,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,
			0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,
			0,0,1,1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,1,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,
			0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,0,1,1,0,0,0,0,1,1,0,0,0,
			0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
			0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
			0,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,
			0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,
			0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,
			1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,
			0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,
			1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,
			0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,
			1,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,
			0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,0,1,1,0,
			0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,
			0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
			0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
			0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,
			0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,
			0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,
			0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,1,1,1,1,1,1,0,0,
			1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,
			0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,
			0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,0,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,
			0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,
			0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,1,0,1,1,0,0,1,1,1,0,0,1,0,
			0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1,1,0,
			0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,1,1,0,0,1,0,0,1,1,1,0,0,
			0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,
			1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,0,1,1,0,
			1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,0,
			0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,1,1,1,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,
			0,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,
			0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,
			1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,
			1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,
			0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,1,1,0,1,1,0,0,
			1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,
			0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,
			1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,0,0,
			0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,
			0,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,
			0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,
			0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,
			1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,0,0,
			1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,
			0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,
			0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,
			1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,0,0,
			1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,0,1,1,0,0,0,1,1,1,1,1,1,0,0,
			0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,
			1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,
			0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,
			0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,
			0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,
			0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,0,0,0,0,0,
			0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,
			1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,
			0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,0,1,1,0,0,
			1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,
			0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,
			1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,0,0,0,0,1,1,1,0,0,
			0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,
			0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,
			0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,
			0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,
			0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,
			1,1,0,0,0,1,1,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,
			0,1,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,
			0,0,1,1,1,0,0,0,0,1,1,0,0,1,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,1,0,0,
			0,1,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,
			0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,
			0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,
			0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,
			0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,
			0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
			1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
			0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,
			0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,
			0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,
			0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,
			0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,0,
			0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,1,1,1,1,1,0,0,
			0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,
			0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,0,0,0,0,
			0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,
			1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
			0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,
			0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,
			0,1,1,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,1,1,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,
			0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,
			0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,0,0,1,1,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,
			1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
			0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
			0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
			0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,1,1,0,0,
			0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,
			0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,
			1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,
			1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			0,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,
			1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
			1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
		};
		
		fontImage_ = SDL_CreateRGBSurface(
			SDL_SRCCOLORKEY, 
			128, 128, globalEngineInstance->GetScreen()->format->BitsPerPixel, 0, 0, 0, 0);
		SDL_SetColorKey(fontImage_, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(fontImage_->format, 255, 0, 255));
		SDL_FillRect(fontImage_, 0, SDL_MapRGB(fontImage_->format, 255, 0, 255));
		
		if (SDL_MUSTLOCK(fontImage_))
		{
			SDL_LockSurface(fontImage_);
		}
		
		unsigned int whitePixel = SDL_MapRGB(fontImage_->format, 255, 255, 255);
		
		switch(fontImage_->format->BytesPerPixel)
		{
			// 8 bits per pixel
			case 1: 
			{
				for (int y = 0; y < 128; y++)
				{
					for (int x = 0; x < 128; x++)
					{
						if (0 != defaultFont[x + (y * 128)])
						{
							unsigned char* imageDataBuffer = 
								static_cast<unsigned char*>(
									static_cast<unsigned char*>(fontImage_->pixels) + y * fontImage_->pitch + x);
							*imageDataBuffer = whitePixel;
						}
					}
				}
			} break;
			
			// 15 or 16 bits per pixel 
			case 2: 
			{
				unsigned int pitchOver2 = fontImage_->pitch / 2;
				for (int y = 0; y < 128; y++)
				{
					for (int x = 0; x < 128; x++)
					{
						if (0 != defaultFont[x + (y * 128)])
						{
							unsigned short* imageDataBuffer = 
								static_cast<unsigned short*>(
									static_cast<unsigned short*>(fontImage_->pixels) + y * pitchOver2 + x);
							*imageDataBuffer = whitePixel;
						}
					}
				}
			} break;
			
			// 24 bits per pixel
			case 3: 
			{
				if (SDL_LIL_ENDIAN == SDL_BYTEORDER)
				{
					for (int y = 0; y < 128; y++)
					{
						for (int x = 0; x < 128; x++)
						{
							if (0 != defaultFont[x + (y * 128)])
							{
								unsigned char* imageDataBuffer = 
									static_cast<unsigned char*>(
										static_cast<unsigned char*>(fontImage_->pixels) + y * fontImage_->pitch + x * 3);
							
								imageDataBuffer[0] = whitePixel;
								imageDataBuffer[1] = whitePixel >> 8;
								imageDataBuffer[2] = whitePixel >> 16;
							}
						}
					}
				}
				else
				{
					for (int y = 0; y < 128; y++)
					{
						for (int x = 0; x < 128; x++)
						{
							if (0 != defaultFont[x + (y * 128)])
							{
								unsigned char* imageDataBuffer = 
									static_cast<unsigned char*>(
										static_cast<unsigned char*>(fontImage_->pixels) + y * fontImage_->pitch + x * 3);
							
								imageDataBuffer[2] = whitePixel;
								imageDataBuffer[1] = whitePixel >> 8;
								imageDataBuffer[0] = whitePixel >> 16;
							}
						}
					}
				}
			} break;
			
			// 32 bits per pixel
			case 4: 
			{
				unsigned int pitchOver4 = fontImage_->pitch / 4;
				for (int y = 0; y < 128; y++)
				{
					for (int x = 0; x < 128; x++)
					{
						if (0 != defaultFont[x + (y * 128)])
						{
							unsigned int* imageDataBuffer = 
								static_cast<unsigned int*>(
									static_cast<unsigned int*>(fontImage_->pixels) + y * pitchOver4 + x);
							*imageDataBuffer = whitePixel;
						}
					}
				}
			} break;
			default: break;
		}
		
		if (SDL_MUSTLOCK(fontImage_))
		{
			SDL_UnlockSurface(fontImage_);
		}
		
	} // end constructor

	////////////////////////////////////////////////////////////////////////////

	BitmapFont::~BitmapFont()
	{
		Destroy();
	} // end destructor

	////////////////////////////////////////////////////////////////////////////
	
	bool BitmapFont::Load(const char* filePath, int letterWidth, int letterHeight, int spacing)
	{
		Destroy();
		
		fontImage_ = Engine::LoadImageResource(filePath);
		
		if (!fontImage_)
		{
			return false;
		}
		
		letterWidth_ 	= letterWidth;
		letterHeight_ 	= letterHeight;
		spacing_ 		= spacing;
		return true;
	}
	
	////////////////////////////////////////////////////////////////////////////

	void BitmapFont::Create(SDL_Surface* source, int letterWidth, int letterHeight, int spacing)
	{
		Destroy();
		
		fontImage_ = SDL_CreateRGBSurface(
			SDL_SRCCOLORKEY, 
			source->w, source->h,
			globalEngineInstance->GetScreen()->format->BitsPerPixel, 
			0, 0, 0, 0);
		
		SDL_SetColorKey(fontImage_, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(fontImage_->format, 255, 0, 255));
		
		Engine::BlitSprite(source, fontImage_, 0, 0);
		
		letterWidth_ 	= letterWidth;
		letterHeight_ 	= letterHeight;
		spacing_ 		= spacing;
	}

	////////////////////////////////////////////////////////////////////////////

	void BitmapFont::Print(SDL_Surface* destination, int x, int y, const char* text, ...)
	{
		if (!fontImage_)
		{
			return;
		}
		
		char textBuffer[BITFNT_MAX_STRING_LENGTH];
		va_list va;
		va_start(va, text);
		vsnprintf(textBuffer, BITFNT_MAX_STRING_LENGTH - 1, text, va);
		va_end(va);
		
		unsigned int textLength = strlen(textBuffer);
		
		// save the origin, and a drawing cursor location
		int originX = x;
		int cursorX = x;
		int cursorY = y;
		
		// default tab size is 8
		int tabSize = (8 * letterWidth_) + (7 * spacing_);
		
		for (unsigned int index = 0; index < textLength; index++)
		{
			switch(textBuffer[index])
			{
				case '\n':
				{
					// handle the new line
					cursorX = originX;
					cursorY += letterHeight_;
					cursorY += spacing_;
				} break;
				
				case '\t':
				{
					// handle the tab
					cursorX += tabSize;
				} break;
				
				default:
				{
					// any other character just gets printed
					
					// get the letter
					int letter = textBuffer[index];
					
					// find the position of the letter
					int letterX = (letter / 16) * letterWidth_;
					int letterY = (letter % 16) * letterHeight_;
					
					// blit the damn thing!
					Engine::Blit(fontImage_, destination, letterX, letterY, cursorX, cursorY, letterWidth_, letterHeight_);
					
					// advance cursor position
					cursorX += letterWidth_;
					cursorX += spacing_;
				} break;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	int BitmapFont::GetLetterWidth() const
	{
		return letterWidth_;
	}

	////////////////////////////////////////////////////////////////////////////

	int BitmapFont::GetLetterHeight() const
	{
		return letterHeight_;
	}
	
	////////////////////////////////////////////////////////////////////////////

	int BitmapFont::GetLetterSpacing() const
	{
		return spacing_;
	}
	
	////////////////////////////////////////////////////////////////////////////

	SDL_Surface* BitmapFont::GetFontImage() const
	{
		return fontImage_;
	}
	
	////////////////////////////////////////////////////////////////////////////

	void BitmapFont::Destroy()
	{
		if (0 != fontImage_)
		{
			Engine::UnloadImageResource(fontImage_);
		}
	}
	
	////////////////////////////////////////////////////////////////////////////

	WallSpriteSet::WallSpriteSet(const char* rootPath)
	{
		visibleDepth_ 	= 0x3;
		frontImages_ 	= new SDL_Surface* [visibleDepth_];
		leftImages_ 	= new SDL_Surface* [visibleDepth_];
		rightImages_ 	= new SDL_Surface* [visibleDepth_];
		
		for (int index = 0; index < visibleDepth_; index++)
		{
			char buffer[0x100];
			
			sprintf(buffer, "%sf%d.png", rootPath, index);
			frontImages_[index] = Engine::LoadImageResource(buffer);
			
			sprintf(buffer, "%sl%d.png", rootPath, index);
			leftImages_[index] = Engine::LoadImageResource(buffer);
			
			sprintf(buffer, "%sr%d.png", rootPath, index);
			rightImages_[index] = Engine::LoadImageResource(buffer);
		}
	}
	
	////////////////////////////////////////////////////////////////////////////

	WallSpriteSet::~WallSpriteSet()
	{
		// free images
		#define _TMP_DELOBJ(object) if (object[index]) { Engine::UnloadImageResource(object[index]); }
		for (int index = 0; index < visibleDepth_; index++)
		{
			_TMP_DELOBJ(frontImages_)
			_TMP_DELOBJ(leftImages_)
			_TMP_DELOBJ(rightImages_)
		}
		#undef _TMP_DELOBJ
		
		// free allocated arrays
		#define _TMP_DELOBJ(object) if (object) { delete [] object; object = 0; }
		
		_TMP_DELOBJ(frontImages_)
		_TMP_DELOBJ(leftImages_)
		_TMP_DELOBJ(rightImages_)
		
		#undef _TMP_DELOBJ
	}
	
	////////////////////////////////////////////////////////////////////////////

	SDL_Surface* WallSpriteSet::GetFrontImage(int range)
	{
		return (range > visibleDepth_) ? 0 : frontImages_[range];
	}
	
	////////////////////////////////////////////////////////////////////////////

	SDL_Surface* WallSpriteSet::GetLeftImage(int range)
	{
		return (range > visibleDepth_) ? 0 : leftImages_[range];
	}
	
	////////////////////////////////////////////////////////////////////////////

	SDL_Surface* WallSpriteSet::GetRightImage(int range)
	{
		return (range > visibleDepth_) ? 0 : rightImages_[range];
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	ArtManager::ArtManager()
	{
		allArtLoadedSuccessfully_ = this->LoadArt();
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	ArtManager::~ArtManager()
	{
		allWallSprites_.clear();
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	bool ArtManager::DidArtLoadSuccessfully() const
	{
		return allArtLoadedSuccessfully_;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	WallSpriteSet* ArtManager::GetWallSetNumber(unsigned int which)
	{
		return (which > 0 && which <= allWallSprites_.size()) ? allWallSprites_.at(which - 1) : 0 ;
	}

	////////////////////////////////////////////////////////////////////////////
	
	float ArtManager::GetXOffsetCenter(int range, int offset)
	{
		return (
			(0x0 == range) ? ( 6.0f + (static_cast<float>(offset) * 63.0f)) :
			(0x1 == range) ? (17.0f + (static_cast<float>(offset) * 41.0f)) :
			(0x2 == range) ? (23.0f + (static_cast<float>(offset) * 29.0f)) : 0.0f) * 4.0f;
	}

	////////////////////////////////////////////////////////////////////////////
	
	float ArtManager::GetXOffsetLeft(int range, int offset)
	{
		return (
			(0x0 == range) ? ( 0.0f) :
			(0x1 == range) ? ( 6.0f) :
			(0x2 == range) ? (17.0f) : 0.0f) * 4.0f;
	}

	////////////////////////////////////////////////////////////////////////////
	
	float ArtManager::GetXOffsetRight(int range, int offset)
	{
		return (	
			(0x0 == range) ? (69.0f) :
			(0x1 == range) ? (58.0f) :
			(0x2 == range) ? (52.0f) : 0.0f) * 4.0f;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	float ArtManager::GetYOffsetCenter(int range, int offset)
	{
		return (
			(0x0 == range) ? ( 8.0f) :
			(0x1 == range) ? (23.0f) :
			(0x2 == range) ? (32.0f) : 0.0f) * 4.0f;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	float ArtManager::GetYOffsetLeft(int range, int offset)
	{
		return (
			(0x0 == range) ? ( 0.0f) :
			(0x1 == range) ? ( 8.0f) :
			(0x2 == range) ? (23.0f) : 0.0f) * 4.0f;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	float ArtManager::GetYOffsetRight(int range, int offset)
	{
		return (
			(0x0 == range) ? ( 0.0f) :
			(0x1 == range) ? ( 8.0f) :
			(0x2 == range) ? (23.0f) : 0.0f) * 4.0f;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	bool ArtManager::LoadArt()
	{
		allWallSprites_.push_back(new WallSpriteSet("resources/first_wall/"));
		allWallSprites_.push_back(new WallSpriteSet("resources/stone_wall/"));
		allWallSprites_.push_back(new WallSpriteSet("resources/wood_wall/"));
		allWallSprites_.push_back(new WallSpriteSet("resources/metal_wall/"));
		
		return true;
	}

	////////////////////////////////////////////////////////////////////////////
	
	Position::Position(int x, int y, int facing) :
		x_(x),
		y_(y),
		facing_(facing)
	{
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetLeftFacingOfThis()
	{
		return new Position(x_, y_, (facing_ + 3) % 4);
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetRightFacingOfThis()
	{
		return new Position(x_, y_, (facing_ + 1) % 4);
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionLeftOfThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { xOffset = -1 * steps; } break;
			case 1: { yOffset = -1 * steps; } break;
			case 2: { xOffset = steps; } break;
			case 3: { yOffset = steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
		
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionRightOfThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { xOffset = steps; } break;
			case 1: { yOffset = steps; } break;
			case 2: { xOffset = -1 * steps; } break;
			case 3: { yOffset = -1 * steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
		
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionAheadOfThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { yOffset = -1 * steps; } break;
			case 1: { xOffset = steps; } break;
			case 2: { yOffset = steps; } break;
			case 3: { xOffset = -1 * steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
		
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionBehindThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { yOffset = steps; } break;
			case 1: { xOffset = -1 * steps; } break;
			case 2: { yOffset = -1 * steps; } break;
			case 3: { xOffset = steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
	
	void Position::Copy(Position* source)
	{
		x_ 		= source->x_;
		y_ 		= source->y_;
		facing_ = source->facing_;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	Map::Map() :
		width_(0),
		height_(0),
		walls_(0),
		passibility_(0),
		startingPoints_(0)
	{
	}
	
	////////////////////////////////////////////////////////////////////////////

	Map::~Map()
	{
		delete [] walls_;
		delete [] startingPoints_;
		delete [] passibility_;
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::MakeMockup()
	{
		this->MakeFirstMockup();
	}
	
	////////////////////////////////////////////////////////////////////////////

	int Map::GetWallForCoordinate(Position* position)
	{
		return (
			(
				(position->x_ >= 0 && position->x_ < width_) && 
				(position->y_ >= 0 && position->y_ < height_)
			) ? walls_[position->y_][position->x_][position->facing_] : 0 );
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool Map::CanPassWallForCoordinate(Position* position)
	{
		return (
			(
				(position->x_ >= 0 && position->x_ < width_) && 
				(position->y_ >= 0 && position->y_ < height_)
			) ? passibility_[position->y_][position->x_][position->facing_] : false );
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Map::GetStartingPoint(int which)
	{
		return (
			(0x0 != startingPoints_ && which < 1) 
			? startingPoints_[which] : new Position(0, 0, 0));
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::ClearMap()
	{
		int row = 0, column = 0;
		// STEP #1 - free all the allocated walls_ arrays
		// if the walls_ array pointer is allocated
		if (walls_)
		{
			// for each row 
			for (row = 0; row < height_; row++)
			{
				// if the walls_ row array pointer is allocated
				if (walls_[row])
				{
					// for each column
					for (column = 0; column < width_; column++)
					{
						// if the walls_ row column array pointer is allocated
						if (walls_[row][column])
						{
							// free the allocation
							delete [] walls_[row][column];
							walls_[row][column] = 0;
						}
					}
					
					// free the allocation
					delete [] walls_[row];
					walls_[row] = 0;
				}
			}
			
			// free the allocation
			delete [] walls_;
			walls_ = 0;
		}
		
		// STEP #2 - re-allocate walls_ arrays
		walls_ = new int** [height_];
		for (row = 0; row < height_; row++)
		{
			walls_[row] = new int* [width_];
			for (column = 0; column < width_; column++)
			{
				walls_[row][column] = new int [4];
			}
		}
		
		// STEP #3 - free all the allocated passibility_ arrays
		// if the passibility_ array pointer is allocated
		if (passibility_)
		{
			// for each row 
			for (row = 0; row < height_; row++)
			{
				// if the passibility_ row array pointer is allocated
				if (passibility_[row])
				{
					// for each column
					for (column = 0; column < width_; column++)
					{
						// if the passibility_ row column array pointer is allocated
						if (passibility_[row][column])
						{
							// free the allocation
							delete [] passibility_[row][column];
							passibility_[row][column] = 0;
						}
					}
					
					// free the allocation
					delete [] passibility_[row];
					passibility_[row] = 0;
				}
			}
			
			// free the allocation
			delete [] passibility_;
			passibility_ = 0;
		}
		
		// STEP #4 - re-allocate passibility_ arrays
		passibility_ = new bool** [height_];
		for (row = 0; row < height_; row++)
		{
			passibility_[row] = new bool* [width_];
			for (column = 0; column < width_; column++)
			{
				passibility_[row][column] = new bool [4];
			}
		}
		
		// STEP #5 - free the startingPoints array
		if (startingPoints_)
		{
			if (startingPoints_[0])
			{
				delete startingPoints_[0];
				startingPoints_[0] = 0;
			}
			delete [] startingPoints_;
			startingPoints_ = 0;
		}
		
		// STEP #6 - initialize walls_ and passibility_ arrays
		
		for (row = 0; row < height_; row++)
		{
			for (column = 0; column < width_; column++)
			{
				for (int depth = 0; depth < 4; depth++)
				{
					walls_[row][column][depth] = 0;
					passibility_[row][column][depth] = true;
				}
				
				// mark the edges of the map as non-passable
				passibility_[row][column][3] = (        0x0 == column) ? false : true;
				passibility_[row][column][0] = (        0x0 == row)    ? false : true;
				passibility_[row][column][1] = ( width_ - 1 == column) ? false : true;
				passibility_[row][column][2] = (height_ - 1 == row)    ? false : true;
			}
		}
		
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::MakeFirstMockup()
	{
		width_ = height_ = 0xA; // a 10x10 map
		this->ClearMap();
		
		startingPoints_ = new Position* [1];
		startingPoints_[0] = new Position(1, 1, 0);
		
		// build walls all around the map border
		
		const int WALL_FACING_EAST 	= 1;
		const int WALL_FACING_WEST 	= 3;
		const int WALL_FACING_NORTH = 4;
		const int WALL_FACING_SOUTH = 2;
		
		for (int row = 0; row < height_; row++)
		{
			this->MakeNormalWall(new Position(0, row, WALL_FACING_EAST), 1);
			this->MakeNormalWall(new Position(width_ - 1, row, WALL_FACING_WEST), 1);
		}
		
		for (int column = 0; column < width_; column++)
		{
			this->MakeNormalWall(new Position(column, 0, WALL_FACING_SOUTH), 1);
			this->MakeNormalWall(new Position(column, height_ - 1, WALL_FACING_NORTH), 1);
		}
		
		#if 0
		int wallData[] = 
		{
			// XCoordinate, ZCoordinate, wallFacing, wallID
			
			// 5, 9, 1, 1,
			// 5, 9, 2, 1,
			// 5, 9, 3, 1,
			4, 9, 0, 1,
			6, 9, 0, 1,
			// 5, 8, 0, 1,
			5, 8, 1, 1,
			5, 8, 3, 1,
			4, 8, 0, 1,
			6, 8, 0, 1,
			5, 7, 0, 1,
			5, 7, 1, 1,
			5, 7, 3, 1,
			4, 7, 0, 1,
			6, 7, 0, 1,
			5, 9, 2, 2,
			4, 9, 2, 2,
			3, 9, 2, 2,
			6, 9, 2, 2,
			7, 9, 2, 2,
			2, 9, 2, 2,
			8, 9, 2, 2,
			4, 8, 3, 3,
			6, 8, 1, 3,
			4, 7, 3, 3,
			6, 7, 1, 3,
			3, 7, 3, 2,
			3, 8, 3, 2,
			3, 9, 3, 2,
			3, 6, 3, 2,
			3, 5, 3, 2,
			3, 6, 0, 4,
			4, 6, 0, 4
		};
		
		int wallCount = (sizeof(wallData) / sizeof(int)) / 4;
		for (int index = 0; index < wallCount; index++)
		{
			this->MakeNormalWall(
				new Position(wallData[index], wallData[index + 1], wallData[index + 2]),
					wallData[index + 3]);
		}
		#endif
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::MakeNormalWall(Position* position, int wallID)
	{
		walls_[position->y_][position->x_][position->facing_] = wallID;
		passibility_[position->y_][position->x_][position->facing_] = false;
		
		Position* positionAhead = position->GetPositionAheadOfThis(1);
		
		if (
			(positionAhead->x_ >= 0x0 && positionAhead->x_ < width_) &&
			(positionAhead->y_ >= 0x0 && positionAhead->y_ < height_))
		{
			int newFacing = (position->facing_ + 2) % 4;
			walls_[positionAhead->y_][positionAhead->x_][newFacing] = wallID;
			passibility_[positionAhead->y_][positionAhead->x_][newFacing] = false;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////

	GameState::GameState() :
		currentMap_(0),
		playerPosition_(new Position(0, 0, 0))
	{
	}
	
	GameState::~GameState()
	{
		if (currentMap_) { delete currentMap_; currentMap_ = 0; }
		if (playerPosition_) { delete playerPosition_; playerPosition_ = 0; }
	}
	
	////////////////////////////////////////////////////////////////////////////

	void GameState::StartNewGame()
	{
		if (currentMap_) { delete currentMap_; }
		currentMap_ = new Map();
		currentMap_->MakeMockup();
		playerPosition_->Copy(currentMap_->GetStartingPoint(0));
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerForward()
	{
		if (currentMap_->CanPassWallForCoordinate(playerPosition_))
		{
			playerPosition_->Copy(playerPosition_->GetPositionAheadOfThis(1));
			return true;
		}
		
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerBack()
	{
		Position tempPosition(0, 0, 0);
		tempPosition.Copy(playerPosition_->GetLeftFacingOfThis()->GetLeftFacingOfThis());
		if (currentMap_->CanPassWallForCoordinate(&tempPosition))
		{
			playerPosition_->Copy(playerPosition_->GetPositionBehindThis(1));
			return true;
		}
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerLeft()
	{
		Position tempPosition(0, 0, 0);
		tempPosition.Copy(playerPosition_->GetLeftFacingOfThis());
		if (currentMap_->CanPassWallForCoordinate(&tempPosition))
		{
			playerPosition_->Copy(playerPosition_->GetPositionLeftOfThis(1));
			return true;
		}
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerRight()
	{
		Position tempPosition(0, 0, 0);
		tempPosition.Copy(playerPosition_->GetRightFacingOfThis());
		if (currentMap_->CanPassWallForCoordinate(&tempPosition))
		{
			playerPosition_->Copy(playerPosition_->GetPositionRightOfThis(1));
			return true;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////

	void GameState::TurnPlayerLeft()
	{
		playerPosition_->Copy(playerPosition_->GetLeftFacingOfThis());
	}

	////////////////////////////////////////////////////////////////////////////

	void GameState::TurnPlayerRight()
	{
		playerPosition_->Copy(playerPosition_->GetRightFacingOfThis());
	}

	////////////////////////////////////////////////////////////////////////////

	Map* GameState::GetCurrentMap() const
	{
		return currentMap_;
	}

	////////////////////////////////////////////////////////////////////////////

	Position* GameState::GetPlayerPosition() const
	{
		return playerPosition_;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	MapView::MapView(ArtManager* artManager) :
		artManager_(artManager),
		viewWidth_(300),
		viewHeight_(400)
	{
	}
	
	////////////////////////////////////////////////////////////////////////////

	MapView::~MapView()
	{
		// do not delete the artManager pointer because it is allocated externally
	}
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::RenderMap(SDL_Surface* target, Map* currentMap, Position* currentPosition)
	{
		this->DrawSky(target, currentMap, currentPosition);
		this->DrawGround(target, currentMap, currentPosition);
		this->DrawDistRank(target, currentMap, currentPosition, 2, 1);
		this->DrawDistRank(target, currentMap, currentPosition, 1, 1);
		this->DrawDistRank(target, currentMap, currentPosition, 0, 1);
	}
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::DrawSky(SDL_Surface* target, Map* currentMap, Position* currentPosition)
	{
		SDL_Rect skyRect;
		skyRect.x = skyRect.y = 0;
		skyRect.w = viewWidth_;
		skyRect.h = static_cast<int>(0.6f * static_cast<float>(viewHeight_));
		SDL_FillRect(target, &skyRect, SDL_MapRGB(target->format, 149, 208, 255));
	}
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::DrawGround(SDL_Surface* target, Map* currentMap, Position* currentPosition)
	{
		SDL_Rect groundRect;
		groundRect.x = 0;
		groundRect.y = static_cast<int>(0.6f * static_cast<float>(viewHeight_));
		groundRect.w = viewWidth_;
		groundRect.h = static_cast<int>(0.4f * static_cast<float>(viewHeight_));
		SDL_FillRect(target, &groundRect, SDL_MapRGB(target->format, 51, 61, 8));
	}

	////////////////////////////////////////////////////////////////////////////

	void MapView::DrawDistRank(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset)
	{
		Position pointToDraw(0, 0, 0);
		Position basePoint(0, 0, 0);
		
		basePoint.Copy(currentPosition->GetPositionAheadOfThis(range));
		
		// draw to the left
		for (int index = -1 * offset; index < 0; index++)
		{
			pointToDraw.Copy(basePoint.GetPositionLeftOfThis(-1 * index));
			this->DrawStraightWall(target, currentMap, &pointToDraw, range, index);
		}
		
		// draw the the right
		for (int index = offset; index > 0; index--)
		{
			pointToDraw.Copy(basePoint.GetPositionRightOfThis(index));
			this->DrawStraightWall(target, currentMap, &pointToDraw, range, index);
		}
		
		// draw dead center
		this->DrawLeftWall(target, currentMap, &basePoint, range, 0);
		this->DrawRightWall(target, currentMap, &basePoint, range, 0);
		this->DrawStraightWall(target, currentMap, &basePoint, range, 0);
	}	
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::DrawLeftWall(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset)
	{
		SDL_Surface* nextImage = 0;
		int nextWallNumber = 0;
		WallSpriteSet* nextSet = 0;
		
		nextWallNumber = currentMap->GetWallForCoordinate(currentPosition->GetLeftFacingOfThis());
		nextSet = artManager_->GetWallSetNumber(nextWallNumber);
		if (nextSet)
		{
			nextImage = nextSet->GetLeftImage(range);
			if (nextImage)
			{
				int destX = static_cast<int>(artManager_->GetXOffsetLeft(range, offset));
				int destY = static_cast<int>(artManager_->GetYOffsetLeft(range, offset));
				Engine::BlitSprite(nextImage, target, destX, destY);
			}
		}
	}
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::DrawRightWall(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset)
	{
		SDL_Surface* nextImage = 0;
		int nextWallNumber = 0;
		WallSpriteSet* nextSet = 0;
		
		nextWallNumber = currentMap->GetWallForCoordinate(currentPosition->GetRightFacingOfThis());
		nextSet = artManager_->GetWallSetNumber(nextWallNumber);
		if (nextSet)
		{
			nextImage = nextSet->GetRightImage(range);
			if (nextImage)
			{
				int destX = static_cast<int>(artManager_->GetXOffsetRight(range, offset));
				int destY = static_cast<int>(artManager_->GetYOffsetRight(range, offset));
				Engine::BlitSprite(nextImage, target, destX, destY);
			}
		}
	}
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::DrawStraightWall(SDL_Surface* target, Map* currentMap, Position* currentPosition, int range, int offset)
	{
		SDL_Surface* nextImage = 0;
		int nextWallNumber = 0;
		WallSpriteSet* nextSet = 0;
		
		nextWallNumber = currentMap->GetWallForCoordinate(currentPosition);
		nextSet = artManager_->GetWallSetNumber(nextWallNumber);
		if (nextSet)
		{
			nextImage = nextSet->GetFrontImage(range);
			if (nextImage)
			{
				int destX = static_cast<int>(artManager_->GetXOffsetCenter(range, offset));
				int destY = static_cast<int>(artManager_->GetYOffsetCenter(range, offset));
				Engine::BlitSprite(nextImage, target, destX, destY);
			}
		}
	}
	
} // end namespace

////////////////////////////////////////////////////////////////////////////////

LOFI::Engine* LOFI::globalEngineInstance = 0;
FILE* stderrLog = 0;

void OpenStdErrLog();
void CloseStdErrLog();

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	OpenStdErrLog();
	atexit(CloseStdErrLog);
	atexit(SDL_Quit);

	LOFI::Engine engine;
	LOFI::globalEngineInstance = &engine;

	if (engine.Initialize(argc, argv))
	{
		engine.Execute();
		engine.Destroy();
	}
	else
	{
		WriteLog(stderr, "Engine Initialization Failed!\n");
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void OpenStdErrLog()
{
	stderrLog = freopen("stderr.log", "a", stderr);
	if (stderrLog)
	{
		WriteLog(stderr, "Starting.\n");
	}
}

////////////////////////////////////////////////////////////////////////////////

void CloseStdErrLog()
{
	if (stderrLog)
	{
		WriteLog(stderr, "Stopping.\n");
		fclose(stderr);
	}
}

