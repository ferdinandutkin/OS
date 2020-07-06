#include <iostream>
#include <string>
#include <map>
#include <algorithm>

template<typename T, size_t max_size>
class Stack {

private:

	size_t top;
	T* container;

public:
	template <typename... Types>
	Stack(Types... values) {
		top = -1;
		container = new T[max_size];
		push(values ...);
	}
	Stack() {
		top = -1;
		container = new T[max_size];
	}

	~Stack() { delete[] container; }

	void push(T value) {
		if (full()) {
			std::cerr << "Stack is full!" << std::endl;
		}
		else container[++top] = value;
	}

	template <typename... Types>
	void push(T value, Types... values) {
		push(value);
		push(values...);
	}

	T peek() {
		if (empty()) {
			std::cerr << "Stack is empty!" << std::endl;
		}
		else return container[top];
	}
	T pop() {
		if (empty()) {
			std::cerr << "Stack is empty!" << std::endl;
		}
		else return container[top--];
	}


	bool empty() { return top == -1; }

	bool full() { return top + 1 == max_size; }
};

enum state { X = -4, YES = -3, NO, ERR, S0, S1, S2, S3, S4, S5, S6, S7, S8 };

std::ostream& operator << (std::ostream& os, const state en) {
	std::map<state, std::string> map{ { X, "X" },{ YES, "YES" },{ NO, "NO" },{ ERR, "ERR" },{ S0, "S0" },{ S1, "S1" },{ S2, "S2" },{ S3, "S3" },{ S4, "S4" },
	{ S5, "S5" },{ S6, "S6" },{ S7, "S7" },{ S8, "S8" } };
	os << map[en];
	return os;
}




constexpr int SIGN = 0;
constexpr int NUMBER = 1;
constexpr int DOT = 2;
constexpr int SPACE = 3;
constexpr int DEFAULT = 4;
constexpr int EXP_SIGN = 5;
constexpr int COMPLEX_I = 6;
constexpr int SPECIAL_SIGN = 7;
constexpr int BRACKET_LEFT = 8;
constexpr int BRACKET_RIGHT = 9;


