#pragma once
#include "Screen.h"

class Cartridge 
{
private:
	char sign = '.';
	COORD startpozition, startpozitionNew;
	void move()
	{
		startpozitionNew = startpozition;
		while (startpozition.Y - startpozitionNew.Y < 3 && startpozitionNew.Y > 0)//9 край пол€
		{
			if (startpozitionNew.X >= 0 && startpozitionNew.X < 25 && startpozitionNew.Y < 15 && startpozitionNew.Y >= 0)//кос€к здесь
			{
				WinCheck[startpozitionNew.Y][startpozitionNew.X] = ' ';
			}
			position(startpozitionNew.Y, startpozitionNew.X, ' ');//стара€ позици€
			startpozitionNew.Y -= 1;
			if (startpozitionNew.X>=0 && startpozitionNew.X < 25 && startpozitionNew.Y < 15 && startpozitionNew.Y >= 0)
			{
				WinCheck[startpozitionNew.Y][startpozitionNew.X] = ' ';
			}
			position(startpozitionNew.Y, startpozitionNew.X, '.');//нова€ позици€
			Sleep(50);
		}
		position(startpozitionNew.Y, startpozitionNew.X, ' ');
		this->~Cartridge();
	}
	void CartridgeDraw(COORD pozitionship)
	{
		startpozition.X = pozitionship.X;
		startpozition.Y = pozitionship.Y - 1;
		position(startpozition.Y, startpozition.X, sign);
	}
	void position(int y, int x, char ch = '#')// позиционирование объектов рамки
	{
		COORD point;
		point.X = x;
		point.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);//считывает какой доступ к консоли и кака€ координата
		std::cout.put(ch);
	}
	friend class Ship;
};

