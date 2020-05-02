#include <algorithm>
#include "debug.h"
#include "Game.h"
#include "Simon.h"
#include "Whip.h"
#include "SubWeapon.h"
#include "Gate.h"
#include "Ground.h"

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
	if (isOnStair == false && isAutoWalk == false)//
	{
		vy += SIMON_GRAVITY * dt;
	}

	if (isAutoWalk == true)//
		SimonAutoWalk();

	if (coObjects == NULL)
	{
		if (isAutoWalk == false)
		{
			x += dx;
			y += dy;
		}

		return;
	}
	/*if (isWalking == false && isJumping == false)
	{
		vx = 0;
	}*/
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (state != MARIO_STATE_DIE)
	CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0 && isAutoWalk == false)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		if (isAutoWalk == false)//
		{
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
		}
				
		/*if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					if (e->ny == -1)
					{
						vy = 0;
						if (isJumping == true)
						{
							isJumping = false;
						}
					}
					else
						y += dy;
				}

				// Khi đang lên/xuống cầu thang, va chạm theo trục x sẽ không được xét tới
				if (state == SIMON_STAIRUP || state == SIMON_STAIRDOWN)
				{
					if (nx != 0) x -= nx * 0.4f;
				}
			}

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
	/*int ani;
	
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
	animation_set->at(ani)->Render(nx, x, y);*/
	animation_set->at(state)->Render(nx,x, y);
	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_WALKING:
		isOnStair = false;
		isWalking = true;
		if (nx > 0)
			vx = SIMON_WALKING_SPEED;
		else
			vx = -SIMON_WALKING_SPEED;			
		break;
	case SIMON_JUMP:
		isOnStair = false;
		if (isJumping == false)
		{
			isJumping = true;
			vy = -SIMON_JUMP_SPEED;
		}
		break;
	case SIMON_IDLE:
		isOnStair = false;
		isWalking = false;
		isSitting = false;
		vx = 0;
		break;
	case SIMON_SITDOWN:
		isOnStair = false;
		vx = 0;		
		isSitting = true;
		isWalking = false;
		break;
	case SIMON_ATK:
		isOnStair = false;
		isAtk = true;
		isWalking = false;
		if (isJumping == false)
		{
			vx = 0;
		}
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_SIT_ATK:
		isOnStair = false;
		isAtk = true;
		vx = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_HENSHIN:
		isOnStair = false;
		vx = 0;
		isGotChainItem = true;
		isJumping = false;
		isAtk = false;
		isWalking = false;
		isSitting = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STAIRDOWN:
		if (nx > 0) vx = 0.08;//SIMON_STAIR_SPEED_X;
		else vx = -0.08;//SIMON_STAIR_SPEED_X;
		vy = 0.08;//SIMON_STAIR_SPEED_Y;
		isOnStair = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STAIRUP:
		if (nx > 0) vx = 0.08;//SIMON_STAIR_SPEED_X;
		else vx = -0.08;//SIMON_STAIR_SPEED_X;
		vy = -0.08;//SIMON_STAIR_SPEED_Y;
		isOnStair = true;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STAIRDOWN_ATK:
	case SIMON_STAIRUP_ATK:
		isAtk = true;
		vx = vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	}
}

bool Simon::IsAtk()
{
	return state == SIMON_SIT_ATK || state == SIMON_ATK || state == SIMON_STAIRDOWN_ATK || state == SIMON_STAIRUP_ATK;
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
					SimonScore += 1000;
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

bool Simon::SimonColliWithStair(vector<LPGAMEOBJECT>* liststair)
{
	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);
	
	simon_t += 50;	// thu nhỏ vùng xét va chạm, chỉ xét va chạm với chân của Simon
	simon_b += 10;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống

	CanMoveUp = false;
	CanMoveDown = false;

	for (UINT i = 0; i < liststair->size(); i++)
	{
		float stair_l, stair_t, stair_r, stair_b;
		liststair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);

		if (CGameObject::AABB(simon_l, simon_t, simon_r, simon_b, stair_l, stair_t, stair_r, stair_b) == true)
		{
			if (liststair->at(i)->GetState() == 2|| liststair->at(i)->GetState() == 3|| liststair->at(i)->GetState() == 4) stairnx = -1;//trái trên phải dưới
			else stairnx = 1;//trái dưới phải trên

			StairIsCollided = liststair->at(i);

			if (simon_b < stair_b) CanMoveDown = true;// có bật thang ở dưới simon
			if (y >= stair_t - 32) CanMoveUp = true;// có bật thang ở trên simon

			// kiểm tra xem simon có thể di chuyển lên/xuống hay ko
			// (dựa vào toạ độ của 2 bậc liền kề hơn/kém nhau 32)

			float upstair_x = -99999999999, upstair_y = -99999999999; // toạ độ của bậc thang liền kề

			for (UINT j = 0; j < liststair->size(); j++)
			{
				if (i == j)//trùng bật thang đg đứng
					continue;

				liststair->at(j)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == 32 && dy == -32) {// vì bậc nằm trên nên dy = -...
					CanMoveUp = true;
				}	
				if (dx == 32 && dy == 32) { // vì bậc nằm duoi nên dy = +...
					CanMoveDown = true;
				}
			}

			return true; // collision between Simon and stairs
		}

	}
	return false;
}

void Simon::AutoWalk(float new_x, int new_state, int new_nx)//
{
	isAutoWalk = true;
	newposition = new_x;
	stateAfterAutoWalk = new_state;
	nxAfterAutoWalk = new_nx;
}

void Simon::SimonAutoWalk()
{
	//if (abs(dx) <= abs(AutoWalkdx))//đi đến đúng khoảng cách
	//{
	//	x += dx;
	//	y += dy;
	//	AutoWalkdx -= dx;
	//}
	//else
	//{
	//	x += AutoWalkdx;
	//	state = stateAfterAutoWalk;
	//	nx = nxAfterAutoWalk;

	//	SetState(state);
	//	if (state == SIMON_STAIRDOWN) y += 1.0f; // để đảm bảo simon sẽ va chạm với bậc thang 

	//	isAutoWalk = false;
	//	AutoWalkdx = 0;
	//	stateAfterAutoWalk = -1;
	//	nxAfterAutoWalk = 0;
	//}
	if (x == newposition)
	{
		state = stateAfterAutoWalk;
		nx = nxAfterAutoWalk;
		SetState(state);
		if (state == SIMON_STAIRDOWN) y += 1.0f; // để đảm bảo simon sẽ va chạm với bậc thang 
		isAutoWalk = false;
		newposition = 0;
		stateAfterAutoWalk = -1;
		nxAfterAutoWalk = 0;
	}
	else
	{
		x += dx;
		y += dy;
		if ((nx == 1 && x >= newposition) || (nx == -1 && x <= newposition))
		{
			x = newposition;
			state = stateAfterAutoWalk;
			nx = nxAfterAutoWalk;
			SetState(state);
			if (state == SIMON_STAIRDOWN) y += 1.0f; // để đảm bảo simon sẽ va chạm với bậc thang 
			isAutoWalk = false;
			newposition = 0;
			stateAfterAutoWalk = -1;
			nxAfterAutoWalk = 0;
		}
	}

}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x+15;
	top = y ;
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT ;
}