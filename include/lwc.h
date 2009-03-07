
// CODESTYLE: v2.0

// lwc.h
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: Master Include

/**
 * @file lwc.h
 * @brief master Include - Header
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#ifndef __LWC_H__
#define __LWC_H__

	// C STANDARD LIBRARY
	#include <cstdio>
	#include <cstdlib>
	#include <cstring>
	#include <cstdarg>
	#include <ctime>
	#include <cmath>
	#include <cctype>

	// STL
	#include <vector>
	#include <string>
	#include <map>
	#include <algorithm>

	// SDL
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>

	////////////////////////////////////////////////////////////////////////////////

	// error log handling
	#if !defined(CONFIG_DISABLE_LOGGING)
		#define WriteLog fprintf
	#else
		static void DisabledStdErrLogFunction(FILE* fp, const char* format, ...){}
		#define WriteLog DisabledStdErrLogFunction
	#endif


	// GAME
	#include "Map.h"
	#include "MapView.h"
	#include "MiniMap.h"
	#include "Position.h"
	#include "WallSpriteSet.h"
	#include "ArtManager.h"
	#include "BitmapFont.h"
	#include "GameState.h"
	#include "Engine.h"
		
	
#endif

