
// CODESTYLE: v2.0

// MapView.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.MapView

/**
 * @file MapView.h
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.MapView - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __MAPVIEW_H__
#define __MAPVIEW_H__

struct SDL_Surface;

namespace LOFI
{
	class ArtManager;
	class Map;
	class Position;
	
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
		SDL_Surface* floorAndCeiling_;
	}; // end class

} // end namespace
#endif


