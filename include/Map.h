
// CODESTYLE: v2.0

// Map.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.Map

/**
 * @file Map.h
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.Map - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __MAP_H__
#define __MAP_H__

namespace LOFI
{
	const int PLAYER_FACING_NORTH 	= 0;
	const int PLAYER_FACING_SOUTH 	= 2;
	const int PLAYER_FACING_EAST  	= 1;
	const int PLAYER_FACING_WEST  	= 3;
	
	const int WALL_FACING_EAST 		= 1;
	const int WALL_FACING_WEST 		= 3;
	const int WALL_FACING_NORTH 	= 4;
	const int WALL_FACING_SOUTH 	= 2;
	
	const int WALL_TYPE_BRICK 		= 1;
	const int WALL_TYPE_STONE 		= 2;
	const int WALL_TYPE_WOOD 		= 3;
	const int WALL_TYPE_METAL		= 4;
	
	class Position;
	
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
		int GetWidth() const;
		int GetHeight() const;
		
		bool IsVisited(int column, int row) const;
	private:
		void MakeFirstMockup();
		void MakeNormalWall(Position* position, int wallID);
		void RemoveWall(Position* position);
		int width_;
		int height_;
		
		#if 0
		typedef struct MapCell_Type
		{
			bool visited_;
			bool solid_;
			int wall_;
		} MapCell, *MapCellPtr;
		
		MapCellPtr cells_;
		#endif
		
		#if 1
		int*** walls_;
		bool*** passibility_;
		bool*** visited_;
		#endif
		
		Position** startingPoints_;
	}; // end class 

} // end namespace
#endif


