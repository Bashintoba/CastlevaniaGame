#include "TileMap.h"

TileMap::TileMap(int ID, LPCWSTR FilePath_tex, LPCWSTR FilePath_data, int Map_width, int  Map_height, int Num_row_read, int Num_col_read, int Tile_width , int Tile_height )
{
	this->Id = ID;

	this->MapPNG = FilePath_tex;
	this->MapFilePath = FilePath_data;
	this->Map_Width = Map_width;
	this->Map_Height = Map_height;
	this->num_row_read = Num_row_read;
	this->num_col_read = Num_col_read;
	this->Tile_Width = Tile_width;
	this->Tile_Height = Tile_height;
	this->Num_Cols = Map_Width / Tile_Width;
	this->Num_Rows = Map_Height / Tile_Height;
	LoadMap();
	Load();
}
void TileMap::Load()
{
	tilemap.clear();
	DebugOut(L"[INFO] Start loading map resources from : %s \n", MapFilePath);
	ifstream f;
	f.open(MapFilePath);
	if (f.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", Id);
		f.close();
		return;
	}
	char str[MAX_SCENE_LINE];
	
	while (f.getline(str, MAX_SCENE_LINE))
	{		
		string line(str);
		vector<string> tokens = filemap(line);
		vector<LPSPRITE> spriteline;
		for (int i = 0; i < tokens.size(); i++)
		{
			int idTile = Id + atoi(tokens[i].c_str());
			spriteline.push_back(sprites->Get(idTile));
			
		}
		tilemap.push_back(spriteline);
	}

	f.close();

	DebugOut(L"[INFO] Done loading map resources %s\n", MapFilePath);
}

void TileMap::LoadMap()
{
	CTextures * texture = CTextures::GetInstance();
	texture->Add(Id, MapPNG, D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(Id);

	int id_sprite = 1;
	for (UINT i = 0; i < num_row_read; i++)
	{
		for (UINT j = 0; j < num_col_read; j++)
		{		
			int id_Sprite = Id + id_sprite;
			sprites->Add(id_Sprite, Tile_Width * j, Tile_Height * i, Tile_Width * (j + 1), Tile_Height * (i + 1), texTileMap);
			id_sprite = id_sprite + 1;
		}
	}

}

void TileMap::Draw()
{

	int firstcol = (int)CGame::GetInstance()->GetCamPosX() / 32;
	int lastcol = firstcol + (SCREEN_WIDTH / 32);

	for (UINT i = 0; i < Num_Rows; i++)
	{
		for (UINT j = firstcol; j <= lastcol; j++)
		{
			float x = Tile_Width * (j - firstcol) + CGame::GetInstance()->GetCamPosX() - (int)CGame::GetInstance()->GetCamPosX() % Tile_Width;
			float y = Tile_Height * i + 80;

			tilemap[i][j]->Draw(-1, x, y);
		}
	}


}

TileMap::~TileMap()
{
}

TileMaps * TileMaps::_instance = NULL;

void TileMaps::Add(int ID, LPCWSTR FilePath_tex, LPCWSTR FilePath_data,int Map_width,int Map_height, int Num_row_read, int Num_col_read, int Tile_width, int Tile_height)
{
	LPTILEMAP tilemap = new TileMap(ID, FilePath_tex, FilePath_data, Map_width, Map_height,Num_row_read, Num_col_read, Tile_width, Tile_height);
	tilemaps[ID] = tilemap;
}

TileMaps * TileMaps::GetInstance()
{
	if (_instance == NULL) _instance = new TileMaps();
	return _instance;
}