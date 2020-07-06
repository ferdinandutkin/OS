#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <iomanip>
#include <sstream>
#include <iterator>

#include <io.h>
#include <fcntl.h>


std::wstring calculate(std::wstring tosolve);
std::wstring bracketsolver(std::wstring bracket);
void strreplace(std::wstring& data, const std::wstring& match, const std::wstring& replace);


/*WIP! В ПРИНЦИПЕ НЕ РАБОТАЕТ*/





void strreplace(std::wstring& data, const std::wstring& match, const std::wstring& replace) {

	size_t pos = data.find(match);

	while (pos != std::wstring::npos) {
		data.replace(pos, match.size(), replace);
		pos = data.find(match, pos + replace.size());
	}
}



std::wstring bracketsolver(std::wstring bracket) {

	std::vector<long double> operands;
	std::vector<wchar_t> operators;
	std::wstringstream ssbraket(bracket);
	std::wstring something;


	while (ssbraket >> something) {
		if ((something.length() == 1) && !isdigit(something[0])) {
			if ((operators.size() >= operands.size()) && operators.size())
				operators.back() = ((operators.back() == L'+') == (something[0] == L'+')) ? L'+' : L'-';
			else
				operators.push_back(something[0]);
		}
		else if ((!operands.size()) && (operators.size() == 1)) {
			if ((operators[0] != L'*') && (operators[0] != L'/'))
				operands.push_back(stold(operators[0] + something));
			operators.pop_back();
		}
			
		else operands.push_back(stold(something));
	} //тут чета все сломалось



	while (operands.size() != 1) {
		long double operandl, operandr;
		ptrdiff_t dist;
		std::vector<wchar_t>::iterator found;

		std::unordered_map<wchar_t, std::function<long double(long double, long double)>> operations{
					{L'/', std::divides<long double>() }, //или {'/',[&](long double operandl, long double operandr) { return operandl / operandr; } }, 
					{L'*', std::multiplies<long double>() },
					{L'-', std::minus<long double>() },
					{L'+', std::plus<long double>() }

		};
		for (auto const& [symbol, operation] : operations) {
			while ((found = find(operators.begin(), operators.end(), symbol)) != operators.end()) {
				dist = distance(operators.begin(), found);

				operandl = operands[dist];
				operands.erase(operands.begin() + dist); //получаем левый операнд и удаляем его


				operandr = operands[dist];
				operands.erase(operands.begin() + dist);  //правый теперь на той же позиции получаем и удаляем его


				operands.insert(operands.begin() + dist, operation(operandl, operandr)); //применяем операцию к операндам и помещаем результат к оставшиммя 


				operators.erase(found);

			}
		}
	}
	return std::to_wstring(operands[0]);

}


std::wstring calculate(std::wstring tosolve) {
	


	if (tosolve.find(L'=') != std::wstring::npos)
		return (L"");
	while (tosolve.find(L'(') != std::wstring::npos) {

		std::wstring bracket = tosolve.substr(tosolve.find_last_of(L'('), tosolve.find_first_of(L')') - tosolve.find_last_of(L'(') + 1);

		strreplace(tosolve, bracket, bracketsolver(bracket.substr(1, bracket.length() - 2)));
	}

	

	std::wstringstream formattingstream;
	formattingstream  << std::noshowpoint << stold(bracketsolver((tosolve)));

	return(L" = " + formattingstream.str());

}
