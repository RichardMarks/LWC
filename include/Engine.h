
// CODESTYLE: v2.0

// Engine.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: the game engine class is to be used as a singleton; without having a misbehaving static instance singleton pattern

/**
 * @file Engine.h
 * @brief Core Engine - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __ENGINE_H__
#define __ENGINE_H__

struct SDL_Surface;

namespace LOFI
{

	class Map;
	class MapView;
	class MiniMap;
	class BitmapFont;
	class ArtManager;
	class GameState;

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
		
		/// gets the game state
		GameState* GetGameState() const;

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

} // end namespace
#endif


