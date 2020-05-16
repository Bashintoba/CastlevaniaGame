#ifndef sm_h
#define sm_h

#include "GameObject.h"
#include "Whip.h"
#include "SubWeapon.h"
#include "Define.h"
#include<math.h>
#include "Timer.h"

class Simon : public CGameObject
{
private:
	Whip * whip ;
	SubWeapon * subweapon ;
	vector<SubWeapon*> SubWeaponList;
public:
	Simon();
	~Simon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL, bool stopMovement = false);
	virtual void Render();
	void SetState(int state);	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	Timer* untouchableTimer = new Timer(1000);
	Timer* invisibilityTimer = new Timer(5000);
	bool isJumping = false;
	bool isAtk = false;
	bool isWalking = false;
	bool isSitting = false;
	bool IsAtk();
	bool IsWait = false;
	bool isGotChainItem = false;
	bool isGotCross = false;
	bool isAtkWithWhip = false;
	bool isAtkWithSW = false;
	bool isChangeScene = false;
	bool isOnStair = false;//
	bool CanMoveDown = false;//
	bool CanMoveUp = false;//
	bool isAutoWalk = false;
	bool CantMoveDown = true;
	bool isOnMF = false;
	LPGAMEOBJECT StairIsCollided = nullptr;//
	LPGAMEOBJECT StairIsCollidedTemp = nullptr;//
	int stairnx = 0;//
	void StandOnStair() { vx = vy = 0; }//
	int Subweapon = -1;
	int SimonMana = 15;
	int SimonScore = 0;
	int SimonLife = 3;
	int SimonDoubleTri = -1;
	int IdSwithMap = 0;
	int IdCurrMap = 0;
	int IdNextMap = 0;
	Whip * GetWhip() { return whip; }
	SubWeapon * GetSubWeapon() { return subweapon; }
	vector<SubWeapon*> GetListSubWeapon() { return SubWeaponList; }
	void SimonColliWithItems(vector<LPGAMEOBJECT> *listitem);
	bool SimonColliWithStair(vector<LPGAMEOBJECT> *liststair);
	int GetMana() { return SimonMana; }
	void SetMana(int Mana) { SimonMana = Mana; }
	int GetScore() { return SimonScore; }
	void SetScore(int s) { SimonScore = s; }
	int Getsubweapon() { return Subweapon; }
	void Setsubweapon(int sw) { Subweapon = sw; }
	int GetSimonLife() { return SimonLife; }
	void SetSimonLife(int life) { SimonLife = life; }
	int GetSimonDoubleTri() { return SimonDoubleTri; }
	void SetSimonDoubleTri(int dbt) { SimonDoubleTri = dbt; }
	void AddScore(int score) { SimonScore += score; }	
	void AutoWalk(float new_x, int new_state, int new_nx);
	void SimonAutoWalk();
	float newposition = 0;		// Khoảng cách 
	int stateAfterAutoWalk = -1;	// Trạng thái sau khi auto-walk
	int nxAfterAutoWalk = 0;		// Hướng Simon sau khi auto-walk
	
};

#endif