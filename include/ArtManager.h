
// CODESTYLE: v2.0

// ArtManager.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.ArtManager

/**
 * @file ArtManager.h
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.ArtManager - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __ARTMANAGER_H__
#define __ARTMANAGER_H__

#include <vector>

namespace LOFI
{
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

} // end namespace
#endif


