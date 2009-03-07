
// CODESTYLE: v2.0

// MapView.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.MapView

/**
 * @file MapView.cpp
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.MapView - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
	MapView::MapView(ArtManager* artManager) :
		artManager_(artManager),
		viewWidth_(300),
		viewHeight_(400)
	{
		//floorAndCeiling_ = Engine::LoadImageResource("resources/updown/floorceil.png");
		
	}
	
	////////////////////////////////////////////////////////////////////////////

	MapView::~MapView()
	{
		// do not delete the artManager pointer because it is allocated externally
		
		//Engine::UnloadImageResource(floorAndCeiling_);
	}
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::RenderMap(SDL_Surface* target, Map* currentMap, Position* currentPosition)
	{
		
		//Engine::BlitSprite(floorAndCeiling_, target, 0, 0);
		
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
		SDL_FillRect(target, &skyRect, SDL_MapRGB(target->format, 77, 130, 229));
	}
	
	////////////////////////////////////////////////////////////////////////////

	void MapView::DrawGround(SDL_Surface* target, Map* currentMap, Position* currentPosition)
	{
		SDL_Rect groundRect;
		groundRect.x = 0;
		groundRect.y = static_cast<int>(0.6f * static_cast<float>(viewHeight_));
		groundRect.w = viewWidth_;
		groundRect.h = static_cast<int>(0.4f * static_cast<float>(viewHeight_));
		SDL_FillRect(target, &groundRect, SDL_MapRGB(target->format, 16, 80, 30));
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

