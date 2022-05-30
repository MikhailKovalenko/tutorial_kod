#pragma once
#include <conio.h>
#include "Screen.h"
#include "Cartridge.h"

class Ship
{
private:
	char mark[3]{ '_','W','_' };//символ отрисовки корабля _W_
	COORD pozitionship[3];
	void drawStart()
	{
		for (int i = 0; i < 3; i++)
		{
			pozitionship[i].X = 12 + i;//позиции частей корабля по х
			pozitionship[i].Y = 17;//позиции частей корабля по y
			position(pozitionship[i].Y, pozitionship[i].X, mark[i]);//отрисовка частей корабля на поле 
		}
		//начальная позиция корабля
	}
	void move()//метод движения корабля
	{
		while (this != NULL)//пока объект существует
		{
			
			switch (_getch())
			{
			case 'w':
			{
				if (pozitionship[0].Y > 0)//исправления бага с движением
				{
					redraw();
					draw(0, -1);//движение вверх
					break;
				}
			}
			case 's':
			{
				redraw();
				draw(0, 1);//движение вниз
				break;
			}
			case 'a':
			{
				if (pozitionship[0].X > 0)//исправления бага с движением
				{
					redraw();
					draw(-1, 0);//движение влево
					break;
				}
			}
			case 'd':
			{
				redraw();
				draw(1, 0);//движение вправо
				break;
			}
			}	
		}
	}
	void draw(int addx,int addy)//прорисовка корабля
	{

		for (int i = 0; i < 3; i++)
		{
			pozitionship[i].X += addx;//позиции частей корабля по х
			pozitionship[i].Y += addy;//позиции частей корабля по y
			position(pozitionship[i].Y, pozitionship[i].X, mark[i]);//отрисовка частей корабля на поле 
		}
	}
	void redraw(const char arr[3] = "   ")
	{
		for (int i = 0; i < 3; i++)
		{
			if (pozitionship[i].X >= 0 && pozitionship[i].X < 25 && pozitionship[i].Y < 15 && pozitionship[i].Y >= 0)
			{
				WinCheck[pozitionship[i].Y][pozitionship[i].X] = ' ';
			}
			position(pozitionship[i].Y, pozitionship[i].X, arr[i]);
		}
	}

	void fire()
	{
		while (this != NULL)//пока объект существует
		{
			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)//считывает нажата ли правая кнопка мыши
			{
				Cartridge cartridge;
				cartridge.CartridgeDraw(this->pozitionship[1]);
				cartridge.move();
			}
		}
	}
	~Ship() {}
	void position(int y, int x, char ch = '#')// позиционирование объектов рамки
	{
		COORD point;
		point.X = x;
		point.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);//считывает какой доступ к консоли и какая координата
		std::cout.put(ch);
	}
	friend class Game;
};

