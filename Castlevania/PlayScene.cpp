#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "debug.h"
#include "Textures.h"
#include "Sprites.h"

using namespace std;

CPlayScene::CPlayScene(int map,vector<vector<string>> FileInFMap, vector<vector<string>> FileInFClearMap) :CScene()
{	
	key_handler = new CPlaySceneKeyHandler(this);
	this->FileInfMap = FileInFMap;
	this->FileInfClearMap = FileInFClearMap;
	LoadPlayer();
	SwitchMap(map,FileInfMap, FileInfClearMap);
}

int CPlayScene::RandomItems()
{
	int random = rand() % 100;
	if (random <= 20)
		return 0;
	else if (20 < random && random <= 40)
		return 1;
	else if (40 < random && random <= 50)
		return 2;
	else if (50 < random && random <= 60)
		return 3;
	else if (60 < random && random <= 70)
		return 4;
	else if (70 < random && random <= 80)
		return 5;
	else if (80 < random && random <= 90)
		return 6;
	else if (90 < random && random <= 100)
		return 13;
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
	

void CPlayScene::SwitchMap(int map, vector<vector<string>> FileInFMap ,vector<vector<string>> FileInFClearMap)
{
	Unload();
	Clear(simon->IdCurrMap,FileInFClearMap);
	simon->IdCurrMap = simon->IdSwithMap;
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
	if (simon->IdCurrMap <= simon->IdNextMap)
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
	simon->IdCurrMap = simon->IdSwithMap = map;
}

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_CLEARTEXTURES(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CTextures::GetInstance()->Clear(atoi(tokens[i].c_str()));
}

void CPlayScene::_ParseSection_CLEARSPRITES(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CSprites::GetInstance()->Clear(atoi(tokens[i].c_str()));
}

void CPlayScene::_ParseSection_CLEARANIMATIONS(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CAnimations::GetInstance()->Clear(atoi(tokens[i].c_str()));
}

void CPlayScene::_ParseSection_CLEARANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
		CAnimationSets::GetInstance()->Clear(atoi(tokens[i].c_str()));
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

		if (simon->IdCurrMap <= simon->IdNextMap)
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
		int st = atof(tokens[4].c_str());
		obj = new Ground();
		obj->SetPosition(x, y);
		obj->ani = st;
		listObjects.push_back(obj);
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
		listObjects.push_back(listobj);
		break;
	}
	case OBJECT_TYPE_GATE:
	{
		int switchmap = atof(tokens[3].c_str());
		obj = new Gate();
		obj->SetPosition(x, y);
		obj->IdNextMap = switchmap;
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_STAIR:
	{
		int st = atof(tokens[4].c_str());
		obj = new Stair();
		obj->SetPosition(x, y);
		obj->ani = st;
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
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MOVINGPLATFORM:
	{
		int st = atof(tokens[3].c_str());
		obj = new MovingPlatform();
		obj->SetPosition(x, y);
		obj->ani = st;
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_KNIGHT:
	{
		int Xstart = atof(tokens[6].c_str());
		int Xend = atof(tokens[7].c_str());
		obj = new Knight(Xstart, Xend);
		int idaniset = atof(tokens[3].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(idaniset);
		obj->SetAnimationSet(ani_set);
		int st = atof(tokens[4].c_str());
		obj->SetState(st);
		int nX = atof(tokens[5].c_str());
		obj->nx = nX;	
		obj->SetPosition(x, y);
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_DARKENBAT:
	{
		int st = atof(tokens[4].c_str());
		obj = new Darkenbat(simon);
		int idaniset = atof(tokens[3].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(idaniset);
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		obj->SetState(st);
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_GHOST:
	{
		int st = atof(tokens[4].c_str());
		obj = new Ghost(simon);
		int idaniset = atof(tokens[3].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(idaniset);
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		obj->SetState(st);
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MONKEY:
	{
		int st = atof(tokens[4].c_str());
		obj = new Monkey(simon);
		int idaniset = atof(tokens[3].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(idaniset);
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		obj->SetState(st);
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_SKELETON:
	{
		int st = atof(tokens[4].c_str());
		obj = new Skeleton(simon);
		int idaniset = atof(tokens[3].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(idaniset);
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		obj->SetState(st);
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_RAVEN:
	{
		int st = atof(tokens[5].c_str());
		float dist = atof(tokens[4].c_str());
		obj = new Raven(simon,dist);
		int idaniset = atof(tokens[3].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(idaniset);
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		obj->SetState(st);
		listObjects.push_back(obj);
		break;
	}
	case OBJECT_TYPE_ZOMBIE:
	{
		int st = atof(tokens[4].c_str());
		obj = new Zombie(simon);
		int idaniset = atof(tokens[3].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(idaniset);
		obj->SetAnimationSet(ani_set);
		obj->SetPosition(x, y);
		obj->SetState(st);
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
	//DebugOut(L"%d \n", ListObjects.size());// đừng mở lag lắm :(((

	for (UINT i = 0; i < ListObjects.size(); i++)
	{
		LPGAMEOBJECT obj = ListObjects[i];

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

void CPlayScene::UpdateGrid()
{
	grid->ResetGrid(listObjects);
}

bool CPlayScene::IsInCam(LPGAMEOBJECT object)
{
	CGame* game = CGame::GetInstance();
	float x, y;
	object->GetPosition(x, y);
	if (object != simon)
		return x >= game->GetCamPosX() && x < (game->GetCamPosX() + SCREEN_WIDTH) && (y >= game->GetCamPosY() && y < (game->GetCamPosY() + SCREEN_HEIGHT));
	else
		return y >= game->GetCamPosY() && y < (game->GetCamPosY() + SCREEN_HEIGHT);
}

void CPlayScene::Cross()
{
	if (simon->isGotCross== true)
	{
		simon->isGotCross = false;
		crossTimer->Start();

		for (UINT i = 0; i < AllObjects.size(); i++)
		{
			// Cross chỉ tác dụng với các object hiện trên màn hình
			if (IsInCam(AllObjects[i]) == false)
				continue;

			if (dynamic_cast<Knight*>(AllObjects[i]) && AllObjects[i]->GetHP()>0)
			{
				auto knight = dynamic_cast<Knight*>(AllObjects[i]);
				knight->SetHP(0);
				knight->SetState(KNIGHT_STATE_DIE);
			}
			else if (dynamic_cast<Darkenbat*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
			{
				auto db = dynamic_cast<Darkenbat*>(AllObjects[i]);
				db->SetHP(0);
				db->SetState(DARKBAT_STATE_DIE);
			}
			else if (dynamic_cast<Monkey*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
			{
				auto monkey = dynamic_cast<Monkey*>(AllObjects[i]);
				monkey->SetHP(0);
				monkey->SetState(MONKEY_STATE_DIE);
			}
			else if (dynamic_cast<Ghost*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
			{
				auto ghost = dynamic_cast<Ghost*>(AllObjects[i]);
				ghost->SetHP(0);
				ghost->SetState(GHOST_STATE_DIE);
			}
			else if (dynamic_cast<Raven*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
			{
				auto raven = dynamic_cast<Raven*>(AllObjects[i]);
				raven->SetHP(0);
				raven->SetState(RAVEN_STATE_DIE);
			}
			else if (dynamic_cast<Zombie*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
			{
				auto zombie = dynamic_cast<Zombie*>(AllObjects[i]);
				zombie->SetHP(0);
				zombie->SetState(ZOMBIE_STATE_DIE);
			}
			else if (dynamic_cast<Skeleton*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
			{
				auto skeleton = dynamic_cast<Skeleton*>(AllObjects[i]);
				skeleton->SetHP(0);
				skeleton->SetState(SKELETON_STATE_DIE);
			}
		}
	}
}

void CPlayScene::Clear(int map, vector<vector<string>> FileInFClearMap)
{
	
	for (int i = (map-1); i <= (map-1); i++)
	{
		for (int j = 0; j < FileInFClearMap[i].size(); j++)
		{
			if (j == 0) { clearFilePath = ToLPCWSTR(FileInFClearMap[i][j]); }
		}
	}

	ifstream f;
	f.open(clearFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[CLEARTEXTURES]") { section = SCENE_SECTION_CLEARTEXT; continue; }
		if (line == "[CLEARSPRITES]") { section = SCENE_SECTION_CLEARSP; continue; }
		if (line == "[CLEARANIMATIONS]") { section = SCENE_SECTION_CLEARANI; continue; }
		if (line == "[CLEARANIMATION_SETS]") { section = SCENE_SECTION_CLEARANISET; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_CLEARTEXT:_ParseSection_CLEARTEXTURES(line); break;
		case SCENE_SECTION_CLEARSP: _ParseSection_CLEARSPRITES(line); break;
		case SCENE_SECTION_CLEARANI: _ParseSection_CLEARANIMATIONS(line); break;
		case SCENE_SECTION_CLEARANISET: _ParseSection_CLEARANIMATION_SETS(line); break;
		}
	}

	f.close();
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

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	GetObjectFromGrid();

	if (stopWatchTimer->IsTimeUp() == true)
		stopWatchTimer->Stop();

	if (crossTimer->IsTimeUp() == true)
		crossTimer->Stop();

	Cross();

	for (size_t i = 0; i < AllObjects.size(); i++)
	{
		AllObjects[i]->Update(dt, &AllObjects, stopWatchTimer->IsTimeUp() == false);
	}
	simon->Update(dt, &AllObjects);
	
	float simonx, simony;
	simon->GetPosition(simonx, simony);
	bool isSimonStand = true;

	if (simon->GetState() == SIMON_SITDOWN || simon->GetState() == SIMON_SIT_ATK)
		isSimonStand = false;

	simon->GetWhip()->SetWhipPosition(simon->nx,simonx, simony, isSimonStand);
	if (((simon->animation_set->at(SIMON_ATK)->GetCurrentFrame() == 2) && simon->GetState() == SIMON_ATK) || ((simon->animation_set->at(SIMON_SIT_ATK)->GetCurrentFrame() == 2) && simon->GetState() == SIMON_SIT_ATK) || ((simon->animation_set->at(SIMON_STAIRUP_ATK)->GetCurrentFrame() == 2) && simon->GetState() == SIMON_STAIRUP_ATK) || ((simon->animation_set->at(SIMON_STAIRDOWN_ATK)->GetCurrentFrame() == 2) && simon->GetState() == SIMON_STAIRDOWN_ATK))
	{		
		if (simon->isAtkWithWhip == true)
		{
			simon->GetWhip()->Update(dt, &AllObjects);
		}
	}
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
		if (dynamic_cast<Knight*>(obj) && obj->GetState() == KNIGHT_STATE_DIE && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			simon->AddScore(200);
			int IdItems = RandomItems();
			listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY()));
		}
		if (dynamic_cast<Darkenbat*>(obj) && obj->GetState() == DARKBAT_STATE_DIE && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			simon->AddScore(100);
			int IdItems = RandomItems();
			listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY()));
		}
		if (dynamic_cast<Monkey*>(obj) && obj->GetState() == MONKEY_STATE_DIE && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			simon->AddScore(300);
			int IdItems = RandomItems();
			listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY()-10));
		}
		if (dynamic_cast<Ghost*>(obj) && obj->GetState() == GHOST_STATE_DIE && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			simon->AddScore(300);
			int IdItems = RandomItems();
			listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY() - 10));
		}
		if (dynamic_cast<Raven*>(obj) && obj->GetState() == RAVEN_STATE_DIE && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			simon->AddScore(200);
			int IdItems = RandomItems();
			listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY()));
		}
		if (dynamic_cast<Zombie*>(obj) && obj->GetState() == ZOMBIE_STATE_DIE && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			simon->AddScore(200);
			int IdItems = RandomItems();
			listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY()));
		}
		if (dynamic_cast<Skeleton*>(obj) && obj->GetState() == SKELETON_STATE_DIE && obj->isDone == false && obj->isEnable == false)
		{
			obj->isEnable = true;
			simon->AddScore(400);
			int IdItems = RandomItems();
			listItems.push_back(DropItems(IdItems, obj->GetPositionX(), obj->GetPositionY()));
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
		SwitchMap(simon->IdNextMap,FileInfMap,FileInfClearMap);//
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
	
	//dành cho những mobs có thể di chuyển ra khỏi map làm grid báo lỗi p.s chỉ dành cho đi như game
	for (UINT i = 0; i < AllObjects.size(); i++)
	{
		if (IsInCam(AllObjects[i]) == true)
			continue;

		if (dynamic_cast<Darkenbat*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
		{
			auto db = dynamic_cast<Darkenbat*>(AllObjects[i]);
			db->SetHP(0);
			db->SetState(DARKBAT_STATE_DIE);
		}

		if (dynamic_cast<Ghost*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0)
		{
			auto ghost = dynamic_cast<Ghost*>(AllObjects[i]);
			ghost->SetHP(0);
			ghost->SetState(GHOST_STATE_DIE);
		}

		if (dynamic_cast<Raven*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0 && AllObjects[i]->GetState()!=RAVEN_STATE_INACTIVE)
		{
			auto raven = dynamic_cast<Raven*>(AllObjects[i]);
			raven->SetHP(0);
			raven->SetState(RAVEN_STATE_DIE);
		}

		if (dynamic_cast<Zombie*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0 && AllObjects[i]->GetState()!= ZOMBIE_STATE_INACTIVE)
		{
			auto zombie = dynamic_cast<Zombie*>(AllObjects[i]);
			zombie->SetHP(0);
			zombie->SetState(ZOMBIE_STATE_DIE);
		}

		if (dynamic_cast<Skeleton*>(AllObjects[i]) && AllObjects[i]->GetHP() > 0 && AllObjects[i]->GetState() != SKELETON_STATE_INACTIVE)
		{
			auto zombie = dynamic_cast<Skeleton*>(AllObjects[i]);
			zombie->SetHP(0);
			zombie->SetState(SKELETON_STATE_DIE);
		}
	}

	if (IsInCam(simon) == false)
	{
		simon->SetState(SIMON_DEAD);
	}

	UpdateGrid();

	if (Simonisdead == true && simonDeadTimer->IsTimeUp() == true)
	{
		simonDeadTimer->Stop();
		if (simon->GetSimonLife() > 0)
		{
			int life = simon->GetSimonLife();
			life -= 1;
			Simonisdead = false;
			simon->SetSimonLife(life);
			simon->SetHP(16);
			simon->GetWhip()->SetState(NORMAL_WHIP);
			simon->SetMana(15);
			SwitchMap(simon->IdCurrMap, FileInfMap, FileInfClearMap);
			hud->SetisTimeover(false);
		}
		else
		{
			Simonisdead = false;
			simon->SetSimonLife(3);
			simon->SetHP(16);
			simon->GetWhip()->SetState(NORMAL_WHIP);
			simon->SetMana(15);
			simon->Setsubweapon(-1);
			simon->SetScore(0);
			simon->SetSimonDoubleTri(-1);
			SwitchMap(1, FileInfMap, FileInfClearMap);
			hud->SetisTimeover(false);
			hud->SetTime(0);
		}
	}

	if (hud->GetisTimeover() == true)
	{
		simon->SetState(SIMON_DEAD);
		//return;
	}

	if (simon->GetState() == SIMON_DEAD)
	{
		if (Simonisdead == false)
		{
			Simonisdead = true;
			simonDeadTimer->Start();
		}
		return;
	}

	
}

void CPlayScene::Render()
{	
	tilemaps->Get((idMap*10000))->Draw();//
	int nx = 0;

	for (int i = 0; i < listStairsUp.size(); i++)
		listStairsUp[i]->Render();
	for (int i = 0; i < listStairsDown.size(); i++)
		listStairsDown[i]->Render();
	for (int i = 0; i < listItems.size(); i++)
		listItems[i]->Render();
	for (int i = 0; i < AllObjects.size(); i++)
		AllObjects[i]->Render();
	

	simon->Render();
	if (simon->IsAtk() == true)///////////////
	{
		if (simon->isAtkWithWhip == true)
		{
			int tempState = simon->GetState();
			if (simon->invisibilityTimer->IsTimeUp() == false)
			{
				switch (tempState)
				{
				case SIMON_ATK:	tempState = SIMON_INV_ATK; break;
				case SIMON_SIT_ATK:	tempState = SIMON_INV_SIT_ATK; break;
				case SIMON_STAIRDOWN_ATK: tempState = SIMON_INV_STAIRDOWN_ATK; break;
				case SIMON_STAIRUP_ATK:	tempState = SIMON_INV_STAIRUP_ATK; break;
				default:
					break;
				}
			}
			simon->GetWhip()->Render(simon->nx, simon->animation_set->at(tempState)->GetCurrentFrame());
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
	CPlayScene* playscene = ((CPlayScene*)scene);
	vector<SubWeapon*> weaponlist = simon->GetListSubWeapon();
	SubWeapon * subweapon;

	float simonx, simony;
	simon->GetPosition(simonx, simony);
	bool isSimonStand = true;

	if (simon->GetState() == SIMON_SITDOWN || simon->GetState() == SIMON_SIT_ATK)
		isSimonStand = false;

	if (simon->Getsubweapon() == WEAPONS_STOP_WATCH)
	{
		if (simon->GetMana() < 5)
			return;
		if (playscene->stopWatchTimer->IsTimeUp() == false) // đang sử dụng stop watch
			return;

		weaponlist[0]->SetEnable(false);
	}

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

		if (simon->Getsubweapon() == WEAPONS_STOP_WATCH)
		{
			int tam = simon->GetMana() - 5;
			simon->SetMana(tam);
			subweapon->SetState(simon->Subweapon);
			playscene->stopWatchTimer->Start();
		}
		else
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
		simon->animation_set->at(SIMON_INV_STAIRUP)->Reset();
		simon->animation_set->at(SIMON_INV_STAIRDOWN)->Reset();
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
	if (simon->GetState() == SIMON_DEFLECT && simon->animation_set->at(SIMON_DEFLECT)->IsOver(TIME_DELAY * 2) == false)
		return;
	if (simon->isAutoWalk == true)
		return;
	if (simon->GetState() == SIMON_DEAD)
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
		simon->GetWhip()->SetDame1Turn(false);
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
		simon->IdSwithMap = 1;
		simon->IdNextMap = 2;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap(),playscene->GetFileClearMap());
		break;
	case DIK_W:
		simon->IdSwithMap = 2;
		simon->IdNextMap = 3;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap(), playscene->GetFileClearMap());
		break;
	case DIK_E:
		simon->IdSwithMap = 3;
		simon->IdNextMap = 4;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap(), playscene->GetFileClearMap());
		break;
	case DIK_R:
		simon->IdSwithMap = 4;
		simon->IdNextMap = 5;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap(), playscene->GetFileClearMap());
		break;
	case DIK_T:
		simon->IdSwithMap = 5;
		simon->IdNextMap = 6;
		playscene->SwitchMap(simon->IdNextMap, playscene->GetFileInFMap(), playscene->GetFileClearMap());
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
	case DIK_6:
		simon->isGotCross = true;
		break;
	case DIK_7:
		simon->invisibilityTimer->Start();
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
	if (simon->GetState() == SIMON_DEFLECT && simon->animation_set->at(SIMON_DEFLECT)->IsOver(TIME_DELAY*2) == false)
		return;
	if (simon->isAutoWalk == true)
		return;
	if (simon->GetState() == SIMON_DEAD)
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
		simon->GetWhip()->SetDame1Turn(false);
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

