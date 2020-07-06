#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

void ex1() {

	cout << "������� ������ ��� ���������� ����� (�������� �����������  ���������)" << endl;

	char** arr; /*��������� �� ���������
				����� ������� �������� � ������� �����*/
	int arrsize; //������ ������� ����� �������� � ����������

	cout << "������ �������: ";
	cin >> arrsize; //�������� �������� ������� ������������� 

	 

	arr = (char**)malloc(arrsize * sizeof(char*));
	/*malloc � ������ ��������� ���������� ��������� void-��������� �� ���������� ������� ������
	������� ���������� � ��������� ������������ ����*/

	if (arr == nullptr) 
	{
		//� ��������� ������ �� ���������� NULL
		cout << "������ ��������� ������" <<  endl;
		exit(-1);
	}

	cout << "������� �������� �������" <<  endl;
 

	for (int i = 0; i < arrsize; i++)
	{
		int symb_count;
		
		cout << "�������� � ������ " << i + 1 << ": ";
		cin >> symb_count;
		//�������� ���-�� �������� ��� ������� �������� ������

		arr[i] = (char*)malloc((symb_count + 1) * sizeof(char));
		
		//�������� ������ ��� ������ + 1 ������ ��� ����-�����������


		if (!arr[i])
		{
			cout << "������ ��������� ������" << endl;
			exit(-1);
		} //�������� �� ���������� ��������� ������

		cout << "������: ";
		scanf("%s", arr[i]);
		//������ ������ � ����������
	}

	cout << "�������� ������: " <<  endl;

	for (int i = 0; i < arrsize; i++)
	{ //������� ���������� ������ � ����������� ���������� ��� ��� ������
		cout << arr[i] <<  endl;
		free(arr[i]);
	}

	free(arr); //����������� ������ ��� ������ �����
	cin.get();

}






struct Book 
{
	char* title;
};

void ex2() 
{
	cout << "������� ������ ��� ���������� ���������" << endl;
	Book* arr;
	/*��������� �� ������ ������� ������� ��������*/

	int arr_size;
	cout << "������� ���������� ��������: ";
	cin >> arr_size;
	//�������� ���-�� �������� � ������� ��������� � ����������

	 

	arr = (Book*)malloc(arr_size * sizeof(Book));
	/*�������� ������ �� ���� � ���������� ����� ������� ��������
	�������� void-��������� � Book*/

	if (!arr)
	{
	/*���� malloc ������ ������� ���������*/
		cout << "������ ��������� ������" << endl;
		exit(-1);
	}

	cout << "������� �������� �������" << endl;
 

	for (int i = 0; i < arr_size; i++)
	{
		int symb_count;
		cout << "�������� � ������ " << i + 1 << ": ";
		/*�������� ���-�� �������� �������� � ������ ��� ���� title */
		cin >> symb_count;
		arr[i].title = (char*)malloc((symb_count + 1) * sizeof(char));
		/*�������� ������ ��� ������ ���-�� �������� + 1 ��� ���� �����������*/


		if (!arr[i].title)
		{
			cout << "������ ��������� ������" << endl;
			exit(-1);
		} //�������� �� ���������� ��������� ������

		cout << "������: ";

		scanf("%s", arr[i].title);
		//�������� ������ � ����������

	}

	cout << "�������� ������: " <<  endl;

	for (int i = 0; i < arr_size; i++)
	{
		/*������� ���������� ������ �� ������� �������� � ����������� ������*/
		cout << arr[i].title << endl;
		free(arr[i].title);
	}

	free(arr);
	/*����������� ������ ������� ��������*/
}


struct Bookcase
{
	Book* Books;
};

void ex3()
{
	cout << "������� ������ ��� ���������� ������� ��������" << endl;

	Bookcase* arr;
	/*��������� �� ������ ������� ������� �������� ���������� ��������� �� ������ ��������*/


	int arr_size;
	cout << "���������� �������� � ������� ��������: ";
	cin >> arr_size;
	arr = (Bookcase*)malloc(arr_size * sizeof(Bookcase));
	//�������� ������ 

	
	int* arr_in_arr_sizes = (int*)malloc(arr_size * sizeof(int));
	//��������������� ������ �������� ���-�� �������� ������ ������



	if (!arr || !arr_in_arr_sizes)
	{
		cout << "������ ��������� ������" << endl;
		exit(-1);
	} //�������� �� ���������� ��������� ������

	
	for (int i = 0; i < arr_size; i++)
	{
		int arr_in_arr_size;
		cout << "���������� �������� ������ ��������� " << i + 1 << ": ";
		cin >> arr_in_arr_size;
		//�������� ����� � ���������� � ���������� ��� �� ��������������� ������
		arr_in_arr_sizes[i] = arr_in_arr_size;
		arr[i].Books = (Book*)malloc(arr_in_arr_size * sizeof(Book));
		//�������� ������ ��� ������ ������ ��������

		if (!arr[i].Books)
		{
			cout << "������ ��������� ������" << endl;
			exit(-1);
		} //�������� �� ���������� ��������� ������


		for (int j = 0; j < arr_in_arr_size; j++)
		{
			int symb_count;
			cout << "�������� � ������ " << j + 1 << ": ";
			cin >> symb_count;
			arr[i].Books[j].title = (char*)malloc((symb_count + 1) * sizeof(char));
			//�������� ������� + ����
			cout << "C�����: ";
			scanf("%s", arr[i].Books[j].title);

		}
	}

	cout << "�������� ������: " <<  endl;
	for (int i = 0; i < arr_size; i++)
	{
		cout << "��������� " << i + 1 <<  endl;
		//���������� ���-�� �������� � ������� �� ���������������� �������
		for (int j = 0; j < arr_in_arr_sizes[i]; j++)
		{
			cout << "   C�������� ������ �������� " << j + 1 << ": ";
			cout << arr[i].Books[j].title <<  endl;
			free(arr[i].Books[j].title);
			//������� ������
		}
		free(arr[i].Books);
		//������� ������� ������ �������
	}
	free(arr);
	//������ �������

}





//������� ������ ��� ���������� ������� ��������

 

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << "�������: \n";
	//ex1();
	//ex2();
	ex3();
}
