#pragma once

#include <vector>
#include "GameObject.h"

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
	Grid(int map_width, int map_height, int cell_width = 264, int cell_height = 250);
	~Grid();
	void PushObjIntoGrid(vector<LPGAMEOBJECT> lists);
	void GetObjFromGrid(float Camx,float Camy,vector<LPGAMEOBJECT>& lists);
};


