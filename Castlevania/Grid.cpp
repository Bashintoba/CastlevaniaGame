﻿#include "Grid.h"

Grid::Grid(int map_width, int map_height, int cell_width, int cell_height)
{
	this->map_width = map_width;
	this->map_height = map_height;

	this->cell_width = cell_width;
	this->cell_height = cell_height;

	nums_col = map_width / cell_width;
	nums_row = map_height / cell_height;

	cells.resize(nums_row);

	for (int i = 0; i < nums_row; i++)
	{
		cells[i].resize(nums_col);
	}
}

Grid::~Grid()
{
}

void Grid::pushObjIntoGrid(LPGAMEOBJECT obj, int col, int row)
{
	cells[row][col].push_back(obj);
}

void Grid::GetObjFromGrid(float Camx,float Camy,vector<LPGAMEOBJECT>& lists)
{
	int start_col = (int)(Camx / cell_width);
	int end_col = ceil((Camx + SCREEN_WIDTH) / cell_width);

	for (int i = 0; i < nums_row; i++)
	{
		for (int j = start_col; j < end_col; j++)
		{
			for (int k = 0; k < cells[i][j].size(); k++)
			{
				lists.push_back(cells[i][j][k]);
			}
		}
	}
}

void Grid::ResetGrid(vector<LPGAMEOBJECT> lists)
{
	for (int i = 0; i < nums_row; i++)
	{
		for (int j = 0; j < nums_col; j++)
		{
			cells[i][j].clear();
		}
	}

	for (int i = 0; i < lists.size(); i++)
	{
		if (lists[i]->isDone == false)
		{
			int row = (int)(lists[i]->GetPositionY() / cell_height);
			int col = (int)(lists[i]->GetPositionX() / cell_width);

			cells[row][col].push_back(lists[i]);
		}
	}
}




