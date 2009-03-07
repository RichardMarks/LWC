
// CODESTYLE: v2.0

// MiniMap.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: takes a pointer to a Map instance and has a rendering method to display a mini-top-down version of it

/**
 * @file MiniMap.h
 * @brief Mini Map - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __MINIMAP_H__
#define __MINIMAP_H__

struct SDL_Surface;

namespace LOFI
{
	class Map;
	
	/**
	 * @class MiniMap
	 * @brief takes a pointer to a Map instance and has a rendering method to display a mini-top-down version of it
	 *
	 * The mini-map will display only the map-cells that have been marked as visited.
	 */
	class MiniMap
	{
	public:
		MiniMap(Map* sourceMap, int width, int height);
		~MiniMap();
		void SetMap(Map* sourceMap);
		void Render(SDL_Surface* target, int x, int y);
		void Update();
	private:
		void RecreateMiniMapSurface();
		void DestroyMiniMapSurface();
		Map* currentMap_;
		SDL_Surface* miniMapSurface_;
		int width_;
		int height_;
	}; // end class 

} // end namespace
#endif