int main() {
	setlocale(LC_ALL, "Rus");



	state table[5][9][10] =
	{
		{ // Таблица 1
			{ // S0
				S1, // Знак
				S2, // 0 - 9
				S3, // Точка
				ERR, // Пробел
				ERR, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )

			},
			{ // S1
				NO, // Знак
				S2, // 0 - 9
				S3, // Точка
				NO, // Пробел
				NO, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S2
				NO, // Знак
				S2, // 0 - 9
				S4, // Точка
				YES, // Пробел
				NO, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S3
				NO, // Знак
				S5, // 0 - 9
				NO, // Точка
				NO, // Пробел
				NO, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S4
				NO, // Знак
				S5, // 0 - 9
				NO, // Точка
				YES, // Пробел
				NO, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S5
				NO, // Знак
				S5, // 0 - 9
				NO, // Точка
				YES, // Пробел
				NO, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S6
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S7
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S8
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
		},
		{ // Таблица 2
			{ // S0
				S1, // Знак
				S2, // 0 - 9
				S3, // Точка
				ERR, // Пробел
				ERR, // Другое
				S6, // e
				X, // i
				X, // * or /
				X, // (
				X, // )

			},
			{ // S1
				NO, // Знак
				S2, // 0 - 9
				S3, // Точка
				NO, // Пробел
				NO, // Другое
				NO, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S2
				NO, // Знак
				S2, // 0 - 9
				S4, // Точка
				YES, // Пробел
				NO, // Другое
				S6, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S3
				NO, // Знак
				S5, // 0 - 9
				NO, // Точка
				NO, // Пробел
				NO, // Другое
				NO, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S4
				NO, // Знак
				S5, // 0 - 9
				NO, // Точка
				YES, // Пробел
				NO, // Другое
				S6, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S5
				NO, // Знак
				NO, // 0 - 9
				NO, // Точка
				NO, // Пробел
				NO, // Другое
				S6, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S6
				S7, // Знак
				S8, // 0 - 9
				NO, // Точка
				NO, // Пробел
				NO, // Другое
				NO, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S7
				NO, // Знак
				S8, // 0 - 9
				NO, // Точка
				NO, // Пробел
				NO, // Другое
				NO, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S8
				NO, // Знак
				S8, // 0 - 9
				NO, // Точка
				YES, // Пробел
				NO, // Другое
				NO, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
		},
		{ // Таблица 3
			{ // S0
				S1, // Знак
				S2, // 0 - 9 && a - d 
				X, // Точка
				ERR, // Пробел
				ERR, // Другое
				X, // e
				S4, // i
				X, // * or /
				X, // (
				X, // )

			},
			{ // S1
				NO, // Знак
				S2, // 0 - 9
				X, // Точка
				NO, // Пробел
				NO, // Другое
				X, // e
				S4, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S2
				S3, // Знак
				S2, // 0 - 9
				X, // Точка
				YES, // Пробел
				NO, // Другое
				X, // e
				S4, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S3
				NO, // Знак
				S3, // 0 - 9
				X, // Точка
				NO, // Пробел
				NO, // Другое
				X, // e
				S4, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S4
				NO, // Знак
				NO, // 0 - 9
				X, // Точка
				YES, // Пробел
				NO, // Другое
				X, // e
				NO, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S5
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S6
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S7
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S8
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
		},
		{ // Таблица 4
			{ // S0
				S1, // Знак
				S2, // 0 - 9
				X, // Точка
				ERR, // Пробел
				ERR, // Другое
				X, // e
				X, // i
				ERR, // * or /
				X, // (
				X, // )

			},
			{ // S1
				NO, // Знак
				S2, // 0 - 9
				X, // Точка
				NO, // Пробел
				NO, // Другое
				X, // e
				X, // i
				NO, // * or /
				X, // (
				X, // )
			},
			{ // S2
				S3, // Знак
				NO, // 0 - 9
				X, // Точка
				YES, // Пробел
				NO, // Другое
				X, // e
				X, // i
				S4, // * or /
				X, // (
				X, // )
			},
			{ // S3
				NO, // Знак
				S2, // 0 - 9
				X, // Точка
				NO, // Пробел
				NO, // Другое
				X, // e
				X, // i
				NO, // * or /
				X, // (
				X, // )
			},
			{ // S4
				S3, // Знак
				S2, // 0 - 9
				X, // Точка
				NO, // Пробел
				NO, // Другое
				X, // e
				X, // i
				NO, // * or /
				X, // (
				X, // )
			},
			{ // S5
				NO, // Знак
				S5, // 0 - 9
				NO, // Точка
				YES, // Пробел
				NO, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S6
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S7
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
			{ // S8
				X, // Знак
				X, // 0 - 9
				X, // Точка
				X, // Пробел
				X, // Другое
				X, // e
				X, // i
				X, // * or /
				X, // (
				X, // )
			},
		},

	};





	int t{};
	while (true) {
		system("cls");
		std::cout << " 1) постоянная точка\n 2) плавающая точка\n 3) комплексное число\n 4) простая арифметика\n 5) спаренность скобок\n 6) выйти\n";
	 

		std::string word;
		std::cout << "Введите задание: ";
		std::string temp;
		std::getline(std::cin, temp);
		if (std::all_of(temp.begin(), temp.end(), isdigit)) {
			int t_temp = std::stoi(temp);
			if ((t_temp > 0) && (t_temp < 7))
				t = t_temp;
			else
			{
				std::cout << "Напиши нормально задание, тогда поговорим";
				getchar();
				continue;
			}
		}
		else {
			std::cout << "Напиши нормально задание, тогда поговорим";
			getchar();
			continue;
		}



		if (t == 6) break;
		t--;


		 
		std::cout << "Введите выражение: ";
		std::getline(std::cin, word);
		if (word.back() != '$') word.push_back('$');
		if (t != 4) {
			Stack<state, 1> st(S0);


			int i = 0;
			for (char& ch : word)
			{
				i++;
				state current = st.pop();
				switch (ch)
				{
				case '+':
				case '-': st.push(table[t][current][SIGN]); break;
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': st.push(table[t][current][NUMBER]); break;
				case '.': st.push(table[t][current][DOT]); break;
				case ' ': st.push(current); break; 
				case '$': st.push(table[t][current][SPACE]); break;
				case 'e': st.push(table[t][current][EXP_SIGN]); break;
				case 'i':
				case 'j': st.push(table[t][current][COMPLEX_I]); break;
				case '*':
				case '/': st.push(table[t][current][SPECIAL_SIGN]); break;
				case '(': st.push(table[t][current][BRACKET_LEFT]); break;
				case ')': st.push(table[t][current][BRACKET_RIGHT]); break;

				default:  st.push(table[t][current][DEFAULT]); break;
				}
				if (st.peek() < S0) break;
			}

			std::cout << st.peek();

			 
		}
		else {
			bool not_answered = true;
			Stack<bool, 128> brackets;
			for (char& ch : word) {
				if (ch == '(')
					brackets.push(true);
				else if (ch == ')') {
					if (brackets.empty()) {
						std::cout << "NO";
						not_answered = false;
						break;
					}
					else
						brackets.pop();
				}

			}
			if (not_answered) {
				if (brackets.empty())
					std::cout << "YES";
				else std::cout << "NO";

			}

		}
		std::cin.get();

	}


}

