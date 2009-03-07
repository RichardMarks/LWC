
// CODESTYLE: v2.0

// WallSpriteSet.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.WallSpriteSet

/**
 * @file WallSpriteSet.cpp
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.WallSpriteSet - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
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
} // end namespace

