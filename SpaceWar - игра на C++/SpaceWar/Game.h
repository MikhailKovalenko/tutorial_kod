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
	void rool()//проверка на выигрыш
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
	void roolEnd()//проверка конца игры можно только лично
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
		screen.drawField();//прорисовка поля мусора
		ship.drawStart();//прорисовка начальной позиции корабля
		thread thread3{ &Game::rool, this };//потоки для отдельной работы методов
		thread thread1{ &Ship::move, &ship };//потоки для отдельной работы методов
		thread thread2{ &Ship::fire, &ship };//потоки для отдельной работы методов
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
		cout << "Победа, Победа важней обеда!";
		screen.position(11, 30, ' ');
		if (start / CLOCKS_PER_SEC < 25) cout << "Вы Умница, получите золотую медаль уничтожения мусора"<<'\n';
		else if (start / CLOCKS_PER_SEC < 30) cout << "Вы Молодец, получите серебряную медаль уничтожения мусора" << '\n';
		else if (start / CLOCKS_PER_SEC < 35) cout << "Плохо но все же ничего, получите бронзовую медаль уничтожения мусора" << '\n';
		else cout << "Из вас так себе уборщик, найдите другую работу" << '\n';
		screen.position(12, 30, ' ');
		cout << "Новая игра, нажмите : Y";
		screen.position(13, 30, ' ');
		cout << "Завершить игру, нажмите : N";
		thread thread1{ &Game::roolEnd, this };
		thread1.join();
	}
};

