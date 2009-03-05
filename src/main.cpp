
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
	 */
	class GameState
	{
	public:
		GameState();
		~GameState();
		void StartNewGame();
		void MovePlayerForward();
		void TurnPlayerLeft();
		void TurnPlayerRight();
		void MovePlayerBack();
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

		/// the SDL screen surface
		SDL_Surface* screen_;
		
		/// the SDL event data
		SDL_Event* event_;
		
		/// the art manager
		ArtManager* artManager_;
		
		// the map view
		MapView* mapView_;
		
		// the game state
		GameState* gameState_;

	}; // end class
	
	extern Engine* globalEngineInstance;
	
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
		event_(0),
		artManager_(0),
		mapView_(0),
		gameState_(0)
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
		screen_ = SDL_SetVideoMode(300, 400, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);

		if (!screen_)
		{
			// log the error
			WriteLog(stderr, "SDL Screen Initialization Failed!\n\tSDL Error: %s\n", SDL_GetError());

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
								gameState_->MovePlayerForward();
							} break;
							
							case 's':
							case 'S':
							case SDLK_DOWN:
							{
								gameState_->MovePlayerBack();
							} break;
							
							case 'a':
							case 'A':
							case SDLK_LEFT:
							{
								gameState_->TurnPlayerLeft();
							} break;
							
							case 'd':
							case 'D':
							case SDLK_RIGHT:
							{
								gameState_->TurnPlayerRight();
							} break;

							default: break;
						} // end switch
					} break;

					default: break;
				} // end switch
			} // end while

			// clear the screen blue-ish color
			// this->ClearScreen(SDL_MapRGB(screen_->format, 0, 192, 255));

			// draw
			mapView_->RenderMap(screen_, gameState_->GetCurrentMap(), gameState_->GetPlayerPosition());

			// flip the screen
			this->FlipScreen();
			
			// reduce the cpu hoggingness of SDL ^-^
			SDL_Delay(20);
		} // end while

	}

	////////////////////////////////////////////////////////////////////////////

	void Engine::Destroy()
	{
		#define _TMP_DELOBJ(object) if (object) { delete object; object = 0; }

		_TMP_DELOBJ(event_)
		
		_TMP_DELOBJ(artManager_)
		_TMP_DELOBJ(mapView_)
		_TMP_DELOBJ(gameState_)

		#undef _TMP_DELOBJ
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
		SDL_Flip(screen_);
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Engine::ClearScreen(unsigned int color)
	{
		SDL_FillRect(screen_, 0, color);
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
		startingPoints_[0] = new Position(5, 9, 0);
		
		int wallData[] = 
		{
			// XCoordinate, YCoordinate, ZCoordinate, wallID
			
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

	void GameState::MovePlayerForward()
	{
		if (currentMap_->CanPassWallForCoordinate(playerPosition_))
		{
			playerPosition_->Copy(playerPosition_->GetPositionAheadOfThis(1));
		}
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

	void GameState::MovePlayerBack()
	{
		Position tempPosition(0, 0, 0);
		tempPosition.Copy(playerPosition_->GetLeftFacingOfThis()->GetLeftFacingOfThis());
		if (currentMap_->CanPassWallForCoordinate(&tempPosition))
		{
			playerPosition_->Copy(playerPosition_->GetPositionBehindThis(1));
		}
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

