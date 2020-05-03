#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "debug.h"
#include "Textures.h"
#include "Sprites.h"

using namespace std;

CPlayScene::CPlayScene(int map,vector<vector<string>> FileInFMap) :CScene()
{	
	key_handler = new CPlaySceneKeyHandler(this);
	this->FileInfMap = FileInFMap;
	LoadPlayer();
	SwitchMap(map,FileInfMap);
}

Items * CPlayScene::DropItems(int iditems, float x, float y)
{
	Items *a = new Items();
	a->SetPosition(x, y);
	a->SetState(iditems);
	return a;
}

void CPlayScene::LoadPlayer()
{
	if (simon == NULL)
	{
		simon = new Simon();		
		DebugOut(L"[INFO] Simon created! \n");
	}
	if (hud == NULL)
	{
		hud = new HUD(simon);
		DebugOut(L"[INFO] HUD created! \n");
	}
}
	

void CPlayScene::SwitchMap(int map, vector<vector<string>> FileInFMap)
{
	Unload();
	int camx, camy;
	for (int i = (map-1); i <= (map - 1); i++)
	{
		for (int j = 0; j < FileInFMap[i].size(); j++)
		{
			if (j == 0) { idMap = atoi(FileInFMap[i][j].c_str()); }
			if (j == 1) { sceneFilePath = ToLPCWSTR(FileInFMap[i][j]); }
			if (j == 2) { camx = atoi(FileInFMap[i][j].c_str()); }
			if (j == 3) { camy = atoi(FileInFMap[i][j].c_str()); }			
		}
	}
	CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
	CGame::GetInstance()->SetCamPos(camx,/*cy*/ camy);
	Load();
	simon->IdCurrMap = map;
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	CGameObject *obj = NULL;
	CGameObject *listobj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:	
	{
		int nx = atof(tokens[3].c_str());
		int nx1 = atof(tokens[7].c_str());
		int st = atof(tokens[4].c_str());
		int st1 = atof(tokens[8].c_str());
		float x1 = atof(tokens[5].c_str());
		float y1 = atof(tokens[6].c_str());

		if (simon->IdCurrMap < simon->IdNextMap)
		{
			simon->nx = nx;
			simon->SetPosition(x, y);
			simon->SetState(st);
			
		}
		else if (simon->IdCurrMap > simon->IdNextMap)
		{
			simon->nx = nx1;
			simon->SetPosition(x1, y1);
			simon->SetState(st1);
			
		}
		break;
	}
	case OBJECT_TYPE_BRICK: 
	{
		int st = atof(tokens[4].c_str()); //state
		obj = new Ground();
		// General object setup
		obj->SetPosition(x, y);
		obj->SetState(st);
		listobjects.push_back(obj);
		objects.push_back(obj);

		break;
	}
	case OBJECT_TYPE_CANDLE: 
	{
		int st = atof(tokens[3].c_str());
		int id = atof(tokens[4].c_str());
		listobj = new Candle();
		listobj->SetPosition(x, y);
		listobj->SetState(st);
		listobj->IDitems = id;
		listobjects.push_back(listobj);
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		int switchmap = atof(tokens[3].c_str());
		obj = new Gate();
		obj->SetPosition(x, y);
		obj->IdNextMap = switchmap;
		objects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_STAIR:
	{
		int st = atof(tokens[4].c_str()); //state
		obj = new Stair();
		obj->SetPosition(x, y);
		obj->SetState(st);
		listStairs.push_back(obj);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	
}


void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	//Simon *simon = ((CPlayScene*)scene)->simon;//
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	simon->Update(dt, &coObjects);
	for (size_t i = 0; i < listobjects.size(); i++)
	{
		listobjects[i]->Update(dt, &coObjects);
	}
	float simonx, simony;
	simon->GetPosition(simonx, simony);
	bool isSimonStand = true;

	if (simon->GetState() == SIMON_SITDOWN || simon->GetState() == SIMON_SIT_ATK)
		isSimonStand = false;

	simon->GetWhip()->SetWhipPosition(simon->nx,simonx, simony, isSimonStand);
	if (((simon->animation_set->at(SIMON_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_ATK) || ((simon->animation_set->at(SIMON_SIT_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_SIT_ATK) || ((simon->animation_set->at(SIMON_STAIRUP_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_STAIRUP_ATK) || ((simon->animation_set->at(SIMON_STAIRDOWN_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_STAIRDOWN_ATK))
	{
		if (simon->isAtkWithSW == true)
		{
			simon->GetSubWeapon()->SetWeaponPosition(simon->nx, simonx, simony, isSimonStand);
			simon->GetSubWeapon()->isDone = false;
		}
		else if (simon->isAtkWithWhip == true)
		{
			simon->GetWhip()->Update(dt, &listobjects);
		}
	}
	simon->GetSubWeapon()->Update(dt, &listobjects);
	if (simon->isGotChainItem == true) // update trạng thái của whip
	{
		simon->IsWait = true;
		simon->isGotChainItem = false;
		simon->GetWhip()->PowerUp();
	}
	if (simon->animation_set->at(SIMON_HENSHIN)->IsOver(450)== true)
	{
		simon->IsWait = false;
	}
	for (UINT i = 0; i < listobjects.size(); i++)
	{
		LPGAMEOBJECT obj = listobjects[i];
		if (dynamic_cast<Candle*>(obj) && obj->GetState() == CANDLE_DESTROYED && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			int IdItems = obj->GetIDitems();
			listItems.push_back(DropItems(IdItems,obj->GetPositionX(), obj->GetPositionY()));
		}
	}
	simon->SimonColliWithItems(&listItems);
	for (int i = 0; i < listItems.size(); i++)
	{
		listItems[i]->Update(dt, &coObjects);
	}
	if (simon->isChangeScene == true)
	{
		simon->isChangeScene = false;		
		SwitchMap(simon->IdNextMap,FileInfMap);//
	}
	hud->Update(dt);
	// Update camera to follow simon
	float cx, cy;
	simon->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();
	if (simon->x > SCREEN_WIDTH / 2 && simon->x + (SCREEN_WIDTH /2) < tilemaps->Get((idMap*10000))->GetMapWidth())//
	{
		CGame::GetInstance()->SetCamPos(simon->x - (SCREEN_WIDTH / 2),/*cy*/ 0.0f);
	}
}

void CPlayScene::Render()
{	
	tilemaps->Get((idMap*10000))->Draw();//
	int nx = 0;
	for (int i = 0; i < listItems.size(); i++)
		listItems[i]->Render();
	for (int i = 0; i < listobjects.size(); i++)
		listobjects[i]->Render();
	for (int i = 0; i < listStairs.size(); i++)
		listStairs[i]->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	simon->Render();
	if (simon->IsAtk() == true)///////////////
	{
		if (simon->isAtkWithSW == true)
		{
			nx = simon->nx;
			simon->GetSubWeapon()->nx = nx;	
		}
		else if (simon->isAtkWithWhip == true)
		{
			simon->GetWhip()->Render(simon->nx, simon->animation_set->at(simon->GetState())->GetCurrentFrame());
		}
	}
	else
		simon->GetWhip()->Render(simon->nx, -1);
	simon->GetSubWeapon()->Render(simon->GetSubWeapon()->nx, simon->animation_set->at(simon->GetState())->GetCurrentFrame());
	hud->Render(idMap, CGame::GetInstance()->GetCamPosX());
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < listStairs.size(); i++)
		delete listStairs[i];
	for (int i = 0; i < listobjects.size(); i++)
		delete listobjects[i];
	for (int i = 0; i < listItems.size(); i++)
		delete listItems[i];
	listStairs.clear();
	objects.clear();
	listobjects.clear();
	listItems.clear();
}

void CPlaySceneKeyHandler::Simon_Atk()
{
	Simon *simon = ((CPlayScene*)scene)->simon;	
	if (simon->IsAtk() == true) return;

	simon->isAtkWithWhip = true;
	if (simon->GetState() == SIMON_IDLE || simon->GetState() == SIMON_JUMP || simon->GetState() == SIMON_WALKING)
	{
		simon->SetState(SIMON_ATK);
	}
	else if (simon->GetState() == SIMON_SITDOWN)
	{
		simon->SetState(SIMON_SIT_ATK);
	}
	else if (simon->GetState() == SIMON_STAIRUP)
	{
		simon->SetState(SIMON_STAIRUP_ATK);
	}
	else if (simon->GetState() == SIMON_STAIRDOWN)
	{
		simon->SetState(SIMON_STAIRDOWN_ATK);
	}
}

void CPlaySceneKeyHandler::Simon_SubAtk()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	if (simon->Getsubweapon() != -1)
	{
		if (simon->IsAtk() == true) return;
		if (simon->GetSubWeapon()->isDone == false) return;
		if (simon->GetMana() >= 1)
		{
			int tam = simon->GetMana() - 1;
			simon->SetMana(tam);
			simon->isAtkWithSW = true;
			simon->GetSubWeapon()->nx = simon->nx;
			simon->GetSubWeapon()->SetState(simon->Subweapon);
			if (simon->GetState() == SIMON_IDLE || simon->GetState() == SIMON_JUMP || simon->GetState() == SIMON_WALKING)
			{
				simon->SetState(SIMON_ATK);
			}
			else if (simon->GetState() == SIMON_SITDOWN)
			{
				simon->SetState(SIMON_SIT_ATK);
			}
			else if (simon->GetState() == SIMON_STAIRUP)
			{
				simon->SetState(SIMON_STAIRUP_ATK);
			}
			else if (simon->GetState() == SIMON_STAIRDOWN)
			{
				simon->SetState(SIMON_STAIRDOWN_ATK);
			}
		}
	}
		
}

void CPlaySceneKeyHandler::Simon_StairDown()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	int Stairnx = simon->stairnx; //hướng cầu thang

	if (simon->CanMoveDown == false)//ra khỏi cầu thang
	{
		if (simon->isOnStair == true)
		{
			//float stairx, stairy;
			//simon->StairIsCollided->GetPosition(stairx, stairy);
			///*if (Stairnx == 1)
			//{
			//	stairx += 5.0f;
			//}
			//else stairx -= 30.0f;*/
			//simon->nx = simon->stairnx;
			//simon->SetState(SIMON_STAIRDOWN);
			//simon->AutoWalk(stairx, SIMON_IDLE, -simon->nx);
				simon->SetState(SIMON_IDLE);
		}
		else
			simon->SetState(SIMON_SITDOWN);
		return;
	}
	// Auto-walk cho Simon đi đến đúng đầu cầu thang 
	if (simon->isOnStair == false)
	{
		float stairx, simonx, stairy;
		simon->GetPosition(simonx, stairy);
		simon->StairIsCollided->GetPosition(stairx, stairy);
		if (Stairnx == 1)
		{
			stairx += 4.0f;
		}
		else stairx -= 28.0f;

		if (stairx < simonx) simon->nx = -1;
		else if (stairx > simonx) simon->nx = 1;
		simon->vy = 0;
		simon->SetState(SIMON_WALKING);
		simon->AutoWalk(stairx, SIMON_STAIRDOWN, -Stairnx);
		simon->isOnStair = true;

		return;
	}
	else
	{
		simon->nx = -simon->stairnx;
		simon->SetState(SIMON_STAIRDOWN);
	}

	

	return;
}

void CPlaySceneKeyHandler::Simon_StairUp()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	int Stairnx = simon->stairnx;

	if (simon->CanMoveUp == false)
	{
		if (simon->isOnStair == true)
		{
			float stairx, stairy;
			simon->StairIsCollided->GetPosition(stairx, stairy);
			if (Stairnx == 1)
			{
				stairx += 5.0f;
			}
			else stairx -= 30.0f;
			simon->nx = simon->stairnx;
			simon->SetState(SIMON_STAIRUP);
			simon->AutoWalk(stairx, SIMON_IDLE, simon->nx);//tránh trường hợp ra khỏi cầu thang mà ko dụng vào mặt đất
		}

		return;
	}
	// Auto-walk cho Simon đi đến đúng chân cầu thang
	if (simon->isOnStair == false)
	{
		float stairx, simonx, stairy;
		simon->GetPosition(simonx, stairy);
		simon->StairIsCollided->GetPosition(stairx, stairy);
		if (Stairnx == 1)
		{
			stairx -= 31.0f;//simon đứng giữa cầu thang thì mới đúng animation 
		}
		else stairx += 4.0f;

		if (stairx < simonx) simon->nx = -1;
		else if (stairx > simonx)  simon->nx = 1;

		simon->vy = 0;
		simon->SetState(SIMON_WALKING);
		simon->AutoWalk(stairx, SIMON_STAIRUP, Stairnx);
		simon->isOnStair = true;

		return;
	}
	else
	{
		simon->nx = Stairnx;
		simon->SetState(SIMON_STAIRUP);
	}

	
	return;
}

bool CPlaySceneKeyHandler::Simon_StandOnStair()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	if (simon->GetState() == SIMON_STAIRUP || simon->GetState() == SIMON_STAIRDOWN|| simon->GetState() == SIMON_STAIRUP_ATK || simon->GetState() == SIMON_STAIRDOWN_ATK)
	{
		if (simon->GetState() == SIMON_STAIRDOWN_ATK)
		{
			simon->SetState(SIMON_STAIRDOWN);
			simon->animation_set->at(SIMON_STAIRDOWN)->SetAniStartTime(0);
		}
		else if (simon->GetState() == SIMON_STAIRUP_ATK)
		{
			simon->SetState(SIMON_STAIRUP);
			simon->animation_set->at(SIMON_STAIRUP)->SetAniStartTime(0);

		}
		
		simon->StandOnStair();
		simon->animation_set->at(SIMON_STAIRUP)->Reset();
		simon->animation_set->at(SIMON_STAIRDOWN)->Reset();
		return true;
	}

	return false;
}

void CPlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	CPlayScene * playscene = ((CPlayScene*)scene);
	if (simon->GetState() == SIMON_HENSHIN && simon->animation_set->at(SIMON_HENSHIN)->IsOver(TIME_HENSHIN) == false)
		return;
	if (simon->GetState() == SIMON_ATK && simon->animation_set->at(SIMON_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->GetState() == SIMON_SIT_ATK && simon->animation_set->at(SIMON_SIT_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->GetState() == SIMON_STAIRDOWN && simon->animation_set->at(SIMON_STAIRDOWN)->IsOver(200) == false)
		return;
	if (simon->GetState() == SIMON_STAIRUP && simon->animation_set->at(SIMON_STAIRUP)->IsOver(200) == false)
		return;
	if (simon->GetState() == SIMON_STAIRDOWN_ATK && simon->animation_set->at(SIMON_STAIRDOWN_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->GetState() == SIMON_STAIRUP_ATK && simon->animation_set->at(SIMON_STAIRUP_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->isAutoWalk == true)
		return;
	else
	{
		if (simon->isJumping == true)
		{
			simon->SetState(SIMON_JUMP);
		}
		simon->isAtk = false;
		simon->isAtkWithSW = false;
		simon->isAtkWithWhip = false;
	}
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame *game = CGame::GetInstance();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->isAtk == true || simon->isSitting == true || simon->isOnStair == true)
			return;
		simon->SetState(SIMON_JUMP);
		break;
	case DIK_A:
		if (game->IsKeyDown(DIK_UP))
		{
			Simon_SubAtk();
		}
		else
			Simon_Atk();
		break;
	case DIK_S:
		Simon_SubAtk();
		break;
	case DIK_Q:
		simon->IdCurrMap = 1;
		simon->IdNextMap = 2;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap());
		break;
	case DIK_W:
		simon->IdCurrMap = 2;
		simon->IdNextMap = 3;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap());
		break;
	case DIK_1:
		simon->Subweapon = 0;
		break;
	case DIK_2:
		simon->Subweapon = 1;
		break;
	case DIK_3:
		simon->Subweapon = 2;
		break;
	case DIK_4:
		simon->Subweapon = 3;
		break;
	case DIK_5:
		simon->Subweapon = 4;
		break;
	}
}

void CPlaySceneKeyHandler::OnKeyUp(int KeyCode)
{}

void CPlaySceneKeyHandler::KeyState(BYTE *states)
{
	
	CGame *game = CGame::GetInstance();
	Simon *simon = ((CPlayScene*)scene)->simon;
	CPlayScene * playscene = ((CPlayScene*)scene);

	if (simon->GetState() == SIMON_HENSHIN && simon->animation_set->at(SIMON_HENSHIN)->IsOver(TIME_HENSHIN)  == false)
		return;
	if (simon->GetState() == SIMON_ATK && simon->animation_set->at(SIMON_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->GetState() == SIMON_SIT_ATK && simon->animation_set->at(SIMON_SIT_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->GetState() == SIMON_STAIRDOWN && simon->animation_set->at(SIMON_STAIRDOWN)->IsOver(200) == false)
		return;
	if (simon->GetState() == SIMON_STAIRUP && simon->animation_set->at(SIMON_STAIRUP)->IsOver(200) == false)
		return;
	if (simon->GetState() == SIMON_STAIRDOWN_ATK && simon->animation_set->at(SIMON_STAIRDOWN_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->GetState() == SIMON_STAIRUP_ATK && simon->animation_set->at(SIMON_STAIRUP_ATK)->IsOver(TIME_DELAY) == false)
		return;
	if (simon->isAutoWalk == true)
		return;
	else
	{
		simon->isAtk = false;
		simon->isAtkWithSW = false;
		simon->isAtkWithWhip = false;
		if (simon->isJumping == true)
		{
			simon->SetState(SIMON_JUMP);
		}
	}

	if (simon->isAtk == true || simon->isJumping == true)
		return;//

	if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->SimonColliWithStair(&playscene->GetListStair()) == true)
		{
			Simon_StairDown();
			return;
		}

		simon->SetState(SIMON_SITDOWN);
		if (game->IsKeyDown(DIK_RIGHT))
		{
			simon->nx = 1;
			simon->GetWhip()->nx = 1;
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			simon->nx = -1;
			simon->GetWhip()->nx = -1;
		}
		return;
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (simon->SimonColliWithStair(&playscene->GetListStair()) == true )
		{
			Simon_StairUp();
				return;
		}

		simon->SetState(SIMON_IDLE);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->SimonColliWithStair(&playscene->GetListStair()) == true && simon->isOnStair == true)
		{
			if (simon->stairnx == 1) // cầu thang trái dưới - phải trên
				Simon_StairUp();
			else
				Simon_StairDown();

			return;
		}
		simon->SetState(SIMON_WALKING);
		simon->nx = 1;
		simon->GetWhip()->nx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->SimonColliWithStair(&playscene->GetListStair()) == true && simon->isOnStair == true)
		{
			if (simon->stairnx == 1) // cầu thang trái dưới - phải trên
				Simon_StairDown();
			else
				Simon_StairUp();

			return;
		}
		simon->SetState(SIMON_WALKING);
		simon->nx = -1;
		simon->GetWhip()->nx = -1;
	}
	else
	{
		if (simon->SimonColliWithStair(&playscene->GetListStair()) == true)
		{
			if (Simon_StandOnStair() == true)
				return;
		}
		simon->SetState(SIMON_IDLE);
	}
}

