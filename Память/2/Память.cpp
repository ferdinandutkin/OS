#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

void ex1() {

	cout << "Создать массив для размещения строк (написать аналогичную  программу)" << endl;

	char** arr; /*указатель на указатель
				адрес первого элемента в массиве строк*/
	int arrsize; //размер массива строк вводимый с клавиатуры

	cout << "Размер массива: ";
	cin >> arrsize; //получаем значение введное пользователем 

	 

	arr = (char**)malloc(arrsize * sizeof(char*));
	/*malloc в случае успешного выполнение выполняет void-указатель на выделенную область памяти
	который приводится к указателю необходимого типа*/

	if (arr == nullptr) 
	{
		//в противном случае он возвращает NULL
		cout << "Ошибка выделения памяти" <<  endl;
		exit(-1);
	}

	cout << "Введите элементы массива" <<  endl;
 

	for (int i = 0; i < arrsize; i++)
	{
		int symb_count;
		
		cout << "Символов в строке " << i + 1 << ": ";
		cin >> symb_count;
		//получаем кол-во символов под которое выделяем память

		arr[i] = (char*)malloc((symb_count + 1) * sizeof(char));
		
		//выделяем память под строку + 1 символ для нуля-терминатора


		if (!arr[i])
		{
			cout << "Ошибка выделения памяти" << endl;
			exit(-1);
		} //проверка на успешность выделения памяти

		cout << "Строка: ";
		scanf("%s", arr[i]);
		//вводим строку с клавиатуры
	}

	cout << "Итоговый массив: " <<  endl;

	for (int i = 0; i < arrsize; i++)
	{ //выводим полученную строку и освобождаем выделенную под нее память
		cout << arr[i] <<  endl;
		free(arr[i]);
	}

	free(arr); //освобождаем память под массив строк
	cin.get();

}






struct Book 
{
	char* title;
};

void ex2() 
{
	cout << "Создать массив для размещения структуры" << endl;
	Book* arr;
	/*указатель на первый элемент массива структур*/

	int arr_size;
	cout << "Введите количество структур: ";
	cin >> arr_size;
	//получаем кол-во структур в массиве введенное с клавиатуры

	 

	arr = (Book*)malloc(arr_size * sizeof(Book));
	/*выделяем память из кучи и возвращаем адрес первого элемента
	приводим void-указатель к Book*/

	if (!arr)
	{
	/*если malloc вернул нулевой указатель*/
		cout << "Ошибка выделения памяти" << endl;
		exit(-1);
	}

	cout << "Введите элементы массива" << endl;
 

	for (int i = 0; i < arr_size; i++)
	{
		int symb_count;
		cout << "Символов в строке " << i + 1 << ": ";
		/*получаем кол-во значащих символов в памяти для поля title */
		cin >> symb_count;
		arr[i].title = (char*)malloc((symb_count + 1) * sizeof(char));
		/*выделяем память под нужное кол-во символов + 1 для нуля терминатора*/


		if (!arr[i].title)
		{
			cout << "Ошибка выделения памяти" << endl;
			exit(-1);
		} //проверка на успешность выделения памяти

		cout << "Строка: ";

		scanf("%s", arr[i].title);
		//получаем строку с клавиатуры

	}

	cout << "Итоговый массив: " <<  endl;

	for (int i = 0; i < arr_size; i++)
	{
		/*выводим полученные строки из массива структур и освобождаем память*/
		cout << arr[i].title << endl;
		free(arr[i].title);
	}

	free(arr);
	/*освобождаем память массива структур*/
}


struct Bookcase
{
	Book* Books;
};

void ex3()
{
	cout << "Создать массив для размещения массива структур" << endl;

	Bookcase* arr;
	/*указатель на первый элемент массива структур содержащих указатель на массив структур*/


	int arr_size;
	cout << "Количество структур в массиве структур: ";
	cin >> arr_size;
	arr = (Bookcase*)malloc(arr_size * sizeof(Bookcase));
	//выделяем память 

	
	int* arr_in_arr_sizes = (int*)malloc(arr_size * sizeof(int));
	//вспомогательный массив хранящий кол-во структур внутри каждой



	if (!arr || !arr_in_arr_sizes)
	{
		cout << "Ошибка выделения памяти" << endl;
		exit(-1);
	} //проверка на успешность выделения памяти

	
	for (int i = 0; i < arr_size; i++)
	{
		int arr_in_arr_size;
		cout << "Количество структур внутри структуры " << i + 1 << ": ";
		cin >> arr_in_arr_size;
		//получаем число с клавиатуры и записываем его во вспомогательный массив
		arr_in_arr_sizes[i] = arr_in_arr_size;
		arr[i].Books = (Book*)malloc(arr_in_arr_size * sizeof(Book));
		//выделяем память под массив внутри стуктуры

		if (!arr[i].Books)
		{
			cout << "Ошибка выделения памяти" << endl;
			exit(-1);
		} //проверка на успешность выделения памяти


		for (int j = 0; j < arr_in_arr_size; j++)
		{
			int symb_count;
			cout << "Символов в строке " << j + 1 << ": ";
			cin >> symb_count;
			arr[i].Books[j].title = (char*)malloc((symb_count + 1) * sizeof(char));
			//значащие символы + нуль
			cout << "Cтрока: ";
			scanf("%s", arr[i].Books[j].title);

		}
	}

	cout << "Итоговый массив: " <<  endl;
	for (int i = 0; i < arr_size; i++)
	{
		cout << "Структура " << i + 1 <<  endl;
		//используем кол-во структур в массиве из вспомогательного массива
		for (int j = 0; j < arr_in_arr_sizes[i]; j++)
		{
			cout << "   Cтруктура внутри стуктуры " << j + 1 << ": ";
			cout << arr[i].Books[j].title <<  endl;
			free(arr[i].Books[j].title);
			//очистка строки
		}
		free(arr[i].Books);
		//очистка массива внктри массива
	}
	free(arr);
	//очистк массива

}





//Создать массив для размещения массива структур

 

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << "Задания: \n";
	//ex1();
	//ex2();
	ex3();
}
