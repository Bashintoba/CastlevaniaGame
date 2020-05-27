#include "Raven.h"

Raven::Raven(LPGAMEOBJECT target, float distance)
{
	isDone = false;
	isEnable = false;
	isbuffspeed = false;
	findtarget = false;
	findtargetagain = false;
	doyouwanttobuildasnowman = false;
	this->target = target;
	this->dist = distance;
}

Raven::~Raven()
{
}

void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	if (state == RAVEN_STATE_DIE && animation_set->at(state)->IsOver(TIME_DELAY) == true)
		this->isDone = true;

	if (stopMovement == true)
		return;

	if (state != RAVEN_STATE_DIE)
	{
		CGameObject::Update(dt);
		x += dx;
		y += dy;

		if (isbuffspeed == false)
		{
			if (x >= target->GetPositionX())
				nx = -1;
			else
				nx = 1;
		}

		if (isbuffspeed == false)
		{
			if (y >= target->GetPositionY())
				directionY = -1;
			else
				directionY = 1;
		}

		if (target != NULL)
		{
			CGame* game = CGame::GetInstance();
			if (GetDistance(this->x, this->y, target->x, target->y) <= dist && (x > game->GetCamPosX() && x < (game->GetCamPosX() + SCREEN_WIDTH) && (y >= game->GetCamPosY() && y < (game->GetCamPosY() + SCREEN_HEIGHT))) == true)
			{
				if (state == RAVEN_STATE_INACTIVE)
				{
					raven_change_state->Start();
					SetState(RAVEN_STATE_FLYING);
					float VX = abs(target->GetPositionX() - this->x);
					float VY = abs(target->GetPositionY() - this->y);
					vx = (float)(VX * 0.0003) * nx;
					vy = (float)(VY * 0.0003) * directionY;
				}

			}
		}

		if (raven_change_state->IsTimeUp() == true && findtargetagain == false && findtarget == false && state != RAVEN_STATE_INACTIVE)
		{
			findtarget = true;
			raven_find_target->Start();
			vx = 0;
			vy = 0;
		}

		if (raven_find_target->IsTimeUp() == true && findtarget == true && isbuffspeed == false)
		{
			raven_find_target->Stop();
			isbuffspeed = true;
			raven_want_to_play_again->Start();
			doyouwanttobuildasnowman = true;
			float VX = abs(target->GetPositionX() - this->x);
			float VY = abs(target->GetPositionY() - this->y);
			vx = (float)(VX * 0.0009) * nx;
			vy = (float)(VY * 0.0009) * directionY;
		}
		//dí lần 2
	   /*if (raven_want_to_play_again->IsTimeUp() == true && doyouwanttobuildasnowman == true)
	   {
		   isbuffspeed = false;
		   findtarget = false;
		   doyouwanttobuildasnowman = false;
		   findtargetagain = true;
		   raven_find_target->Start();
		   vx = vy = 0;
	   }

	   if (raven_find_target->IsTimeUp() == true && findtargetagain == true && isbuffspeed == false)
	   {
		   isbuffspeed = true;
		   float VX = abs(target->GetPositionX() - this->x);
		   float VY = abs(target->GetPositionY() - this->y);
		   vx = (float)(VX * 0.001) * nx;
		   vy = (float)(VY * 0.001) * directionY;
	   }*/
	}

}

void Raven::Render()
{
	if (this->isDone == false)
	{
		animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Raven::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case RAVEN_STATE_INACTIVE:
		vx = 0;
		vy = 0;
		break;
	case RAVEN_STATE_FLYING:
		break;
	case RAVEN_STATE_DIE:
		this->HP = 0;
		vx = 0;
		vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != RAVEN_STATE_DIE)
	{
		left = x;
		top = y;
		right = left + RAVEN_BBOX_WIDTH;
		bottom = top + RAVEN_BBOX_HEIGHT;
	}
}
