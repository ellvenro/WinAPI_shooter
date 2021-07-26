#include <math.h>

#include "object.h"

void pointInit(point& pnt, float x, float y)
{
	pnt.x = x;
	pnt.y = y;
}

object::object()
{
	pointInit(pos, 0, 0);
	pointInit(size, 0, 0);
	pointInit(speed, 0, 0);
	type = PLAYER;
	color = RGB(0, 0, 0);
	range = 0;
	vecSpeed = 0;
	isDel = FALSE;
}

object::object(const object& obj)
{
	pointInit(pos, obj.pos.x, obj.pos.y);
	pointInit(size, obj.size.x, obj.size.y);
	pointInit(speed, obj.speed.x, obj.speed.y);
	type = obj.type;
	color = obj.color;
	range = obj.range;
	vecSpeed = obj.vecSpeed;
	isDel = obj.isDel;
}

void object::objectInit(float xPos, float yPos, float width, float height, TYPE oType)
{
	pointInit(pos, xPos, yPos);
	pointInit(size, width, height);
	pointInit(speed, 0, 0);
	type = oType;
	color = (type == ENEMY) ? RGB(176, 0, 0) : RGB(0, 176, 0);
	if (type == BULLET)
		range = 300;
	vecSpeed = 0;
	isDel = FALSE;
}

void object::objectShow(HDC dc)
{
	SelectObject(dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(dc, color);
	SelectObject(dc, GetStockObject(DC_PEN));
	SetDCPenColor(dc, RGB(0, 0, 0));
	BOOL(_stdcall *shape)(HDC, int, int, int, int);
	shape = (type == ENEMY) ? Ellipse : Rectangle;
	shape(dc, (int)pos.x, (int)pos.y, (int)(pos.x + size.x), (int)(pos.y + size.y));
}

void object::objectMove(object *player)
{
	if (type == ENEMY)
	{
		if (rand() % 40 == 1)
		{
			float enemySpeed = 1.5;
			objectDestination(player->pos.x, player->pos.y, enemySpeed);
		}
	}

	if (type == PLAYER)
	{
		int playerSpeed = 2;
		pointInit(speed, 0, 0);
		if (GetKeyState('W') < 0)
			speed.y = -playerSpeed;
		if (GetKeyState('A') < 0)
			speed.x = -playerSpeed;
		if (GetKeyState('S') < 0)
			speed.y = playerSpeed;
		if (GetKeyState('D') < 0)
			speed.x = playerSpeed;
		if (speed.x != 0 && speed.y != 0)
		{
			speed.x *= 0.7;
			speed.y *= 0.7;
		}
	}

	if (type == BULLET)
	{
		range -= vecSpeed;
		if (range < 0)
			isDel = TRUE;
	}

	pos.x += speed.x;
	pos.y += speed.y;
}

void object::objectDestination(float xPos, float yPos, float vecSpeed)
{
	point pnt;
	pointInit(pnt, xPos - pos.x, yPos - pos.y);
	float dxy = sqrt(pnt.x * pnt.x + pnt.y * pnt.y);
	speed.x = pnt.x / dxy * vecSpeed;
	speed.y = pnt.y / dxy * vecSpeed;
	this->vecSpeed = vecSpeed;
}

void object::GetPos(float& xPos, float& yPos)
{
	xPos = pos.x;
	yPos = pos.y;
}

TYPE object::GetType()
{
	return type;
}

BOOL object::GetIsDel()
{
	return isDel;
}

object& object::operator=(object& obj)
{
	if (this == &obj)
		return *this;

	pointInit(pos, obj.pos.x, obj.pos.y);
	pointInit(size, obj.size.x, obj.size.y);
	pointInit(speed, obj.speed.x, obj.speed.y);
	type = obj.type;
	color = obj.color;
	range = obj.range;
	vecSpeed = obj.vecSpeed;
	isDel = obj.isDel;
	return *this;
}

void CrossingObject(object& obj1, object& obj2)
{
	float xCenter = obj2.pos.x + obj2.size.x / 2;
	float yCenter = obj2.pos.y + obj2.size.y / 2;
	if (xCenter > obj1.pos.x && xCenter < obj1.pos.x + obj1.size.x && yCenter > obj1.pos.y && yCenter < obj1.pos.y + obj1.size.y)
	{
		obj1.isDel = TRUE;
		obj2.isDel = TRUE;
	}
}






//void object::control()
//{
//	int playerSpeed = 2;
//	pointInit(speed, 0, 0);
//	if (GetKeyState('W') < 0)
//		speed.y = -playerSpeed;
//	if (GetKeyState('A') < 0)
//		speed.x = -playerSpeed;
//	if (GetKeyState('S') < 0)
//		speed.y = playerSpeed;
//	if (GetKeyState('D') < 0)
//		speed.x = playerSpeed;
//	if (speed.x != 0 && speed.y != 0)
//	{
//		speed.x *= 0.7;
//		speed.y *= 0.7;
//	}
//}