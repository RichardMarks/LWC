
// CODESTYLE: v2.0

// GameState.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: C++ port of Java public class com.scrimisms.LofiWanderings.GameState

/**
 * @file GameState.cpp
 * @brief C++ port of Java public class com.scrimisms.LofiWanderings.GameState - Implementation
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

namespace LOFI
{
	GameState::GameState() :
		currentMap_(0),
		playerPosition_(new Position(0, 0, 0))
	{
	}
	
	GameState::~GameState()
	{
		if (currentMap_) { delete currentMap_; currentMap_ = 0; }
		if (playerPosition_) { delete playerPosition_; playerPosition_ = 0; }
	}
	
	////////////////////////////////////////////////////////////////////////////

	void GameState::StartNewGame()
	{
		if (currentMap_) { delete currentMap_; }
		currentMap_ = new Map();
		currentMap_->MakeMockup();
		playerPosition_->Copy(currentMap_->GetStartingPoint(0));
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerForward()
	{
		if (currentMap_->CanPassWallForCoordinate(playerPosition_))
		{
			playerPosition_->Copy(playerPosition_->GetPositionAheadOfThis(1));
			return true;
		}
		
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerBack()
	{
		Position tempPosition(0, 0, 0);
		tempPosition.Copy(playerPosition_->GetLeftFacingOfThis()->GetLeftFacingOfThis());
		if (currentMap_->CanPassWallForCoordinate(&tempPosition))
		{
			playerPosition_->Copy(playerPosition_->GetPositionBehindThis(1));
			return true;
		}
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerLeft()
	{
		Position tempPosition(0, 0, 0);
		tempPosition.Copy(playerPosition_->GetLeftFacingOfThis());
		if (currentMap_->CanPassWallForCoordinate(&tempPosition))
		{
			playerPosition_->Copy(playerPosition_->GetPositionLeftOfThis(1));
			return true;
		}
		return false;
	}
	
	////////////////////////////////////////////////////////////////////////////

	bool GameState::MovePlayerRight()
	{
		Position tempPosition(0, 0, 0);
		tempPosition.Copy(playerPosition_->GetRightFacingOfThis());
		if (currentMap_->CanPassWallForCoordinate(&tempPosition))
		{
			playerPosition_->Copy(playerPosition_->GetPositionRightOfThis(1));
			return true;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////

	void GameState::TurnPlayerLeft()
	{
		playerPosition_->Copy(playerPosition_->GetLeftFacingOfThis());
	}

	////////////////////////////////////////////////////////////////////////////

	void GameState::TurnPlayerRight()
	{
		playerPosition_->Copy(playerPosition_->GetRightFacingOfThis());
	}

	////////////////////////////////////////////////////////////////////////////

	Map* GameState::GetCurrentMap() const
	{
		return currentMap_;
	}

	////////////////////////////////////////////////////////////////////////////

	Position* GameState::GetPlayerPosition() const
	{
		return playerPosition_;
	}
} // end namespace

