#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "MovingPlatform.h"
#include "Simon.h"
#include "Ground.h"
#include "BreakBrick.h"
#include "Stair.h"
#include "Whip.h"
#include "Items.h"
#include "Candle.h"
#include "TileMap.h"
#include "Define.h"
#include "Gate.h"
#include "HUD.h"


class CPlayScene : public CScene
{
protected:
	Simon *simon;		// A play scene has to have player, right? 
	HUD *hud;
	CPlayScene *playscene;
	int idMap;
	int Switchmap = 0;
	TileMaps *tilemaps = TileMaps::GetInstance();
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listobjects;//
	vector<LPGAMEOBJECT> listItems;//ds item
	vector<LPGAMEOBJECT> listStairsUp;//ds cau thang
	vector<LPGAMEOBJECT> listStairsDown;//ds cau thang
	vector<vector<string>> FileInfMap;

	void _ParseSection_OBJECTS(string line);
public:
	CPlayScene(int map, vector<vector<string>> FileInFMap);
	Items* DropItems(int iditems ,float x, float y);
	void LoadPlayer();
	void SwitchMap(int map, vector<vector<string>> FileInFMap);
	vector<vector<string>>  GetFileInFMap() {return FileInfMap;}
	vector<LPGAMEOBJECT> GetListStairUp() { return listStairsUp; }
	vector<LPGAMEOBJECT> GetListStairDown() { return listStairsDown; }
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	friend class CPlaySceneKeyHandler;
};

class CPlaySceneKeyHandler : public CSceneKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	void Simon_Atk();
	void Simon_SubAtk();
	void Simon_StairDown();
	void Simon_StairUp();
	bool Simon_StandOnStair();
	bool StairCollisionsDetectionUp();
	bool StairCollisionsDetectionDown();
	CPlaySceneKeyHandler(CScene *s) :CSceneKeyHandler(s) {};
};

