
// CODESTYLE: v2.0

// Map.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.Map

/**
 * @file Map.cpp
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.Map - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
	Map::Map() :
		width_(0),
		height_(0),
		walls_(0),
		passibility_(0),
		startingPoints_(0)
	{
	}
	
	////////////////////////////////////////////////////////////////////////////

	Map::~Map()
	{
		delete [] walls_;
		delete [] startingPoints_;
		delete [] passibility_;
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::MakeMockup()
	{
		this->MakeFirstMockup();
	}
	
	////////////////////////////////////////////////////////////////////////////

	int Map::GetWallForCoordinate(Position* position)
	{
		return (
			(
				(position->x_ >= 0 && position->x_ < width_) && 
				(position->y_ >= 0 && position->y_ < height_)
			) ? walls_[position->y_][position->x_][position->facing_] : 0 );
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool Map::CanPassWallForCoordinate(Position* position)
	{
		return (
			(
				(position->x_ >= 0 && position->x_ < width_) && 
				(position->y_ >= 0 && position->y_ < height_)
			) ? passibility_[position->y_][position->x_][position->facing_] : false );
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Map::GetStartingPoint(int which)
	{
		return (
			(0x0 != startingPoints_ && which < 1) 
			? startingPoints_[which] : new Position(0, 0, 0));
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::ClearMap()
	{
		int row = 0, column = 0;
		// STEP #1 - free all the allocated walls_ arrays
		// if the walls_ array pointer is allocated
		if (walls_)
		{
			// for each row 
			for (row = 0; row < height_; row++)
			{
				// if the walls_ row array pointer is allocated
				if (walls_[row])
				{
					// for each column
					for (column = 0; column < width_; column++)
					{
						// if the walls_ row column array pointer is allocated
						if (walls_[row][column])
						{
							// free the allocation
							delete [] walls_[row][column];
							walls_[row][column] = 0;
						}
					}
					
					// free the allocation
					delete [] walls_[row];
					walls_[row] = 0;
				}
			}
			
			// free the allocation
			delete [] walls_;
			walls_ = 0;
		}
		
		// STEP #2 - re-allocate walls_ arrays
		walls_ = new int** [height_];
		for (row = 0; row < height_; row++)
		{
			walls_[row] = new int* [width_];
			for (column = 0; column < width_; column++)
			{
				walls_[row][column] = new int [4];
			}
		}
		
		// STEP #3 - free all the allocated passibility_ arrays
		// if the passibility_ array pointer is allocated
		if (passibility_)
		{
			// for each row 
			for (row = 0; row < height_; row++)
			{
				// if the passibility_ row array pointer is allocated
				if (passibility_[row])
				{
					// for each column
					for (column = 0; column < width_; column++)
					{
						// if the passibility_ row column array pointer is allocated
						if (passibility_[row][column])
						{
							// free the allocation
							delete [] passibility_[row][column];
							passibility_[row][column] = 0;
						}
					}
					
					// free the allocation
					delete [] passibility_[row];
					passibility_[row] = 0;
				}
			}
			
			// free the allocation
			delete [] passibility_;
			passibility_ = 0;
		}
		
		// STEP #4 - re-allocate passibility_ arrays
		passibility_ = new bool** [height_];
		for (row = 0; row < height_; row++)
		{
			passibility_[row] = new bool* [width_];
			for (column = 0; column < width_; column++)
			{
				passibility_[row][column] = new bool [4];
			}
		}
		
		// STEP #5 - free the startingPoints array
		if (startingPoints_)
		{
			if (startingPoints_[0])
			{
				delete startingPoints_[0];
				startingPoints_[0] = 0;
			}
			delete [] startingPoints_;
			startingPoints_ = 0;
		}
		
		// STEP #6 - initialize walls_ and passibility_ arrays
		
		for (row = 0; row < height_; row++)
		{
			for (column = 0; column < width_; column++)
			{
				for (int depth = 0; depth < 4; depth++)
				{
					walls_[row][column][depth] = 0;
					passibility_[row][column][depth] = true;
				}
				
				// mark the edges of the map as non-passable
				passibility_[row][column][3] = (        0x0 == column) ? false : true;
				passibility_[row][column][0] = (        0x0 == row)    ? false : true;
				passibility_[row][column][1] = ( width_ - 1 == column) ? false : true;
				passibility_[row][column][2] = (height_ - 1 == row)    ? false : true;
			}
		}
		
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::MakeFirstMockup()
	{
		
		
		
		width_ = height_ = 0xA; // a 10x10 map
		this->ClearMap();
		
		startingPoints_ = new Position* [1];
		startingPoints_[0] = new Position(1, 1, PLAYER_FACING_EAST);
		
		
		
		
		// build walls all around the map border
		for (int row = 0; row < height_; row++)
		{
			this->MakeNormalWall(new Position(0, row, WALL_FACING_EAST), WALL_TYPE_STONE);
			this->MakeNormalWall(new Position(width_ - 1, row, WALL_FACING_WEST), WALL_TYPE_STONE);
		}
		
		for (int column = 0; column < width_; column++)
		{
			this->MakeNormalWall(new Position(column, 0, WALL_FACING_SOUTH), WALL_TYPE_STONE);
			this->MakeNormalWall(new Position(column, height_ - 1, WALL_FACING_NORTH), WALL_TYPE_STONE);
		}
		
		
		// create a corridor in the west edge of the map
		for (int column = 2; column < width_ - 2; column++)
		{
			this->MakeNormalWall(new Position(column, 1, WALL_FACING_SOUTH), WALL_TYPE_STONE);
			this->MakeNormalWall(new Position(column, height_ - 2, WALL_FACING_SOUTH), WALL_TYPE_STONE);
		}
		
		for (int row = 2; row < height_ - 1; row++)
		{
			this->MakeNormalWall(new Position(1, row, WALL_FACING_EAST), WALL_TYPE_STONE);
			this->MakeNormalWall(new Position(width_ - 2, row, WALL_FACING_WEST), WALL_TYPE_STONE);
		}
		
		// remove a wall half-way down the west inner wall
		this->RemoveWall(new Position(2, height_ / 2, WALL_FACING_WEST));
		
		
		
		
		
		#if 0
		int wallData[] = 
		{
			// XCoordinate, ZCoordinate, wallFacing, wallID
			
			// 5, 9, 1, 1,
			// 5, 9, 2, 1,
			// 5, 9, 3, 1,
			4, 9, 0, 1,
			6, 9, 0, 1,
			// 5, 8, 0, 1,
			5, 8, 1, 1,
			5, 8, 3, 1,
			4, 8, 0, 1,
			6, 8, 0, 1,
			5, 7, 0, 1,
			5, 7, 1, 1,
			5, 7, 3, 1,
			4, 7, 0, 1,
			6, 7, 0, 1,
			5, 9, 2, 2,
			4, 9, 2, 2,
			3, 9, 2, 2,
			6, 9, 2, 2,
			7, 9, 2, 2,
			2, 9, 2, 2,
			8, 9, 2, 2,
			4, 8, 3, 3,
			6, 8, 1, 3,
			4, 7, 3, 3,
			6, 7, 1, 3,
			3, 7, 3, 2,
			3, 8, 3, 2,
			3, 9, 3, 2,
			3, 6, 3, 2,
			3, 5, 3, 2,
			3, 6, 0, 4,
			4, 6, 0, 4
		};
		
		int wallCount = (sizeof(wallData) / sizeof(int)) / 4;
		for (int index = 0; index < wallCount; index++)
		{
			this->MakeNormalWall(
				new Position(wallData[index], wallData[index + 1], wallData[index + 2]),
					wallData[index + 3]);
		}
		#endif
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::MakeNormalWall(Position* position, int wallID)
	{
		walls_[position->y_][position->x_][position->facing_] = wallID;
		passibility_[position->y_][position->x_][position->facing_] = false;
		
		Position positionAhead;
		positionAhead.Copy(position->GetPositionAheadOfThis(1));
		if (positionAhead.InBounds(0, 0, width_, height_))
		{
			int newFacing = (position->facing_ + 2) % 4;
			walls_[positionAhead.y_][positionAhead.x_][newFacing] = wallID;
			passibility_[positionAhead.y_][positionAhead.x_][newFacing] = false;
		}
		
		# if 0
		Position* positionAhead = position->GetPositionAheadOfThis(1);
		
		if (
			(positionAhead->x_ >= 0x0 && positionAhead->x_ < width_) &&
			(positionAhead->y_ >= 0x0 && positionAhead->y_ < height_))
		{
			int newFacing = (position->facing_ + 2) % 4;
			walls_[positionAhead->y_][positionAhead->x_][newFacing] = wallID;
			passibility_[positionAhead->y_][positionAhead->x_][newFacing] = false;
		}
		#endif
	}
	
	////////////////////////////////////////////////////////////////////////////

	void Map::RemoveWall(Position* position)
	{
		walls_[position->y_][position->x_][position->facing_] = 0;
		passibility_[position->y_][position->x_][position->facing_] = true;
		
		Position positionAhead;
		positionAhead.Copy(position->GetPositionAheadOfThis(1));
		if (positionAhead.InBounds(0, 0, width_, height_))
		{
			int newFacing = (position->facing_ + 2) % 4;
			walls_[positionAhead.y_][positionAhead.x_][newFacing] = 0;
			passibility_[positionAhead.y_][positionAhead.x_][newFacing] = true;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool Map::IsVisited(int column, int row) const
	{
		
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////

	int Map::GetWidth() const
	{
		return width_;
	}
	
	////////////////////////////////////////////////////////////////////////////

	int Map::GetHeight() const
	{
		return height_;
	}
	
	
} // end namespace

