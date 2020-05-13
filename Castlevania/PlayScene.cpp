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
	int camx, camy, camx1, camy1,widthgrid,heightgrid;
	for (int i = (map-1); i <= (map - 1); i++)
	{
		for (int j = 0; j < FileInFMap[i].size(); j++)
		{
			if (j == 0) { idMap = atoi(FileInFMap[i][j].c_str()); }
			if (j == 1) { sceneFilePath = ToLPCWSTR(FileInFMap[i][j]); }
			if (j == 2) { camx = atoi(FileInFMap[i][j].c_str()); }
			if (j == 3) { camy = atoi(FileInFMap[i][j].c_str()); }	
			if (j == 4) { camx1 = atoi(FileInFMap[i][j].c_str()); }
			if (j == 5) { camy1 = atoi(FileInFMap[i][j].c_str()); }
			if (j == 6) { widthgrid = atoi(FileInFMap[i][j].c_str()); }
			if (j == 7) { heightgrid = atoi(FileInFMap[i][j].c_str()); }
		}
	}
	CGame::GetInstance()->SetKeyHandler(this->GetKeyEventHandler());
	if (simon->IdCurrMap < simon->IdNextMap)
	{
		CGame::GetInstance()->SetCamPos(camx,/*cy*/ camy);
	}
	else if (simon->IdCurrMap > simon->IdNextMap)
	{
		CGame::GetInstance()->SetCamPos(camx1,/*cy*/ camy1);
	}
	grid = new Grid(widthgrid, heightgrid);
	Load();
	grid->PushObjIntoGrid(listObjects);
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
		obj->ani = st;
		//obj->SetEnable(true);
		//obj->SetState(st);
		listObjects.push_back(obj);
		//objects.push_back(obj);
		//unit = new Unit(grid, obj, x, y);
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
		//obj->SetEnable(true);
		//unit = new Unit(grid, listobj, x, y);
		listObjects.push_back(listobj);
		//listobjects.push_back(listobj);
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		int switchmap = atof(tokens[3].c_str());
		obj = new Gate();
		obj->SetPosition(x, y);
		obj->IdNextMap = switchmap;
		//obj->SetEnable(true);
		//unit = new Unit(grid, obj, x, y);
		listObjects.push_back(obj);
		//objects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_STAIR:
	{
		int st = atof(tokens[4].c_str()); //state
		obj = new Stair();
		obj->SetPosition(x, y);
		obj->ani = st;
		//obj->SetEnable(true);
		//unit = new Unit(grid, obj, x, y);
		/*if (st == 0 || st == 1 || st == 5 || st == 6)
		{
			listStairsUp.push_back(obj);
		}
		else
			listStairsDown.push_back(obj);*/
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BREAKBRICK:
	{
		int st = atof(tokens[3].c_str());
		int id = atof(tokens[4].c_str());
		obj = new BreakBrick();
		obj->SetPosition(x, y);
		obj->ani = st;
		obj->IDitems = id;
		//obj->SetEnable(true);
		//unit = new Unit(grid, obj, x, y);
		//listobjects.push_back(obj);
		//objects.push_back(obj);
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MOVINGPLATFORM:
	{
		int st = atof(tokens[3].c_str()); //state
		obj = new MovingPlatform();
		// General object setup
		obj->SetPosition(x, y);
		obj->ani = st;
		//obj->SetEnable(true);
		//unit = new Unit(grid, obj, x, y);
		//objects.push_back(obj);
		listObjects.push_back(obj);
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	
}


void CPlayScene::GetObjectFromGrid()
{
	ListObjects.clear();
	listStairsUp.clear();
	listStairsDown.clear();
	AllObjects.clear();
	CGame* game = CGame::GetInstance();
	grid->GetObjFromGrid(game->GetCamPosX(), game->GetCamPosY(), ListObjects);
	//DebugOut(L"%d \n", ListObjects.size());

	for (UINT i = 0; i < ListObjects.size(); i++)
	{
		LPGAMEOBJECT obj = ListObjects[i];

		/*if (dynamic_cast<Ground*>(obj))
			continue;*/
		if (dynamic_cast<Stair*>(obj))
		{
			if (obj->ani == 0 || obj->ani == 1 || obj->ani == 5 || obj->ani == 6)
			{
				listStairsUp.push_back(obj);
			}
			else
				listStairsDown.push_back(obj);
		}
		else
			AllObjects.push_back(obj);
	}
}

//void CPlayScene::UpdateGrid()
//{
//	for (int i = 0; i < ListObjects.size(); i++)
//	{
//		LPGAMEOBJECT obj = ListObjects[i]->GetObj();
//
//		if (obj->IsEnable() == false)
//			continue;
//
//		float newPos_x, newPos_y;
//		obj->GetPosition(newPos_x, newPos_y);
//		ListObjects[i]->Move(newPos_x, newPos_y);
//	}
//}

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
	GetObjectFromGrid();
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	//Simon *simon = ((CPlayScene*)scene)->simon;//
	/*vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}*/

	for (size_t i = 0; i < AllObjects.size(); i++)
	{
		AllObjects[i]->Update(dt, &AllObjects);
	}
	simon->Update(dt, &AllObjects);
	/*for (size_t i = 0; i < listobjects.size(); i++)
	{
		listobjects[i]->Update(dt, &coObjects);
	}*/
	float simonx, simony;
	simon->GetPosition(simonx, simony);
	bool isSimonStand = true;

	if (simon->GetState() == SIMON_SITDOWN || simon->GetState() == SIMON_SIT_ATK)
		isSimonStand = false;

	simon->GetWhip()->SetWhipPosition(simon->nx,simonx, simony, isSimonStand);
	if (((simon->animation_set->at(SIMON_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_ATK) || ((simon->animation_set->at(SIMON_SIT_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_SIT_ATK) || ((simon->animation_set->at(SIMON_STAIRUP_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_STAIRUP_ATK) || ((simon->animation_set->at(SIMON_STAIRDOWN_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_STAIRDOWN_ATK))
	{		
		if (simon->isAtkWithWhip == true)
		{
			simon->GetWhip()->Update(dt, &AllObjects);
		}
	}
	//simon->GetSubWeapon()->Update(dt, &listobjects);
	for (int i = 0; i < 3; i++)
	{
		simon->GetListSubWeapon()[i]->Update(dt, &AllObjects);
	}
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
	for (UINT i = 0; i < AllObjects.size(); i++)
	{
		LPGAMEOBJECT obj = AllObjects[i];
		if (dynamic_cast<Candle*>(obj) && obj->GetState() == CANDLE_DESTROYED && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			int IdItems = obj->GetIDitems();
			listItems.push_back(DropItems(IdItems,obj->GetPositionX(), obj->GetPositionY()));
		}
		if (dynamic_cast<BreakBrick*>(obj) && obj->isDone == true && obj->isEnable == false)
		{
			obj->isEnable = true;
			int IdItems = obj->GetIDitems();
			if (IdItems == 16)
			{
				listItems.push_back(DropItems(IdItems,160, 390));
			}
			else if (IdItems != -1)
			{
				listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY()));
			}
		}
	}
	simon->SimonColliWithItems(&listItems);
	for (int i = 0; i < listItems.size(); i++)
	{
		listItems[i]->Update(dt, &AllObjects);
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
	//UpdateGrid();
}

void CPlayScene::Render()
{	
	tilemaps->Get((idMap*10000))->Draw();//
	int nx = 0;
	for (int i = 0; i < listItems.size(); i++)
		listItems[i]->Render();
	for (int i = 0; i < AllObjects.size(); i++)
		AllObjects[i]->Render();
	for (int i = 0; i < listStairsUp.size(); i++)
		listStairsUp[i]->Render();
	for (int i = 0; i < listStairsDown.size(); i++)
		listStairsDown[i]->Render();
	/*for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();*/

	simon->Render();
	if (simon->IsAtk() == true)///////////////
	{
		if (simon->isAtkWithWhip == true)
		{
			simon->GetWhip()->Render(simon->nx, simon->animation_set->at(simon->GetState())->GetCurrentFrame());
		}
	}
	else
		simon->GetWhip()->Render(simon->nx, -1);
	for (int i = 0; i < 3; i++)
	{
		simon->GetListSubWeapon()[i]->Render(simon->animation_set->at(simon->GetState())->GetCurrentFrame());
	}
	
	hud->Render(idMap, CGame::GetInstance()->GetCamPosX());
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < listStairsUp.size(); i++)
		delete listStairsUp[i];
	for (int i = 0; i < listStairsDown.size(); i++)
		delete listStairsDown[i];
	for (int i = 0; i < AllObjects.size(); i++)
		delete AllObjects[i];
	for (int i = 0; i < listItems.size(); i++)
		delete listItems[i];
	listStairsUp.clear();
	listStairsDown.clear();
	AllObjects.clear();
	listObjects.clear();
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
	vector<SubWeapon*> weaponlist = simon->GetListSubWeapon();
	SubWeapon * subweapon;

	float simonx, simony;
	simon->GetPosition(simonx, simony);
	bool isSimonStand = true;

	if (simon->GetState() == SIMON_SITDOWN || simon->GetState() == SIMON_SIT_ATK)
		isSimonStand = false;

	if (weaponlist[0]->IsEnable() == false)
		subweapon = weaponlist[0];
	else if ((weaponlist[1]->IsEnable() == false && (simon->GetSimonDoubleTri()==0 || simon->GetSimonDoubleTri() == 1)))
		subweapon = weaponlist[1];
	else if (weaponlist[2]->IsEnable() == false && simon->GetSimonDoubleTri() == 1)
		subweapon = weaponlist[2];
	else return;

	if (simon->Getsubweapon() != -1)
	{
		if (simon->IsAtk() == true) return;
		if (subweapon->isDone == false) return;
		if (simon->GetMana() >= 1)
		{
			int tam = simon->GetMana() - 1;
			simon->SetMana(tam);
			simon->isAtkWithSW = true;
			subweapon->nx = simon->nx;
			subweapon->SetState(simon->Subweapon);
			subweapon->SetWeaponPosition(simonx, simony, isSimonStand);
			subweapon->isDone = false; 
			subweapon->SetEnable(true);
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

bool CPlaySceneKeyHandler::StairCollisionsDetectionUp()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	CPlayScene * playscene = ((CPlayScene*)scene);
	simon->CantMoveDown = false;
	return simon->SimonColliWithStair(&playscene->GetListStairUp());
}

bool CPlaySceneKeyHandler::StairCollisionsDetectionDown()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	CPlayScene * playscene = ((CPlayScene*)scene);
	return simon->SimonColliWithStair(&playscene->GetListStairDown());
}

void CPlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
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
	case DIK_E:
		simon->IdCurrMap = 3;
		simon->IdNextMap = 4;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap());
		break;
	case DIK_R:
		simon->IdCurrMap = 4;
		simon->IdNextMap = 5;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap());
		break;
	case DIK_T:
		simon->IdCurrMap = 5;
		simon->IdNextMap = 6;
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
	case DIK_8:
		simon->SimonDoubleTri = 0;
		break;
	case DIK_9:
		simon->SimonDoubleTri = 1;
		break;
	}
}

void CPlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

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

		if (StairCollisionsDetectionDown() == true && simon->CantMoveDown == true)
		{
			Simon_StairDown();
			return;
		}

		if (StairCollisionsDetectionUp() == true)
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
		if (StairCollisionsDetectionUp() == true )
		{
			simon->CantMoveDown = false;
			Simon_StairUp();
			return;
		}

		if (StairCollisionsDetectionDown() == true)
		{
			simon->CantMoveDown = false;
			Simon_StairUp();
			return;
		}
		simon->SetState(SIMON_IDLE);
		return;
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if ((StairCollisionsDetectionDown() == true || StairCollisionsDetectionUp() == true)&& simon->isOnStair == true)
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
		return;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if ((StairCollisionsDetectionDown() == true || StairCollisionsDetectionUp() == true) && simon->isOnStair == true)
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
		return;
	}
	else
	{
		if ((StairCollisionsDetectionDown() == true || StairCollisionsDetectionUp() == true))
		{
			if (Simon_StandOnStair() == true)
				return;
		}
		simon->SetState(SIMON_IDLE);
		return;
	}
}

