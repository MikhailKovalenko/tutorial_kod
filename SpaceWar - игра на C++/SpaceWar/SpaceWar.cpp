#include <windows.h>
#include <iostream>
#include "Game.h"
#include <thread>

using namespace std;

int main()
{
	setlocale(LC_ALL, "RUSSIAN");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);//установление кодировки и языка в консоли

	DWORD prev_mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &prev_mode);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));//убирает выделение мыши

	Game game = Game();//запуск игры
}
