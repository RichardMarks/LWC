
// CODESTYLE: v2.0

// Position.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.Position

/**
 * @file Position.h
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.Position - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __POSITION_H__
#define __POSITION_H__

namespace LOFI
{
	/**
	 * @class Position
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.Position
	 */
	class Position
	{
	public:
		Position() : x_(0), y_(0), facing_(0) {};
		~Position() { x_ = y_ = facing_ = 0; };
		Position(int x, int y, int facing);
		Position* GetLeftFacingOfThis();
		Position* GetRightFacingOfThis();
		Position* GetPositionLeftOfThis(int steps);
		Position* GetPositionRightOfThis(int steps);
		Position* GetPositionAheadOfThis(int steps);
		Position* GetPositionBehindThis(int steps);
		void Copy(Position* source);
		bool InBounds(int left, int top, int right, int bottom, bool inclusive = true);
		int x_;
		int y_;
		int facing_;
	}; // end class

} // end namespace
#endif


