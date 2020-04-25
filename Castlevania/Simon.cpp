#include <algorithm>
#include "debug.h"
#include "Game.h"
#include "Simon.h"
#include "Whip.h"
#include "SubWeapon.h"
#include "Gate.h"

Simon::Simon()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(SIMON_ANIMATION_SET);
	SetAnimationSet(ani_set);
	whip = new Whip();
	subweapon = new SubWeapon();
	/*for (int i = 1; i <= 3; i++)
	{
		subweapon = new SubWeapon();
		subweapon->SetEnable(false);
		SubWeaponList.push_back(subweapon);
	}*/
	nx = 1;
	this->SetState(SIMON_IDLE);
}


Simon::~Simon()
{
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	vy += SIMON_GRAVITY * dt;
	if (isWalking == false && isJumping == false)
	{
		vx = 0;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (state != MARIO_STATE_DIE)
	CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (ny == -1)
		{
			//dy = vy * dt;

			if (isJumping == true)
			{
				isJumping = false;
			}
		}

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Gate *>(e->obj)) // if e->obj is Goomba 
			{
				Gate *gate = dynamic_cast<Gate *>(e->obj);
				isChangeScene = true;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Simon::Render()
{
	int ani;
	
	if (isSitting == true)
	{
		if (isAtk == true)
		{
			ani = SIMON_SIT_ATK;
		}
		else
		{
			ani = SIMON_SITDOWN;
		}
	}
	else if (isAtk == true && isSitting == false)
	{
		ani = SIMON_ATK;
	}
	else 
	{
		if (isGotChainItem == true || IsWait == true)
		{
			ani = SIMON_HENSHIN;
		}
		else if (isWalking == true)
		{			
		if (isJumping == false)
			{
				ani = SIMON_WALKING;
			}
			else if (isSitting == true)
			{
				ani = SIMON_SITDOWN;
			}
			else
			{
				ani = SIMON_JUMP;
			}

		}
		else
		{
			if (isJumping == true)
			{
				ani = SIMON_JUMP;
			}
			else
			{
				ani = SIMON_IDLE;
			}
		}
	}
	animation_set->at(ani)->Render(nx, x, y);
	//animation_set->at(state)->Render(nx,x, y);
	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_WALKING:
		isWalking = true;
		if (nx > 0)
			vx = SIMON_WALKING_SPEED;
		else
			vx = -SIMON_WALKING_SPEED;			
		break;
	case SIMON_JUMP:
		if (isJumping == false)
		{
			isJumping = true;
			vy = -SIMON_JUMP_SPEED;
		}
		break;
	case SIMON_IDLE:
		isWalking = false;
		isSitting = false;
		vx = 0;
		break;
	case SIMON_SITDOWN:
		vx = 0;		
		isSitting = true;
		isWalking = false;
		break;
	case SIMON_ATK:
		isAtk = true;
		isWalking = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_SIT_ATK:
		isAtk = true;
		vx = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_HENSHIN:
		vx = 0;
		isGotChainItem = true;
		isJumping = false;
		isAtk = false;
		isWalking = false;
		isSitting = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

bool Simon::IsAtk()
{
	return state == SIMON_SIT_ATK || state == SIMON_ATK;
}

void Simon::SimonColliWithItems(vector<LPGAMEOBJECT>* listitem)
{
	float simon_l, simon_t, simon_r, simon_b;
	float item_l, item_t, item_r, item_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listitem->size(); i++)
	{
		LPGAMEOBJECT e = listitem->at(i);
		e->GetBoundingBox(item_l, item_t, item_r, item_b);
		if (CGameObject::AABB(simon_l, simon_t, simon_r, simon_b, item_l, item_t, item_r, item_b))
		{
				e->isDone = true;
				int IDItems = e->GetState();
				switch (IDItems)
				{
				case SMALL_HEART:
					SimonMana += 1;
					break;
				case LARGE_HEART:
					SimonMana += 5;
					break;
				case MONEY_BAG_RED:
					SimonScore += 300;
					break;
				case MONEY_BAG_BLUE:
					SimonScore += 500;
					break;
				case MONEY_BAG_WHITE:
					SimonScore += 700;
					break;
				case PORK_CHOP:
					if (SimonHP <= 12)
					{
						SimonHP += 4;
					}
					break;
				case CHAIN:
					SetState(SIMON_HENSHIN); // siêu nhân biến hình			
					break;
				case STOP_WATCH:
					Subweapon = 4;
					break;
				case ITEM_DAGGER:
					Subweapon = 0;
					break;
				case ITEM_AXE:
					Subweapon = 1;
					break;
				case ITEM_BOOMERANG:
					Subweapon = 2;
					break;
				case ITEM_HOLY_WATER:
					Subweapon = 3;
					break;
				case CROSS:
					break;
				case INVISIBILITY_POTION:
					break;
				case DOUBLE_SHOT:
					SimonDoubleTri = 0;
					break;
				case TRIPLE_SHOT:
					SimonDoubleTri = 1;
					break;
				case CROWN:
					break;
				case MAGIC_CRYSTAL:
					break;
				default:
					break;
				}

			//}
		}
	}
}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x +15;
	top = y ;
	right = left + SIMON_BBOX_WIDTH ;
	bottom = top + SIMON_BBOX_HEIGHT;
}