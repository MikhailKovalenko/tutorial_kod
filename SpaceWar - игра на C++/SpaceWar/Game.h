#pragma once
#include "Screen.h"
#include "Ship.h"
#include <thread>
#include <iostream>

using namespace std;

class Game
{
private:
	Screen screen;
	Ship ship;
	clock_t start, end;
	void rool()//�������� �� �������
	{
		int k;
		do
		{
			k = 1;
			for (int i = 0; i < screen.hight; i++)
			{
				for (int j = 0; j < screen.wight; j++)
				{
					if (WinCheck[i][j] != ' ')
					{
						k = 0;
						break;
					}
				}
			}

		} while (k == 0);
		if (k == 1)
		{
			this->~Game();
		}
	}
	void roolEnd()//�������� ����� ���� ����� ������ �����
	{
		if (_getch()=='Y')
		{
			end = clock();
			system("cls");
			NewGame();
		}
		if (_getch() == 'N')
		{
			system("cls");
			exit(1);
		}
	}
	void NewGame()
	{
		start = clock() - end;
		screen.drawField();//���������� ���� ������
		ship.drawStart();//���������� ��������� ������� �������
		thread thread3{ &Game::rool, this };//������ ��� ��������� ������ �������
		thread thread1{ &Ship::move, &ship };//������ ��� ��������� ������ �������
		thread thread2{ &Ship::fire, &ship };//������ ��� ��������� ������ �������
		thread3.join();
		thread1.detach();
		thread2.detach();
	}
public:
	Game()
	{
		NewGame();
	}
	~Game()
	{
		system("cls");
		screen.position(10, 30, ' ');
		cout << "������, ������ ������ �����!";
		screen.position(11, 30, ' ');
		if (start / CLOCKS_PER_SEC < 25) cout << "�� ������, �������� ������� ������ ����������� ������"<<'\n';
		else if (start / CLOCKS_PER_SEC < 30) cout << "�� �������, �������� ���������� ������ ����������� ������" << '\n';
		else if (start / CLOCKS_PER_SEC < 35) cout << "����� �� ��� �� ������, �������� ��������� ������ ����������� ������" << '\n';
		else cout << "�� ��� ��� ���� �������, ������� ������ ������" << '\n';
		screen.position(12, 30, ' ');
		cout << "����� ����, ������� : Y";
		screen.position(13, 30, ' ');
		cout << "��������� ����, ������� : N";
		thread thread1{ &Game::roolEnd, this };
		thread1.join();
	}
};

