#include <math.h>

#include "object.h"

// Функция для инициализации точки
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

// Конструктор копирования для реализации передачи объектов в функции 
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

// Функция для инициализации объекта класса
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

// Функция отображает объект в контексте устройства
void object::objectShow(HDC dc, point offset)
{
	SelectObject(dc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(dc, color);
	SelectObject(dc, GetStockObject(DC_PEN));
	(color == RGB(176, 0, 0)) ? SetDCPenColor(dc, RGB(100, 0, 0)) : SetDCPenColor(dc, RGB(0, 100, 0));
	BOOL(_stdcall *shape)(HDC, int, int, int, int);
	shape = (type == ENEMY) ? Ellipse : Rectangle;
	shape(dc, (int)(pos.x - offset.x), (int)(pos.y - offset.y), (int)(pos.x + size.x - offset.x), (int)(pos.y + size.y - offset.y));
}

// Функция, производящая изменение координат объекта
BOOL object::objectMove(object *player)
{
	// Изменение направления движения врага и проверка на столкновение с игроком
	if (type == ENEMY)
	{
		if (rand() % 40 == 1)
		{
			float enemySpeed = 1.5;
			objectDestination(player->pos.x, player->pos.y, enemySpeed);
		}
		if (CrossingObject(*player, *this))
			return TRUE;
	}

	// Изменение направления движения игрока в зависимости от нажатых клавиш
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

	// Проверка движения пули, если расстояние привышает 300 пикселей, то пуля удаляется
	if (type == BULLET)
	{
		range -= vecSpeed;
		if (range < 0)
			isDel = TRUE;
	}

	pos.x += speed.x;
	pos.y += speed.y;

	return FALSE;
}

// Функция задания скорости объекта в зависимости от конечной точки
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

// Дружественная функция для проверки пересечения двух объектов
BOOL CrossingObject(object& obj1, object& obj2)
{

	if ((obj1.pos.x + obj1.size.x) > obj2.pos.x && (obj2.pos.x + obj2.size.x) > obj1.pos.x &&
		(obj1.pos.y + obj1.size.y) > obj2.pos.y && (obj2.pos.y + obj2.size.y) > obj1.pos.y)
	{
		obj1.isDel = TRUE;
		obj2.isDel = TRUE;
		return TRUE;
	}
	return FALSE;
}	