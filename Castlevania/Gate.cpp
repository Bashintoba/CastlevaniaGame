#include "Gate.h"

Gate::Gate()
{

}

Gate::~Gate() {}


void Gate::Render()
{
	//RenderBoundingBox();
}

void Gate::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + GATE_BBOX_WIDTH;
	bottom = top + GATE_BBOX_HEIGHT;
}