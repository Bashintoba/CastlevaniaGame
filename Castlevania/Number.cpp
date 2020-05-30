#include "Number.h"

Number::Number()
{
}

Number::~Number()
{
}

void Number::Draw(float x, float y,const string & s)
{
	for (UINT i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ')
			continue;
		if (s[i] == '0')
		{
			number = CSprites::GetInstance()->Get(NUM0);
		}
		else if (s[i] == '1')
		{
			number = CSprites::GetInstance()->Get(NUM1);
		}
		else if (s[i] == '2')
		{
			number = CSprites::GetInstance()->Get(NUM2);
		}
		else if (s[i] == '3')
		{
			number = CSprites::GetInstance()->Get(NUM3);
		}
		else if (s[i] == '4')
		{
			number = CSprites::GetInstance()->Get(NUM4);
		}
		else if (s[i] == '5')
		{
			number = CSprites::GetInstance()->Get(NUM5);
		}
		else if (s[i] == '6')
		{
			number = CSprites::GetInstance()->Get(NUM6);
		}
		else if (s[i] == '7')
		{
			number = CSprites::GetInstance()->Get(NUM7);
		}
		else if (s[i] == '8')
		{
			number = CSprites::GetInstance()->Get(NUM8);
		}
		else if (s[i] == '9')
		{
			number = CSprites::GetInstance()->Get(NUM9);
		}
		number->Draw(-1,x + (i * 15), y);
	}
}
