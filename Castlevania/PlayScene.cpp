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
		simon->SetPosition(x, y);
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
		int id = atof(tokens[4].c_str());
		listobj = new Candle();
		listobj->SetPosition(x, y);
		listobj->IDitems = id;
		listobjects.push_back(listobj);
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		int switchmap = atof(tokens[3].c_str());
		obj = new Gate();
		obj->SetPosition(x, y);
		Switchmap = switchmap;
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

	simon->GetWhip()->SetWhipPosition(simonx, simony, isSimonStand);
	if (((simon->animation_set->at(SIMON_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_ATK) || ((simon->animation_set->at(SIMON_SIT_ATK)->GetCurrentFrame() == 2) && simon->state == SIMON_SIT_ATK))
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
		
		SwitchMap(Switchmap,FileInfMap);//
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
	hud->Render(idMap,CGame::GetInstance()->GetCamPosX());

	//tilemaps->Get((idMap*10000))->Draw();//
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
	/*else
		simon->GetWhip()->Render(simon->nx, -1);*/
	simon->GetSubWeapon()->Render(simon->GetSubWeapon()->nx, simon->animation_set->at(simon->GetState())->GetCurrentFrame());
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
		playscene->SwitchMap(2, playscene->GetFileInFMap());
		break;
	case DIK_1:
		simon->Subweapon = 0;
		break;
	}
}

void CPlaySceneKeyHandler::OnKeyUp(int KeyCode)
{}

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
		}
	}
		
}

void CPlaySceneKeyHandler::Simon_StairDown()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	int stairDirection = simon->stairnx;

	if (simon->CanMoveDown == false)
	{
		if (simon->isOnStair == true)
			simon->SetState(SIMON_IDLE);
		else
			simon->SetState(SIMON_SITDOWN);

		return;
	}

	// Auto-walk của Simon đi đến đúng đầu cầu thang rồi mới bước xuống
	if (simon->isOnStair == false)
	{
		float stair_x, simon_x, temp_y;

		simon->stairCollided->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == -1) stair_x -= 28.0f;

		if (stair_x < simon_x) simon->nx = -1;
		else if (stair_x > simon_x) simon->nx = 1;
		else return;

		simon->SetState(SIMON_WALKING);
		simon->vy = 0;
		simon->AutoWalk(stair_x - simon_x, SIMON_STAIRDOWN, -stairDirection);
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
	int stairDirection = simon->stairnx;

	if (simon->CanMoveUp == false)
	{
		if (simon->isOnStair == true)
		{
			int nx = simon->stairnx;
			simon->nx = nx;
			simon->SetState(SIMON_STAIRUP);
			simon->AutoWalk(14 * nx, SIMON_IDLE, nx);
		}

		return;
	}

	// Auto-walk của Simon đi đến đúng chân cầu thang rồi mới bước lên
	if (simon->isOnStair == false)
	{
		float stair_x, simon_x, temp_y;

		simon->stairCollided->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == 1)
		{
			stair_x -= 31.0f;
		}
		else stair_x += 5.0f;

		if (stair_x < simon_x) simon->nx = -1;
		else if (stair_x > simon_x)  simon->nx = 1;
		else return;
		simon->SetState(SIMON_WALKING);
		simon->vy = 0;
		simon->AutoWalk(stair_x - simon_x, SIMON_STAIRUP, stairDirection);
		simon->isOnStair = true;

		return;
	}
	else
	{
		simon->nx = stairDirection;
		simon->SetState(SIMON_STAIRUP);
	}

	return;
}

bool CPlaySceneKeyHandler::Simon_Stand_On_Stair()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	if (simon->GetState() == SIMON_STAIRUP || simon->GetState() == SIMON_STAIRDOWN )
	{
		/*if (simon->GetState() == HIT_STAIR_UP)
		{
			simon->SetState(STAIR_UP);
			isNeedToWaitingAnimation = false;
		}
		else if (simon->GetState() == HIT_STAIR_DOWN)
		{
			simon->SetState(STAIR_DOWN);
			isNeedToWaitingAnimation = false;
		}*/

		simon->StandOnStair();
		simon->animation_set->at(SIMON_STAIRUP)->Reset();
		//simon->animations[STAIR_UP_INVISIBLE]->Reset();
		simon->animation_set->at(SIMON_STAIRDOWN)->Reset();
		//simon->animations[STAIR_DOWN_INVISIBLE]->Reset();

		return true;
	}

	return false;
}

bool CPlaySceneKeyHandler::StairCollisionsDetection()
{
	Simon *simon = ((CPlayScene*)scene)->simon;
	CPlayScene * playscene = ((CPlayScene*)scene);
	return simon->SimonColliWithStair(&playscene->GetListStair());
}

void CPlaySceneKeyHandler::KeyState(BYTE *states)
{
	
	CGame *game = CGame::GetInstance();
	Simon *simon = ((CPlayScene*)scene)->simon;

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
		if (StairCollisionsDetection() == true)
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
		if (StairCollisionsDetection() == true)
		{
			Simon_StairUp();
				return;
		}

		simon->SetState(SIMON_IDLE);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if (StairCollisionsDetection() == true && simon->isOnStair == true)
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
		if (StairCollisionsDetection() == true && simon->isOnStair == true)
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
		if (StairCollisionsDetection() == true)
		{
			if (Simon_Stand_On_Stair() == true)
				return;
		}
		simon->SetState(SIMON_IDLE);
	}
}

