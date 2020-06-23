#include "HUD.h"

string HUD::FillNumber(string s, UINT MaxNumber)
{
	while (s.size() < MaxNumber)
		s = "0" + s;
	return s;
}

HUD::HUD(Simon * simon)
{
	this->simon = simon;
	this->boss = boss;
	Time = 0;
	for (int i = 0; i < SIMON_MAXHP; i++)
	{
		SimonHP.push_back(CSprites::GetInstance()->Get(SHPSPRITE));
		LoseHP.push_back(CSprites::GetInstance()->Get(BHPSPRITE));
		BossHP.push_back(CSprites::GetInstance()->Get(LHPSPRITE));
	}
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(DAGGERSPRITE));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(AXESPRITE));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(BOOMERANGSPRITE));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(HOLYWATERSPRITE));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(WATCHSPRITE));

	DoubleTri.push_back(CSprites::GetInstance()->Get(DOUBLESPRITE));
	DoubleTri.push_back(CSprites::GetInstance()->Get(TRISPRITE));
}

HUD::~HUD()
{
}

void HUD::Update(DWORD dt)
{
	simonHP = simon->GetHP();
	if (boss == NULL)
	{
		bossHP = SIMON_MAXHP;
	}
	else
		bossHP = boss->GetHP();
	Score = simon->GetScore();
	Mana = simon->GetMana();
	Life = simon->GetSimonLife();
	subWeapon = simon->Getsubweapon();
	doubletri = simon->GetSimonDoubleTri();
	Time += dt;
	RemainingTime = DEFAULT_TIME_PLAY - Time / CLOCKS_PER_SEC;

	if (RemainingTime <= 0)
	{
		RemainingTime = 0;
		isTimeover = true;
	}
}

void HUD::Render(int map, int CamX, int CamY)
{
	board->Render(-1,CamX, CamY);

	number.Draw(CamX + SCORE_X, CamY + SCORE_Y, FillNumber(std::to_string(Score), MAX_NUM_SCORE));
	number.Draw(CamX + TIME_X, CamY + TIME_Y, FillNumber(std::to_string(RemainingTime), MAX_NUM_TIME));
	number.Draw(CamX + MAP_X, CamY + MAP_Y, FillNumber(std::to_string(map), MAX_NUM_MAP));
	number.Draw(CamX + MANA_X, CamY + MANA_Y, FillNumber(std::to_string(Mana), MAX_NUM_MANA));
	number.Draw(CamX + LIFE_X, CamY + LIFE_Y, FillNumber(std::to_string(Life), MAX_NUM_LIFE));
	if (subWeapon != -1)
	{
		if (subWeapon == 0)
		{
			SubWeaponItems[subWeapon]->Draw(-1, CamX + WEAPON_X, CamY + WEAPON_Y1);
		}
		else
			SubWeaponItems[subWeapon]->Draw(-1,CamX + WEAPON_X,CamY + WEAPON_Y2);
	}
	if (doubletri != -1)
	{
		DoubleTri[doubletri]->Draw(-1, CamX + DOUBLE_X, CamY + DOUBLE_Y);
	}

	for (int i = 0; i < simonHP; i++)
		SimonHP[i]->Draw(-1,SIMON_HP_X + i * 9 + CamX ,SIMON_HP_Y);

	for (int i = simonHP; i < SIMON_MAXHP; i++)
		LoseHP[i]->Draw(-1, SIMON_HP_X + i * 9 + CamX, SIMON_HP_Y);
	// Boss HP
	for (int i = 0; i < bossHP; i++)
		BossHP[i]->Draw(-1, BOSS_HP_X + i * 9 + CamX, BOSS_HP_Y);

	for (int i = bossHP; i < SIMON_MAXHP; i++)
		LoseHP[i]->Draw(-1, BOSS_HP_X + i * 9 + CamX, BOSS_HP_Y);
}

void HUD::SetBoss(Boss* boss)
{
	this->boss = boss;
}
