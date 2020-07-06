#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>

using namespace std;


void addrow() {
	static bool notfirst;
	cout << setfill((char)196);
	if (!notfirst)
		cout << (char)218 << setw(20) << (char)194 << setw(20) << (char)191 << endl;
	else
		cout << (char)195 << setw(20) << (char)197 << setw(20) << (char)180 << endl;
	cout << setfill((char)32) << (char)179 << setw(20) << (char)179 << setw(20) << (char)179 << endl;
	cout << setfill((char)196) << (char)192 << setw(20) << (char)193 << setw(20) << (char)217 << endl;
	notfirst = true;

}
int main() {
	SetConsoleOutputCP(866);
	SetConsoleCP(1251);

	HANDLE COHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coords = { 1, 1 };

	addrow();

	SetConsoleCursorPosition(COHandle, coords);
	cout << "”€Œˆ‹ˆŸ";
	coords.X = 21;
	SetConsoleCursorPosition(COHandle, coords);
	cout << "ƒ“€";


	while (true) {
		coords.X = 0, coords.Y++;
		SetConsoleCursorPosition(COHandle, coords);
		addrow();
		coords.Y++, coords.X++;
		SetConsoleCursorPosition(COHandle, coords);
		string placeholder;
		cin >> placeholder;
		coords.X = 21;
		SetConsoleCursorPosition(COHandle, coords);
		cin >> placeholder;
	}
}