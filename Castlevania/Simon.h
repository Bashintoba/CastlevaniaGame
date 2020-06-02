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
	//thời gian
	Timer* untouchableTimer = new Timer(TIME_UNTOUCH);
	Timer* invisibilityTimer = new Timer(TIME_INVIS);
	//cờ
	bool isJumping = false;
	bool isAtk = false;
	bool isSitting = false;
	bool IsWait = false;
	bool isGotChainItem = false;
	bool isGotCross = false;
	bool isAtkWithWhip = false;
	bool isAtkWithSW = false;
	bool isChangeScene = false;
	bool isAutoWalk = false;
	bool CantMoveDown = true;
	bool isOnMF = false;
	//cầu thang
	LPGAMEOBJECT StairIsCollided = nullptr;//
	LPGAMEOBJECT StairIsCollidedTemp = nullptr;//
	bool SimonColliWithStair(vector<LPGAMEOBJECT>* liststair);
	bool SimonColliWithStair12(vector<LPGAMEOBJECT>* liststair);
	void StandOnStair() { vx = vy = 0; }//
	int stairnx = 0;//
	bool isOnStair = false;//
	bool CanMoveDown = false;//
	bool CanMoveUp = false;//
	bool CanMoveDOut = false;
	bool CanMoveUOut = false;
	bool Tam1 = false;
	//thuộc tính cơ bản
	int Subweapon;
	int SimonMana;
	int SimonScore;
	int SimonLife;
	int SimonDoubleTri;
	int IdSwithMap;
	int IdCurrMap;
	int IdNextMa;
	//vũ khí
	Whip * GetWhip() { return whip; }
	SubWeapon * GetSubWeapon() { return subweapon; }
	vector<SubWeapon*> GetListSubWeapon() { return SubWeaponList; }
	bool IsAtk();
	//va chạm với item
	void SimonColliWithItems(vector<LPGAMEOBJECT>* listitem);
	//getset
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
	//auto walk
	void AutoWalk(float new_x, int new_state, int new_nx);
	void SimonAutoWalk();
	float newposition = 0;		// Khoảng cách 
	int stateAfterAutoWalk = -1;	// Trạng thái sau khi auto-walk
	int nxAfterAutoWalk = 0;		// Hướng Simon sau khi auto-walk
	
};

#endif