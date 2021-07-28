﻿#include <Windows.h>
#include <iostream>

#include "object.h"

using namespace std;

RECT rct;
object player;
object* masObject;
int masCnt = 0;
point offset;
BOOL newGame = FALSE;

HWND hwnd;
HDC dc;

void WinInit();
void WinMove();
void WinShow(HDC dc); 
void NewObject(float xPos, float yPos, float width, float height, TYPE type);
void DelObject();
void AddBullet(float x, float y);
void AddEnemy();
void ChangeOffset();

LRESULT CALLBACK  WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

//int main()
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSA wlc;
    memset(&wlc, 0, sizeof(WNDCLASSA));
    wlc.lpszClassName = "my Window";
    wlc.lpfnWndProc = WndProc;
    wlc.hCursor = LoadCursorA(NULL, (LPCSTR)IDC_CROSS);
    RegisterClassA(&wlc);

    hwnd = CreateWindowExA(NULL, "my Window", "Window", WS_OVERLAPPEDWINDOW, 10, 10, 640, 480, NULL, NULL, NULL, NULL);
    dc = GetDC(hwnd);
    ShowWindow(hwnd, SW_SHOWNORMAL);

    WinInit();

    MSG msg;
    while (1)
    {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            WinMove();
            WinShow(dc);
            Sleep(5);
        }
    }
    if (masCnt > 0)
        delete[] masObject;

    return 0;
}

// Функция инициализации игры
void WinInit()
{
    newGame = FALSE;
    if (masCnt > 0)
        delete[] masObject;
    masCnt = 0;

    player.objectInit(100, 100, 40, 40, PLAYER);
}

// Функция, реализующая сдвиг всех объектов и удаление ненужны
void WinMove()
{
    if (newGame)
        WinInit();

    player.objectMove();
    ChangeOffset();

    for (int i = 0; i < masCnt; i++)
    {
        newGame = masObject[i].objectMove(&player);
        if (newGame)
            break;
    }

    for (int i = 0; i < masCnt - 1; i++)
    {
        if (masObject[i].GetType() == ENEMY)
            for (int j = i + 1; j < masCnt; j++)
            {
                if (masObject[j].GetType() == BULLET)
                    CrossingObject(masObject[i], masObject[j]);
            }

        if (masObject[i].GetType() == BULLET)
            for (int j = i + 1; j < masCnt; j++)
            {
                if (masObject[j].GetType() == ENEMY)
                    CrossingObject(masObject[i], masObject[j]);
            }
    }

    AddEnemy();
    DelObject();
}

// Функция реализации изображения игры 
void WinShow(HDC dc)
{
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc, rct.right - rct.left, rct.bottom - rct.top);
    SelectObject(memDC, memBM);

    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(230, 230, 230));
    SelectObject(memDC, GetStockObject(DC_PEN));
    SetDCPenColor(memDC, RGB(255, 255, 255));
    int rectSize = 200;
    int dx = (int)offset.x % rectSize;
    int dy = (int)offset.y % rectSize;
    for (int i = -1; i < rct.right / rectSize + 2; i++)
        for (int j = -1; j < rct.bottom / rectSize + 2; j++)
            Rectangle(memDC, -dx + i * rectSize, -dy + j * rectSize, -dx + (i + 1) * rectSize, -dy + (j + 1) * rectSize);

    player.objectShow(memDC, offset);
    for (int i = 0; i < masCnt; i++)
        masObject[i].objectShow(memDC, offset);

    BitBlt(dc, 0, 0, rct.right - rct.left, rct.bottom - rct.top, memDC, 0, 0, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);
}

void NewObject(float xPos, float yPos, float width, float height, TYPE type)
{
    if (masCnt > 0)
    {
        object* buf = new object[masCnt + 1];
        for (int i = 0; i < masCnt; i++)
            buf[i] = masObject[i];
        delete[] masObject;
        masObject = buf;
    }
    else
        masObject = new object[1];
    
    masObject[masCnt].objectInit(xPos, yPos, width, height, type);
    masCnt++;
}

void DelObject()
{
    if (masCnt > 1)
    {
        int cnt = masCnt;
        for (int i = 0; i < masCnt; i++)
        {
            if (masObject[i].GetIsDel())
            {
                masCnt--;
                masObject[i] = masObject[masCnt];
            }
            object* buf = new object[masCnt];
            for (int j = 0; j < masCnt; j++)
                buf[j] = masObject[j];
            delete[] masObject;
            masObject = buf;
        }
    }
    if (masCnt == 1)
    {
        if (masObject[0].GetIsDel())
        {
            delete[] masObject;
            masCnt--;
        }
    }
}

void AddBullet(float x, float y)
{
    float xPos, yPos;
    player.GetPos(xPos, yPos);
    NewObject(xPos + 20, yPos + 20, 10, 10, BULLET);
    masObject[masCnt - 1].objectDestination(x, y, 4);
}

void AddEnemy()
{
    int rad = 300;
    int pos1 = (rand() % 2 == 1) ? rad : -rad;
    int pos2 = (rand() % (rad * 2)) - rad;
    float x, y;
    player.GetPos(x, y);
    int k = rand() % 120;
    if (k == 1)
        NewObject(pos1 + x, pos2 + y, 40, 40, ENEMY);
    if (k == 2)
        NewObject(pos2 + x, pos1 + y, 40, 40, ENEMY);
}

void ChangeOffset()
{
    float x, y;
    player.GetPos(x, y);
    offset.x = x + 20 - rct.right / 2;
    offset.y = y + 20 - rct.bottom / 2;
}

LRESULT CALLBACK  WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == WM_DESTROY)
        PostQuitMessage(0);

    else if (message == WM_KEYDOWN)
        printf("code = %d\n", wparam);

    else if (message == WM_CHAR)
        printf("%c\n", wparam);

    else if (message == WM_SIZE)
        GetClientRect(hwnd, &rct);

    else if (message == WM_MOUSEMOVE)
    {
        int xPos = LOWORD(lparam);
        int yPos = HIWORD(lparam);
        //printf("mouse: %d, %d\n", xPos, yPos);
    }

    else if (message == WM_LBUTTONDOWN)
    {
        int xPos = LOWORD(lparam);
        int yPos = HIWORD(lparam);
        AddBullet(xPos + offset.x, yPos + offset.y);
    }

    else if (message == WM_RBUTTONDOWN)
    {
        for (int i = 0; i < masCnt; i++)
            cout << i << ": " << masObject[i].GetType() << endl;
    }

    else
        return DefWindowProcA(hwnd, message, wparam, lparam);
}