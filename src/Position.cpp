
// CODESTYLE: v2.0

// Position.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.Position

/**
 * @file Position.cpp
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.Position - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
	Position::Position(int x, int y, int facing) :
		x_(x),
		y_(y),
		facing_(facing)
	{
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetLeftFacingOfThis()
	{
		return new Position(x_, y_, (facing_ + 3) % 4);
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetRightFacingOfThis()
	{
		return new Position(x_, y_, (facing_ + 1) % 4);
	}
	
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionLeftOfThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { xOffset = -1 * steps; } break;
			case 1: { yOffset = -1 * steps; } break;
			case 2: { xOffset = steps; } break;
			case 3: { yOffset = steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
		
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionRightOfThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { xOffset = steps; } break;
			case 1: { yOffset = steps; } break;
			case 2: { xOffset = -1 * steps; } break;
			case 3: { yOffset = -1 * steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
		
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionAheadOfThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { yOffset = -1 * steps; } break;
			case 1: { xOffset = steps; } break;
			case 2: { yOffset = steps; } break;
			case 3: { xOffset = -1 * steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
		
	////////////////////////////////////////////////////////////////////////////

	Position* Position::GetPositionBehindThis(int steps)
	{
		int xOffset = 0;
		int yOffset = 0;
		switch(facing_)
		{
			case 0: { yOffset = steps; } break;
			case 1: { xOffset = -1 * steps; } break;
			case 2: { yOffset = -1 * steps; } break;
			case 3: { xOffset = steps; } break;
			default: break;
		}
		return new Position(x_ + xOffset, y_ + yOffset, facing_);
	}
	
	void Position::Copy(Position* source)
	{
		x_ 		= source->x_;
		y_ 		= source->y_;
		facing_ = source->facing_;
	}
	
	bool Position::InBounds(int left, int top, int right, int bottom, bool inclusive)
	{
		return (!inclusive) ? 
			((x_ > left && y_ > top && x_ < right && y_ < bottom) ? true : false) : 
			((x_ >= left && y_ >= top && x_ <= right && y_ <= bottom) ? true : false);
	}
} // end namespace

