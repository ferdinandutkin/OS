#include <windows.h>
#include <iostream>

const WORD colors[] = {
	0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
	0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
};

int main() {
	HANDLE COHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	short posX = 0, posY = 0;

	for (WORD color : colors){
		SetConsoleCursorPosition(COHandle, { posX+=6, posY+=2 });
		SetConsoleTextAttribute(COHandle, color);
		std::cout << "Sometext";
	}

	getchar();
}