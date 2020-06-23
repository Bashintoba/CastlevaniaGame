#include "Boss.h"

Boss::Boss(LPGAMEOBJECT target)
{
	isDone = false;
	isEnable = false;
	isStart = false;
	isAtk = false;
	isFindtarget = false;
	infinity = false;
	wait = false;
	relax = false;
	this->target = target;
	this->HP = SIMON_MAXHP;
}

Boss::~Boss()
{
}

void Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (state == BOSS_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
		this->isDone = true;

	if (stopMovement == true)
		return;
	if (state != BOSS_STATE_INACTIVE)
	{
		if (x < game->GetCamPosX())
		{
			x = game->GetCamPosX();
		}

		if ((x+BOSS_BBOX_WIDTH+ LIMIT) > (game->GetCamPosX() + SCREEN_WIDTH))
		{
			x = (game->GetCamPosX() + SCREEN_WIDTH -BOSS_BBOX_WIDTH- LIMIT);
		}

		if (y < (game->GetCamPosY() + LIMIT*2))
		{
			y = (game->GetCamPosY() + LIMIT*2);
		}
	}
	if (state != BOSS_STATE_DIE)
	{
		CGameObject::Update(dt);
		
		x += dx;
		y += dy;

		if (isAtk == false)
		{
			if (x < target->GetPositionX()) Nx = 1;
			else Nx = -1;
		}

		if (isAtk == false)
		{
			if (y < target->GetPositionY()) Ny = 1;
			else Ny = -1;
		}

		if (target != NULL)
		{
			if (x<target->GetPositionX() && (abs(y-target->GetPositionY())<=150))
			{
				if (state == BOSS_STATE_INACTIVE)
				{
					Boss_move_to_postion->Start();
					SetState(BOSS_STATE_ATK);
					vx = (float)(BOSS_SPEED)*-1;
					vy = (float)(BOSS_SPEED)*1;
					isStart = true;
				}
			}
		}

		if (Boss_move_to_postion->IsTimeUp() == true && isStart == true)
		{
			Boss_find_target->Start();
			isStart = false;
			isAtk = false;
			infinity = false;
			wait = true;
			vx = 0;
			vy = 0;
		}

		if (Boss_find_target->IsTimeUp() == true && wait == true && isAtk == false)
		{
			isAtk = true;
			Boss_relax->Start();
			infinity = true;
			wait = false;
			float VX = abs(target->GetPositionX() - this->x);
			float VY = abs(target->GetPositionY() - this->y);
			vx = (float)(VX/ BOSS_TIME_ATK) * Nx;
			vy = (float)(VY/ BOSS_TIME_ATK) * Ny;
		}

		if (Boss_relax->IsTimeUp() == true && infinity == true)
		{
			Boss_move_to_postion->Start();
			isAtk = false;
			isStart = true;
			if (relax == false)
			{
				relax = true;
				location = (rand() % 3) * BOSS_SPEED;
				if((game->GetCamPosX()+SCREEN_WIDTH)-this->x< DIST_ACTICE)
				{ 
					Nxtemp = 1;
				}
				else
					Nxtemp = -1;
				relax = true;
			}
			if (relax == true)
			{
				vx = (float)((BOSS_SPEED2 + location)) * Nxtemp;
				vy = (float)(BOSS_SPEED) * -1;
			}
			infinity = false;
		}
	}
}

void Boss::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Boss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case BOSS_STATE_ATK:
		break;
	case BOSS_STATE_DIE:
		this->HP = 0;
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Boss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != BOSS_STATE_DIE)
	{
		left = x + 13;
		top = y;
		right = left + BOSS_BBOX_WIDTH;
		bottom = top + BOSS_BBOX_HEIGHT;
	}
}

