﻿#include <algorithm>
#include "debug.h"
#include "Game.h"
#include "Simon.h"
#include "Whip.h"
#include "SubWeapon.h"
#include "Gate.h"
#include "Ground.h"
#include "Candle.h"
#include "Stair.h"
#include "BreakBrick.h"
#include "MovingPlatform.h"
#include "Knight.h"
#include "Darkenbat.h"
#include "Monkey.h"
#include "Ghost.h"
#include "Raven.h"
#include "Zombie.h"
#include "Skeleton.h"
#include "Bone.h"
#include "Boss.h"

Simon::Simon()
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(SIMON_ANIMATION_SET);
	SetAnimationSet(ani_set);
	HP = SIMON_MAXHP;
	whip = new Whip();
	for (int i = 0; i <3; i++)
	{
		subweapon = new SubWeapon(this);
		subweapon->SetEnable(false);
		SubWeaponList.push_back(subweapon);
	}
	nx = 1;
	Subweapon = -1;
	SimonMana = SIMON_MANA;
	SimonScore = 0;
	SimonLife = SIMON_LIFE;
	SimonDoubleTri = -1;
	IdSwithMap = 1;
	IdCurrMap = 1;
	IdNextMap = 1;
	this->SetState(SIMON_IDLE);
}


Simon::~Simon()
{
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects, bool stopMovement)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	if (isOnStair == false && isAutoWalk == false)
	{
		vy += SIMON_GRAVITY * dt;
	}

	if (isAutoWalk == true)//
		SimonAutoWalk();

	if (untouchableTimer->IsTimeUp() == true)
		untouchableTimer->Stop();

	if (invisibilityTimer->IsTimeUp() == true)
		invisibilityTimer->Stop();

	if (coObjects == NULL)
	{
		if (isAutoWalk == false)
		{
			x += dx;
			y += dy;
		}
		return;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT> ListsColl;
	coEvents.clear();
	ListsColl.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i) != dynamic_cast<Candle*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Stair*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Ghost*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Monkey*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Knight*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Skeleton*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Zombie*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Bone*>(coObjects->at(i)) && coObjects->at(i) != dynamic_cast<Boss*>(coObjects->at(i)))
		{
			ListsColl.push_back(coObjects->at(i));
		}
	}
	// turn off collision when die 
	//if (state != SIMON_DEAD)

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (state != SIMON_DEAD && state != SIMON_HENSHIN && untouchableTimer->IsTimeUp() == true && invisibilityTimer->IsTimeUp() == true)
		{
			LPGAMEOBJECT obj = coObjects->at(i);

			if (dynamic_cast<Ghost*>(obj))
			{
				Ghost* e = dynamic_cast<Ghost*> (obj);
				float left, top, right, bottom;
				float Sleft, Stop, Sright, Sbottom;
				e->GetBoundingBox(left, top, right, bottom);
				this->GetBoundingBox(Sleft, Stop, Sright, Sbottom);
				if (AABB(Sleft, Stop, Sright, Sbottom, left, top, right, bottom) == true)
				{
					untouchableTimer->Start();
					this->AddHP(DAME1);
					if (isOnStair == false || HP == 0)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// cài trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}
				}
			}

			if (dynamic_cast<Knight*>(obj))
			{
				Knight* e = dynamic_cast<Knight*> (obj);
				float left, top, right, bottom;
				float Sleft, Stop, Sright, Sbottom;
				e->GetBoundingBox(left, top, right, bottom);
				this->GetBoundingBox(Sleft, Stop, Sright, Sbottom);
				if (AABB(Sleft, Stop, Sright, Sbottom, left, top, right, bottom) == true)
				{
					untouchableTimer->Start();
					this->AddHP(DAME2);
					if (isOnStair == false || HP == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}
				}
			}

			if (dynamic_cast<Zombie*>(obj))
			{
				Zombie* e = dynamic_cast<Zombie*> (obj);
				float left, top, right, bottom;
				float Sleft, Stop, Sright, Sbottom;
				e->GetBoundingBox(left, top, right, bottom);
				this->GetBoundingBox(Sleft, Stop, Sright, Sbottom);
				if (AABB(Sleft, Stop, Sright, Sbottom, left, top, right, bottom) == true)
				{
					untouchableTimer->Start();
					this->AddHP(DAME2);
					if (isOnStair == false || HP == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}
				}
			}

			if (dynamic_cast<Monkey*>(obj))
			{
				Monkey* e = dynamic_cast<Monkey*> (obj);
				float left, top, right, bottom;
				float Sleft, Stop, Sright, Sbottom;
				e->GetBoundingBox(left, top, right, bottom);
				this->GetBoundingBox(Sleft, Stop, Sright, Sbottom);
				if (AABB(Sleft, Stop, Sright, Sbottom, left, top, right, bottom) == true )
				{
					untouchableTimer->Start();
					this->AddHP(DAME2);
					if (isOnStair == false || HP == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}
				}
			}

			if (dynamic_cast<Skeleton*>(obj))
			{
				Skeleton* e = dynamic_cast<Skeleton*> (obj);
				float left, top, right, bottom;
				float Sleft, Stop, Sright, Sbottom;
				e->GetBoundingBox(left, top, right, bottom);
				this->GetBoundingBox(Sleft, Stop, Sright, Sbottom);
				if (AABB(Sleft, Stop, Sright, Sbottom, left, top, right, bottom) == true)
				{
					untouchableTimer->Start();
					this->AddHP(DAME2);
					if (isOnStair == false || HP == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}
				}
			}

			if (dynamic_cast<Bone*>(obj))
			{
				Bone* e = dynamic_cast<Bone*> (obj);
				float left, top, right, bottom;
				float Sleft, Stop, Sright, Sbottom;
				e->GetBoundingBox(left, top, right, bottom);
				this->GetBoundingBox(Sleft, Stop, Sright, Sbottom);
				if (AABB(Sleft, Stop, Sright, Sbottom, left, top, right, bottom) == true)
				{
					untouchableTimer->Start();
					this->AddHP(DAME1);
					e->isDone = true;
					e->isEnable = true;
					if (isOnStair == false || HP == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}
				}
			}

			if (dynamic_cast<Boss*>(obj))
			{
				Boss* e = dynamic_cast<Boss*> (obj);
				float left, top, right, bottom;
				float Sleft, Stop, Sright, Sbottom;
				e->GetBoundingBox(left, top, right, bottom);
				this->GetBoundingBox(Sleft, Stop, Sright, Sbottom);
				if (AABB(Sleft, Stop, Sright, Sbottom, left, top, right, bottom) == true)
				{
					untouchableTimer->Start();
					this->AddHP(DAME2);
					if (isOnStair == false || HP == 0)
					{
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}
				}
			}
		}
	}

	CalcPotentialCollisions(&ListsColl, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0 && isAutoWalk == false )
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
			x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.1f;
		}

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj) || (dynamic_cast<BreakBrick*>(e->obj)))
			{
				if (e->ny != 0)
				{
					if (e->ny == -1 && (state != SIMON_DEFLECT || (state == SIMON_DEFLECT && vy > 0)))
					{
						vy = 0;
						isOnMF = false;
						if (isJumping == true)
						{
							isJumping = false;
						}			
					}
					else
						y += dy;	
			
				}

				// Khi đang lên/xuống cầu thang, va chạm theo trục x sẽ không được xét tới
				if (state == SIMON_STAIRUP || state == SIMON_STAIRDOWN || isOnMF == true)
				{
					if (nx != 0) x -= nx * 0.1f;
				}
			}
			else if (dynamic_cast<Gate *>(e->obj)) 
			{
				Gate *gate = dynamic_cast<Gate *>(e->obj);
				IdNextMap = gate->GetIdNextMap();
				isChangeScene = true;
			}
			else if (dynamic_cast<MovingPlatform *>(e->obj))
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
						isOnMF = true;
						vx = e->obj->Getvx();
					}
					else
						y += dy;
				}
			}
			else if (dynamic_cast<Darkenbat*>(e->obj)|| dynamic_cast<Raven*>(e->obj))
			{
				if (state != SIMON_DEAD &&state != SIMON_HENSHIN && untouchableTimer->IsTimeUp() == true && invisibilityTimer->IsTimeUp() == true)
				{
					untouchableTimer->Start();
					if (dynamic_cast<Darkenbat*>(e->obj))
					{
						Darkenbat* dk = dynamic_cast<Darkenbat*>(e->obj);
						dk->isDone = true;
						dk->SetState(DARKBAT_STATE_DIE);
						this->AddHP(DAME2);
					}
					else if (dynamic_cast<Raven*>(e->obj))
					{
						Raven* raven = dynamic_cast<Raven*>(e->obj);
						raven->isDone = true;
						raven->SetState(RAVEN_STATE_DIE);
						this->AddHP(DAME2);
					}

					if (isOnStair == false || HP == 0)  // Simon đứng trên cầu thang sẽ không bị bật ngược lại
					{
						// đặt trạng thái deflect cho simon
						if (e->nx != 0)
						{
							if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
							else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
						}

						SetState(SIMON_DEFLECT);
					}

				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (this->HP <= 0)
	{
		this->HP = 0;
		SetState(SIMON_DEAD);
		return;
	}
}
void Simon::Render()
{	
	int alpha = 255;
	int tempState = state;
	if (untouchableTimer->IsTimeUp() == false)  // Để render Simon nhấp nháy trong trạng thái sau khi bị thương
		alpha = rand() % 255;
	else if (invisibilityTimer->IsTimeUp() == false) // invisible
	{
		switch (tempState)
		{
		case SIMON_IDLE:	tempState = SIMON_INV_IDLE; break;
		case SIMON_WALKING:	tempState = SIMON_INV_WALKING; break;
		case SIMON_JUMP:	tempState = SIMON_INV_JUMP; break;
		case SIMON_SITDOWN:	tempState = SIMON_INV_SITDOWN; break;
		case SIMON_ATK:	tempState = SIMON_INV_ATK; break;
		case SIMON_SIT_ATK:	tempState = SIMON_INV_SIT_ATK; break;
		case SIMON_STAIRDOWN:tempState = SIMON_INV_STAIRDOWN; break;
		case SIMON_STAIRUP:	tempState = SIMON_INV_STAIRUP; break;
		case SIMON_STAIRDOWN_ATK: tempState = SIMON_INV_STAIRDOWN_ATK; break;
		case SIMON_STAIRUP_ATK:	tempState = SIMON_INV_STAIRUP_ATK; break;
		default:
			break;
		}
	}
	animation_set->at(tempState)->Render(nx,x, y,alpha);
	animation_set->at(state)->SetFrame(animation_set->at(tempState)->GetCurrentFrame());
	//RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_WALKING:
		CantMoveDown = true;
		isOnStair = false;
		if (nx > 0)
			vx = SIMON_WALKING_SPEED;
		else
			vx = -SIMON_WALKING_SPEED;			
		break;
	case SIMON_JUMP:
		isOnStair = false;

		if (vx != SIMON_WALKING_SPEED && vx != -SIMON_WALKING_SPEED)
		{
			vx = 0;
		}

		if (isJumping == false)
		{
			isJumping = true;
			vy = -SIMON_JUMP_SPEED;
		}
		break;
	case SIMON_IDLE:
		CanMoveDOut = false;//
		CanMoveUOut = false;//
		CantMoveDown = true;
		isOnStair = false;
		isSitting = false;
		isJumping = false;
		if (isOnMF == false)
		{
			vx = 0;
		}
		break;
	case SIMON_SITDOWN:
		CantMoveDown = true;
		isOnStair = false;
		vx = 0;		
		isSitting = true;
		break;
	case SIMON_ATK:
		isOnStair = false;
		isAtk = true;
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
		isSitting = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STAIRDOWN:
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = SIMON_STAIR_SPEED_Y;
		CanMoveDOut = true;//
		CanMoveUOut = true;//
		isOnStair = true;
		isJumping = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STAIRUP:
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = -SIMON_STAIR_SPEED_Y;
		CanMoveDOut = true;//
		CanMoveUOut = true;//
		isOnStair = true;
		isJumping = false;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STAIRDOWN_ATK:
	case SIMON_STAIRUP_ATK:
		isAtk = true;
		CanMoveDOut = true;//
		CanMoveUOut = true;//
		vx = vy = 0;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_DEFLECT:
		vy = -SIMON_DEFLECT_SPEED_Y;
		if (nx > 0) vx = -SIMON_DEFLECT_SPEED_X;
		else vx = SIMON_DEFLECT_SPEED_X;
		animation_set->at(state)->Reset();
		animation_set->at(state)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_DEAD:
		untouchableTimer->Stop();
		invisibilityTimer->Stop();
		vx = 0;
		vy = 0;
		break;
	}
}

