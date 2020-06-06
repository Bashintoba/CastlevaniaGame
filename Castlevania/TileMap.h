#pragma once
#include"Sprites.h"
#include"Textures.h"
#include"debug.h"
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include"Game.h"
#include "Define.h"

class TileMap
{
	CSprites * sprites = CSprites::GetInstance();
	LPCWSTR MapFilePath;
	LPCWSTR MapPNG;
	int Map_Width, Map_Height;
	int Num_Rows, Num_Cols;
	int num_row_read, num_col_read;
	int Tile_Width, Tile_Height;
	int Id;
	int R, G, B;

	vector<vector<LPSPRITE>> tilemap;

public:
	TileMap(int ID, LPCWSTR FilePath_tex, LPCWSTR FilePath_data, int Map_width,int  Map_height, int Num_row_read,int Num_col_read,int R,int G,int B, int Tile_width = 32, int Tile_height = 32);
	~TileMap();

	void Load();
	void LoadMap();
	void Draw();	
	int GetMapWidth() { return Map_Width; }
};

typedef TileMap *LPTILEMAP;

class TileMaps
{
	static TileMaps * _instance;
	unordered_map<int, LPTILEMAP> tilemaps;

public:
	void Add(int ID, LPCWSTR FilePath_tex, LPCWSTR FilePath_data, int Map_width, int  Map_height, int Num_row_read, int Num_col_read,int R,int G,int B, int Tile_width = 32, int Tile_height = 32);
	LPTILEMAP Get(int ID) { return tilemaps[ID]; }

	static TileMaps * GetInstance();
};