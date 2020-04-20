#pragma once
#ifndef num_h
#define num_h

#include "Sprites.h"
#include "Textures.h"
#include "Define.h"
#include<string>
#include<iostream>
#include<sstream>

class Number
{
private:
	LPSPRITE number;
public:
	Number();
	~Number();
	void Draw(float x, float y,const string & s);
};

#endif