#pragma once
#ifndef playscene_h
#define playscene_h

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Skeleton.h"
#include "Zombie.h"
#include "Raven.h"
#include "Ghost.h"
#include "Monkey.h"
#include "Knight.h"
#include "Darkenbat.h"
#include "Grid.h"
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
	Grid* grid;
	CPlayScene *playscene;
	int idMap;
	int Switchmap = 0;
	bool Simonisdead = false;
	TileMaps *tilemaps = TileMaps::GetInstance();
	vector<LPGAMEOBJECT> ListObjects;//dung de lay khoi grid
	
	vector<LPGAMEOBJECT> listObjects;//dung de push vao grid
	vector<LPGAMEOBJECT> listItems;//ds item
	vector<LPGAMEOBJECT> listStairsUp;//ds cau thang
	vector<LPGAMEOBJECT> listStairsDown;//ds cau thang
	vector<vector<string>> FileInfMap;
	vector<vector<string>> FileInfClearMap;

	

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_CLEARTEXTURES(string line);
	void _ParseSection_CLEARSPRITES(string line);
	void _ParseSection_CLEARANIMATIONS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	CPlayScene(int map, vector<vector<string>> FileInFMap, vector<vector<string>> FileInFClearMap);
	int RandomItems();
	Items* DropItems(int iditems ,float x, float y);
	void LoadPlayer();
	Timer* stopWatchTimer = new Timer(2000);
	Timer* simonDeadTimer = new Timer(3000);
	Timer* crossTimer = new Timer(500);
	void SwitchMap(int map, vector<vector<string>> FileInFMap, vector<vector<string>> FileInFClearMap);
	vector<vector<string>>  GetFileInFMap() {return FileInfMap;}
	vector<vector<string>> GetFileClearMap() { return FileInfClearMap; }
	vector<LPGAMEOBJECT> GetListStairUp() { return listStairsUp; }
	vector<LPGAMEOBJECT> GetListStairDown() { return listStairsDown; }
	vector<LPGAMEOBJECT> AllObjects;
	void GetObjectFromGrid();//
	void UpdateGrid();
	bool IsInCam(LPGAMEOBJECT object);
	void Cross();
	void Clear(int map, vector<vector<string>> FileInFClearMap);
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

#endif