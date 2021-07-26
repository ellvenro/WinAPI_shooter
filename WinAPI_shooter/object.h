#pragma once
#include <Windows.h>

enum TYPE { PLAYER, ENEMY, BULLET };

struct point
{
	float x;
	float y;
};

void pointInit(point& pnt, float x, float y);

class object
{
	point pos;
	point size;
	COLORREF color;
	point speed;
	TYPE type;
	float range;
	float vecSpeed;
	BOOL isDel;

public:
	object();
	object(const object& obj);

	void objectInit(float xPos, float yPos, float width, float height, TYPE oType);
	void objectShow(HDC dc, point offset);
	void objectMove(object* player = NULL);
	void objectDestination(float xPos, float yPos, float vecSpeed);

	void GetPos(float& xPos, float& yPos);
	TYPE GetType();
	BOOL GetIsDel();

	//void control();

	object& operator=(object& obj);
	friend void CrossingObject(object& obj1, object& obj2);
};