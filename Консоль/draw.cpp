#include <iostream>   
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>

#define WHITE 0xF6
#define RED 0xC3
#define YELLOW 0xE5
#define GREEN 0xA1
#define BLUE 0x1A


using namespace std;

stringstream scale(ifstream &file, int scalenumber) {
	stringstream scaledss;
	string bufline;
	while (getline(file, bufline)) {
		string scaledline;
		for (char el : bufline)
			for (int i = 0; i < scalenumber * 2; i++) //по горизонтали
				scaledline += el;
		for (int i = 0; i < scalenumber; i++) //по вертикали
			scaledss << scaledline << endl;
	}
	return scaledss;
}



int main(int argc, char **argv) {

	string name(argv[1]);
	short relX, relY, scalenumber;
	if (argc == 2) {
		relX = 0;
		relY = 0;
		scalenumber = 1;
	}
	else {
		relX = stoi(argv[2]);
		relY = stoi(argv[3]);
		scalenumber = stoi(argv[4]);
	}

	HANDLE COHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(COHandle, &csbi);
	WORD initattribs = csbi.wAttributes;
	COORD coords;

	short initY = csbi.dwCursorPosition.Y;


	ifstream file(name + ".txt");
	stringstream todraw = scale(file, scalenumber);
	string line;
	for (coords.Y = initY + relY; getline(todraw, line); coords.Y++) {
		for (coords.X = relX; coords.X - relX < line.length(); coords.X++) {
			if (line[coords.X - relX] != ' ') {
				SetConsoleCursorPosition(COHandle, coords);
				switch (line[coords.X - relX]) {
				case '1': SetConsoleTextAttribute(COHandle, WHITE); break;
				case '2': SetConsoleTextAttribute(COHandle, RED); break;
				case '3': SetConsoleTextAttribute(COHandle, YELLOW); break;
				case '4': SetConsoleTextAttribute(COHandle, GREEN); break;
				case '5': SetConsoleTextAttribute(COHandle, BLUE); break;
				}
				putchar(32);
			}
		}
	}
	SetConsoleTextAttribute(COHandle, initattribs);
}


