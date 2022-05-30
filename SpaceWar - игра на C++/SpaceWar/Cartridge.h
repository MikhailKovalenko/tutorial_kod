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
		while (startpozition.Y - startpozitionNew.Y < 3 && startpozitionNew.Y > 0)//9 ���� ����
		{
			if (startpozitionNew.X >= 0 && startpozitionNew.X < 25 && startpozitionNew.Y < 15 && startpozitionNew.Y >= 0)//����� �����
			{
				WinCheck[startpozitionNew.Y][startpozitionNew.X] = ' ';
			}
			position(startpozitionNew.Y, startpozitionNew.X, ' ');//������ �������
			startpozitionNew.Y -= 1;
			if (startpozitionNew.X>=0 && startpozitionNew.X < 25 && startpozitionNew.Y < 15 && startpozitionNew.Y >= 0)
			{
				WinCheck[startpozitionNew.Y][startpozitionNew.X] = ' ';
			}
			position(startpozitionNew.Y, startpozitionNew.X, '.');//����� �������
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
	void position(int y, int x, char ch = '#')// ���������������� �������� �����
	{
		COORD point;
		point.X = x;
		point.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);//��������� ����� ������ � ������� � ����� ����������
		std::cout.put(ch);
	}
	friend class Ship;
};

