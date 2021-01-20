#pragma once
#include "Cell.h"
#include "Utils.h"
#include "Game.h"




#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2
#define MAX_GRID_LINE 1024

class CGrid
{
	int numRow, numCol;
	int cellWidth;
	int  cellHeight;
	Cell** cells;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	CGrid() {}
	CGrid(LPCWSTR filePath);
	void GetObjects(vector<LPGAMEOBJECT>& listObject, int camX, int camY);
	void Load(LPCWSTR filePath);
	void Unload();
};