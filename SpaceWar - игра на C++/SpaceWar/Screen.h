#pragma once
#include <Windows.h>
#include <iostream>

static char WinCheck[15][25];//������ ���������� �� ������� ������

class Screen
{
private:
	CONSOLE_CURSOR_INFO cursorInfo;
	int wight = 25, hight = 15;
	Screen()
	{
		cursorShow(false);
	}
	void drawField()
	{
		position(5,30,' ');
		std::cout << "�������� ������ � ������� �������������� ����������� ������ � ����������";
		position(6, 30, ' ');
		std::cout << "w(�����), a(�����), s(����), d(������).";
		position(7, 30, ' ');
		std::cout << "�������� ������ �������������� �������� ����� ������ ����.";
		for (int i = 0; i < hight; i++)//����������� ������������� �������� ����� ����
		{
			for (int j = 0; j < wight; j++)
			{
				WinCheck[i][j] = ' ';
			}
		}
		for (int i = 0; i < hight; i++)//������������� ������� ������ �� ����
		{
			for (int j = 0; j < wight; j++)
			{
				int x, y;
				x = rand() % 25;
				y = rand() % 15;
				WinCheck[y][x] = '#';
				position(y, x);
			}
		}
	}
	void cursorShow(bool visible)
	{	
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
		cursorInfo.bVisible = visible;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	}
	void position(int y, int x,char ch ='#')// ���������������� �������� �����
	{
		COORD point;
		point.X = x;
		point.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);//��������� ����� ������ � ������� � ����� ����������
		std::cout.put(ch);
	}
	friend class Game;
};

