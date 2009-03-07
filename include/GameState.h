
// CODESTYLE: v2.0

// GameState.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.GameState

/**
 * @file GameState.h
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.GameState - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

namespace LOFI
{
	class Map;
	class Position;
	
	/**
	 * @class GameState
	 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.GameState
	 *
	 * Updated by Richard Marks to include strafing and condition checking
	 */
	class GameState
	{
	public:
		GameState();
		~GameState();
		void StartNewGame();
		
		bool MovePlayerForward();
		bool MovePlayerBack();
		
		// new strafing motion commands
		bool MovePlayerLeft();
		bool MovePlayerRight();
		
		void TurnPlayerLeft();
		void TurnPlayerRight();
		
		Map* GetCurrentMap() const;
		Position* GetPlayerPosition() const;
		
	private:
		Map* currentMap_;
		Position* playerPosition_;
	}; // end class

} // end namespace
#endif


