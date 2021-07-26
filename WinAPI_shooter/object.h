#pragma once
#include <Windows.h>

// Перечисление для определения типа объекта
enum TYPE { PLAYER, ENEMY, BULLET };

// Тип для описания всех объектов
struct point
{
	float x;
	float y;
};

void pointInit(point& pnt, float x, float y);

// Класс, содержащий все объекты при работе программы
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
	BOOL objectMove(object* player = NULL);	
	void objectDestination(float xPos, float yPos, float vecSpeed);	
	void GetPos(float& xPos, float& yPos);
	TYPE GetType();
	BOOL GetIsDel();

	object& operator=(object& obj);
	friend BOOL CrossingObject(object& obj1, object& obj2);
};