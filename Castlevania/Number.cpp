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
			number = CSprites::GetInstance()->Get(100000);
		}
		else if (s[i] == '1')
		{
			number = CSprites::GetInstance()->Get(100001);
		}
		else if (s[i] == '2')
		{
			number = CSprites::GetInstance()->Get(100002);
		}
		else if (s[i] == '3')
		{
			number = CSprites::GetInstance()->Get(100003);
		}
		else if (s[i] == '4')
		{
			number = CSprites::GetInstance()->Get(100004);
		}
		else if (s[i] == '5')
		{
			number = CSprites::GetInstance()->Get(100005);
		}
		else if (s[i] == '6')
		{
			number = CSprites::GetInstance()->Get(100006);
		}
		else if (s[i] == '7')
		{
			number = CSprites::GetInstance()->Get(100007);
		}
		else if (s[i] == '8')
		{
			number = CSprites::GetInstance()->Get(100008);
		}
		else if (s[i] == '9')
		{
			number = CSprites::GetInstance()->Get(100009);
		}
		number->Draw(-1,x + (i * 15), y);
	}
}
