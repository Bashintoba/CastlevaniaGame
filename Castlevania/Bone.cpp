#include "Bone.h"
#include "Game.h"
#include "Simon.h"

Bone::Bone(LPGAMEOBJECT simon)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(BONE_ANIMATION);
	SetAnimationSet(ani_set);
	isDone = false;
	isEnable = true;
	this->simon = simon;
}

void Bone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMovement)
{
	if (stopMovement == true)
	{
		stop = true;
		return;
	}
	else
	{
		stop = false;
	}

	CGame* game = CGame::GetInstance();

	if((x >= game->GetCamPosX() && x < (game->GetCamPosX() + SCREEN_WIDTH) && (y >= game->GetCamPosY() && y < (game->GetCamPosY() + SCREEN_HEIGHT)))== false)
	{
		this->isEnable = true;
		this->isDone = true;
		return;
	}

	CGameObject::Update(dt);
	vy += WEAPONS_AXE_GRAVITY * dt;

	x += dx;
	y += dy;

}

void Bone::Render()
{
	if (isDone == false && CheckOutCam(X) == false)
	{
		if (stop == true)
		{
			int temp = animation_set->at(state)->GetCurrentFrame();
			animation_set->at(state)->SetFrame(temp);
			animation_set->at(state)->RenderByID(temp, nx, x, y);
		}
		else
			animation_set->at(state)->Render(nx, x, y);
		//RenderBoundingBox();
	}
	else
		return;
}

void Bone::SetWeaponPosition(float X, float Y, int Nx)
{
	X += 5;
	Y += 5;
	nx = Nx;
	this->X = X;
	SetPosition(X, Y);
}

void Bone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BONE_BBOX_WIDTH;
	bottom = top + BONE_BBOX_HEIGHT;
}

void Bone::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BONE:
		if (nx > 0) vx = WEAPON_BONE_SPEED_X;
		else vx = -WEAPON_BONE_SPEED_X;
		vy = -WEAPONS_AXE_SPEED_Y;
		break;
	default:
		break;
	}
}

bool Bone::CheckOutCam(float x)
{
	if (vx > 0)
	{
		if (this->x - x >= (SCREEN_WIDTH / 2))
			return true;
	}
	else if (vx < 0)
	{
		if (x - this->x >= (SCREEN_WIDTH / 2))
			return true;
	}
	return false;
}
