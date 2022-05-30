#pragma once
#include <Windows.h>
#include <iostream>

static char WinCheck[15][25];//массив отвечающий за условие победы

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
		std::cout << "ƒвижение игрока в консоли осуществл€етс€ считыванием клавиш с клавиатуры";
		position(6, 30, ' ');
		std::cout << "w(вверх), a(влево), s(вниз), d(вправо).";
		position(7, 30, ' ');
		std::cout << "—трельба игрока осуществл€етс€ нажатием левой кнопки мыши.";
		for (int i = 0; i < hight; i++)//изначальна€ инициализаци€ значени€ €чеек пол€
		{
			for (int j = 0; j < wight; j++)
			{
				WinCheck[i][j] = ' ';
			}
		}
		for (int i = 0; i < hight; i++)//инициализаци€ позиций мусора на поле
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
	void position(int y, int x,char ch ='#')// позиционирование объектов рамки
	{
		COORD point;
		point.X = x;
		point.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);//считывает какой доступ к консоли и кака€ координата
		std::cout.put(ch);
	}
	friend class Game;
};

