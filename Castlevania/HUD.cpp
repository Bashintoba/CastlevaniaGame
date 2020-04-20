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
	Time = 0;
	for (int i = 0; i < 16; i++)
	{
		SimonHP.push_back(CSprites::GetInstance()->Get(2));
		LoseHP.push_back(CSprites::GetInstance()->Get(3));
		BossHP.push_back(CSprites::GetInstance()->Get(4));
	}
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(507));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(509));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(510));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(511));
	SubWeaponItems.push_back(CSprites::GetInstance()->Get(508));

	DoubleTri.push_back(CSprites::GetInstance()->Get(512));
	DoubleTri.push_back(CSprites::GetInstance()->Get(513));
}

HUD::~HUD()
{
}

void HUD::Update(DWORD dt)
{
	simonHP = simon->GetHP();
	Score = simon->GetScore();
	Mana = simon->GetMana();
	Life = simon->GetSimonLife();
	subWeapon = simon->Getsubweapon();
	doubletri = simon->GetSimonDoubleTri();
	Time += dt;
	RemainingTime = DEFAULT_TIME_PLAY - Time / CLOCKS_PER_SEC;
}

void HUD::Render(int map, int CamX, int CamY)
{
	board->Render(-1,CamX, CamY);

	number.Draw(CamX + 100, CamY + 15, FillNumber(std::to_string(Score), 6));
	number.Draw(CamX + 295, CamY + 15, FillNumber(std::to_string(RemainingTime), 4));
	number.Draw(CamX + 465, CamY + 15, FillNumber(std::to_string(map), 2));
	number.Draw(CamX + 393, CamY + 33, FillNumber(std::to_string(Mana), 2)); 
	number.Draw(CamX + 393, CamY + 51, FillNumber(std::to_string(Life), 2)); 
	if (subWeapon != -1)
	{
		if (subWeapon == 0)
		{
			SubWeaponItems[subWeapon]->Draw(-1, CamX + 310, CamY + 38);
		}
		else
			SubWeaponItems[subWeapon]->Draw(-1,CamX + 310,CamY + 34);
	}
	if (doubletri != -1)
	{
		DoubleTri[doubletri]->Draw(-1, CamX + 430, CamY + 31);
	}

	for (int i = 0; i < simonHP; i++)
		SimonHP[i]->Draw(-1,108 + i * 9 + CamX ,31);

	for (int i = simonHP; i < 16; i++)
		LoseHP[i]->Draw(-1, 108 + i * 9 + CamX, 31);
	// Boss HP
	for (int i = 0; i < bossHP; i++)
		BossHP[i]->Draw(-1, 109 + i * 9 + CamX, 47);

	for (int i = bossHP; i < 16; i++)
		LoseHP[i]->Draw(-1, 109 + i * 9 + CamX, 47);
}
