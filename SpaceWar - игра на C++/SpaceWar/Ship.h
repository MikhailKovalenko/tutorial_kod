#pragma once
#include <conio.h>
#include "Screen.h"
#include "Cartridge.h"

class Ship
{
private:
	char mark[3]{ '_','W','_' };//������ ��������� ������� _W_
	COORD pozitionship[3];
	void drawStart()
	{
		for (int i = 0; i < 3; i++)
		{
			pozitionship[i].X = 12 + i;//������� ������ ������� �� �
			pozitionship[i].Y = 17;//������� ������ ������� �� y
			position(pozitionship[i].Y, pozitionship[i].X, mark[i]);//��������� ������ ������� �� ���� 
		}
		//��������� ������� �������
	}
	void move()//����� �������� �������
	{
		while (this != NULL)//���� ������ ����������
		{
			
			switch (_getch())
			{
			case 'w':
			{
				if (pozitionship[0].Y > 0)//����������� ���� � ���������
				{
					redraw();
					draw(0, -1);//�������� �����
					break;
				}
			}
			case 's':
			{
				redraw();
				draw(0, 1);//�������� ����
				break;
			}
			case 'a':
			{
				if (pozitionship[0].X > 0)//����������� ���� � ���������
				{
					redraw();
					draw(-1, 0);//�������� �����
					break;
				}
			}
			case 'd':
			{
				redraw();
				draw(1, 0);//�������� ������
				break;
			}
			}	
		}
	}
	void draw(int addx,int addy)//���������� �������
	{

		for (int i = 0; i < 3; i++)
		{
			pozitionship[i].X += addx;//������� ������ ������� �� �
			pozitionship[i].Y += addy;//������� ������ ������� �� y
			position(pozitionship[i].Y, pozitionship[i].X, mark[i]);//��������� ������ ������� �� ���� 
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
		while (this != NULL)//���� ������ ����������
		{
			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)//��������� ������ �� ������ ������ ����
			{
				Cartridge cartridge;
				cartridge.CartridgeDraw(this->pozitionship[1]);
				cartridge.move();
			}
		}
	}
	~Ship() {}
	void position(int y, int x, char ch = '#')// ���������������� �������� �����
	{
		COORD point;
		point.X = x;
		point.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);//��������� ����� ������ � ������� � ����� ����������
		std::cout.put(ch);
	}
	friend class Game;
};

