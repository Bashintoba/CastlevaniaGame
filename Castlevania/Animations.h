#ifndef anima_h
#define anima_h

#include"Sprites.h"
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

class CAnimation
{
public:
	DWORD aniStartTime;
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
	void SetAniStartTime(DWORD t) { aniStartTime = t; }
	bool IsOver(DWORD dt) { return GetTickCount() - aniStartTime >= dt; }
	void Reset() { currentFrame = -1; }
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(int nx, float x, float y, int alpha = 255);
	void RenderByID(int currentID, int nx, float x, float y, int alpha = 255);
	void SetFrame(int x) { currentFrame = x; }
	int GetCurrentFrame() { return currentFrame; }
	int GetFramesSize() { return frames.size(); }
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations * GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets * __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;


public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static CAnimationSets * GetInstance();
};

#endif