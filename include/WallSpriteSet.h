
// CODESTYLE: v2.0

// WallSpriteSet.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.WallSpriteSet

/**
 * @file WallSpriteSet.h
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.WallSpriteSet - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __WALLSPRITESET_H__
#define __WALLSPRITESET_H__

struct SDL_Surface;

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

} // end namespace
#endif


