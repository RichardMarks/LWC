
// CODESTYLE: v2.0

// ArtManager.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.ArtManager

/**
 * @file ArtManager.cpp
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.ArtManager - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
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
} // end namespace

