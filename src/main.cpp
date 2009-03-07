
// CODESTYLE: v2.0

// main.cpp
// Project: C++ SDL Port of Scrim's LoFiWanderings Game Project (LOFI)
// Author: Richard Marks
// Purpose: Project Program Entry Point

/**
 * @file main.cpp
 * @brief Project Program Entry Point
 * @author Richard Marks <ccpsceo@gmail.com>
 */

#include "lwc.h"

////////////////////////////////////////////////////////////////////////////////

LOFI::Engine* LOFI::globalEngineInstance = 0;
FILE* stderrLog = 0;

void OpenStdErrLog();
void CloseStdErrLog();

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	OpenStdErrLog();
	atexit(CloseStdErrLog);
	atexit(SDL_Quit);

	LOFI::Engine engine;
	LOFI::globalEngineInstance = &engine;

	if (engine.Initialize(argc, argv))
	{
		engine.Execute();
		engine.Destroy();
	}
	else
	{
		WriteLog(stderr, "Engine Initialization Failed!\n");
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void OpenStdErrLog()
{
	stderrLog = freopen("stderr.log", "a", stderr);
	if (stderrLog)
	{
		WriteLog(stderr, "Starting.\n");
	}
}

////////////////////////////////////////////////////////////////////////////////

void CloseStdErrLog()
{
	if (stderrLog)
	{
		WriteLog(stderr, "Stopping.\n");
		fclose(stderr);
	}
}