bool Simon::IsAtk()
{
	return state == SIMON_SIT_ATK || state == SIMON_ATK || state == SIMON_STAIRDOWN_ATK || state == SIMON_STAIRUP_ATK ;
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
					SimonMana += MANA1;
					break;
				case LARGE_HEART:
					SimonMana += MANA5;
					break;
				case MONEY_BAG_RED:
					SimonScore += SCORE300;
					break;
				case MONEY_BAG_BLUE:
					SimonScore += SCORE500;
					break;
				case MONEY_BAG_WHITE:
					SimonScore += SCORE700;
					break;
				case PORK_CHOP:
					this->HP += ADDHP;
					if (this->HP >= SIMON_MAXHP)
					{
						this->HP = SIMON_MAXHP;
					}
					break;
				case CHAIN:
					SetState(SIMON_HENSHIN); // siêu nhân biến hình			
					break;
				case STOP_WATCH:
					Subweapon = WEAPONS_STOP_WATCH;
					break;
				case ITEM_DAGGER:
					Subweapon = DAGGER;
					break;
				case ITEM_AXE:
					Subweapon = WEAPONS_AXE;
					break;
				case ITEM_BOOMERANG:
					Subweapon = WEAPONS_BOOMERANG;
					break;
				case ITEM_HOLY_WATER:
					Subweapon = WEAPONS_HOLY_WATER;
					break;
				case CROSS:
					isGotCross = true;
					break;
				case INVISIBILITY_POTION:
					invisibilityTimer->Start();
					break;
				case DOUBLE_SHOT:
					if (SimonDoubleTri == 1)
					{
						SimonDoubleTri = 1;
					}
					else
						SimonDoubleTri = 0;
					break;
				case TRIPLE_SHOT:
					SimonDoubleTri = 1;
					break;
				case CROWN:
					SimonScore += SCORE1000;
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
	simon_b += 5;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống

	CanMoveUp = false;
	CanMoveDown = false;

	for (UINT i = 0; i < liststair->size(); i++)
	{
		LPGAMEOBJECT obj = liststair->at(i);
		
		float stair_l, stair_t, stair_r, stair_b;
		liststair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);
		if (CGameObject::AABB(simon_l, simon_t, simon_r, simon_b, stair_l, stair_t, stair_r, stair_b) == true)
		{
			Stair* e = dynamic_cast<Stair*>(liststair->at(i));

			stairnx = e->nx;

			if (e->type == 0)
			{
				CanMoveUOut = true;
				if (isOnStair == true)
				{
					int SimonY = int(this->y);
					if (stair_t - SimonY >= DIST1 && stair_t - SimonY <= DIST2)
					{
						CanMoveDown = true;
					}
					else
						CanMoveDown = false;
				}
			}
			else if(e->type == 1)
			{
				CanMoveDOut = true;
				if (isOnStair == true)
				{
					int SimonY = int(this->y);
					if (stair_t - SimonY >= DIST1 && stair_t - SimonY <= DIST2)
					{
						CanMoveUp = true;
					}
					else
						CanMoveUp = false;
				}
			}

			StairIsCollided = liststair->at(i);

			return true;
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
	x += dx;
	y += dy;
	if ((nx == 1 && x >= newposition) || (nx == -1 && x <= newposition))
	{
		x = newposition;
		state = stateAfterAutoWalk;
		nx = nxAfterAutoWalk;
		SetState(state);
		if (state == SIMON_STAIRDOWN && CanMoveDown == false) y += 1.0f; // để simon va chạm với bậc thang 
		isAutoWalk = false;
		newposition = 0;
		stateAfterAutoWalk = -1;
		nxAfterAutoWalk = 0;
	}
}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x+15;
	top = y ;
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT ;
}