
// CODESTYLE: v2.0

// MiniMap.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: takes a pointer to a Map instance and has a rendering method to display a mini-top-down version of it

/**
 * @file MiniMap.cpp
 * @brief Mini Map - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
	MiniMap::MiniMap(Map* sourceMap, int width, int height) : 
		currentMap_(0),
		miniMapSurface_(0),
		width_(width), 
		height_(height)
	{
		width_ = (width_ <= 0) ? 1 : width_;
		height_ = (height_ <= 0) ? 1 : height_;
		
		SetMap(sourceMap);
		RecreateMiniMapSurface();
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	MiniMap::~MiniMap()
	{
		DestroyMiniMapSurface();
		currentMap_ 	= 0;
		width_ 			= 0;
		height_ 		= 0;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	void MiniMap::SetMap(Map* sourceMap)
	{
		currentMap_ = sourceMap;
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	void MiniMap::Render(SDL_Surface* target, int x, int y)
	{
		if ((!miniMapSurface_) || (!target))
		{
			return;
		}
		
		Engine::BlitSprite(miniMapSurface_, target, x, y);
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	void MiniMap::Update()
	{
		RecreateMiniMapSurface();
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	void MiniMap::RecreateMiniMapSurface()
	{
		if (!miniMapSurface_)
		{
			// create the surface that will hold the minimap
			miniMapSurface_ = SDL_CreateRGBSurface(
				SDL_SRCCOLORKEY, 
				width_, height_, 
				globalEngineInstance->GetScreen()->format->BitsPerPixel, 
				0, 0, 0, 0);
		
			if (!miniMapSurface_)
			{
				WriteLog(stderr, "Unable to create mini-map surface %dx%d!\n\tSDL Error: %s\n", width_, height_, SDL_GetError());
				return;
			}
		
			SDL_SetColorKey(miniMapSurface_, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(miniMapSurface_->format, 0, 0, 0));
		}
		
		
		// calculate size of cells
		int mapWidth = currentMap_->GetWidth();
		int mapHeight = currentMap_->GetHeight();
		int mapCellWidth = width_ / mapWidth;
		int mapCellHeight = height_ / mapHeight;
		
		// get player position
		int playerZ = globalEngineInstance->GetGameState()->GetPlayerPosition()->y_;
		int playerX = globalEngineInstance->GetGameState()->GetPlayerPosition()->x_;
		
		// some colors
		unsigned int playerCellColor 		= SDL_MapRGB(miniMapSurface_->format, 255, 255, 0);
		unsigned int notVisitedCellColor 	= SDL_MapRGB(miniMapSurface_->format, 32, 32, 32);
		unsigned int visitedCellColor 		= SDL_MapRGB(miniMapSurface_->format, 0, 128, 0);
		
		// clear mini-map
		SDL_FillRect(miniMapSurface_, 0, SDL_MapRGB(miniMapSurface_->format, 0, 0, 0));
		
		// allocate our rect only once	
		SDL_Rect box;
		
		// draw cells
		for (int row = 0; row < mapHeight; row++)
		{
			for (int column = 0; column < mapWidth; column++)
			{
				// calculate cell positioning
				box.x = column * mapCellWidth;
				box.y = row * mapCellHeight;
				box.w = box.x + mapCellWidth;
				box.h = box.y + mapCellHeight;
				
				// draw the cell
				if (currentMap_->IsVisited(column, row))
				{
					
					if (row == playerZ && column == playerX)
					{
						// if the player is here
						SDL_FillRect(miniMapSurface_, &box, playerCellColor);
					}
					else
					{
						// we have been here before
						SDL_FillRect(miniMapSurface_, &box, visitedCellColor);
					}
				}
				else
				{
					// we have not been here before
					SDL_FillRect(miniMapSurface_, &box, notVisitedCellColor);
				}
			}
		}
	}
	
	////////////////////////////////////////////////////////////////////////////
	
	void MiniMap::DestroyMiniMapSurface()
	{
		Engine::UnloadImageResource(miniMapSurface_);
	}
} // end namespace

