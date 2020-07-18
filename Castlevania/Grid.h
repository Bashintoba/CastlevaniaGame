#pragma once

#include <vector>
#include "GameObject.h"
#include "Define.h"

using namespace std;

class Grid
{
	int map_width;
	int map_height;

	int cell_width;
	int cell_height;

	int nums_col;
	int nums_row;

	vector<vector<vector<LPGAMEOBJECT>>> cells;

public:
	Grid(int map_width, int map_height, int cell_width = GRID_CELL_WIDTH, int cell_height = GRID_CELL_HEIGHT);
	~Grid();
	void pushObjIntoGrid(LPGAMEOBJECT obj, int row, int col);
	void GetObjFromGrid(float Camx,float Camy, vector<LPGAMEOBJECT>& lists);
	void ResetGrid(vector<LPGAMEOBJECT> lists);
};


