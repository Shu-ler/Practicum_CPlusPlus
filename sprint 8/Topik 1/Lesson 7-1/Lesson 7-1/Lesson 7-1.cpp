#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

vector<string_view> SplitIntoWordsView(string_view str) {
	vector<string_view> result;

	// Удалите начало из str до первого непробельного символа, воспользовавшись методом
	// remove_prefix.Он уберёт из string_view указанное количество символов.
	str.remove_prefix(min(str.find_first_not_of(" "), str.size()));

	while (!str.empty()) {

		// В цикле используйте метод find с одним параметром, чтобы найти номер позиции
		// первого пробела.
		int64_t space = str.find(' ');
		int64_t non_space = min(str.find_first_not_of(" ", space), str.size());

		// Добавьте в результирующий вектор элемент string_view, полученный вызовом метода substr,
		// где начальная позиция будет 0, а конечная — найденная позиция пробела или npos.
		// result.push_back(space == pos_end ? str.substr(0) : str.substr(0, space));
		result.push_back(str.substr(0, space));

		// Сдвиньте начало str так, чтобы оно указывало на позицию за пробелом. Это можно 
		// сделать методом remove_prefix, передвигая начало str на указанное в аргументе
		// количество позиций.
		str.remove_prefix(non_space);
	}

	return result;
}

int main() {
	assert((SplitIntoWordsView("") == vector<string_view>{}));
	assert((SplitIntoWordsView("     ") == vector<string_view>{}));
	assert((SplitIntoWordsView("aaaaaaa") == vector{ "aaaaaaa"sv }));
	assert((SplitIntoWordsView("a") == vector{ "a"sv }));
	assert((SplitIntoWordsView("a b c") == vector{ "a"sv, "b"sv, "c"sv }));
	assert((SplitIntoWordsView("a    bbb   cc") == vector{ "a"sv, "bbb"sv, "cc"sv }));
	assert((SplitIntoWordsView("  a    bbb   cc") == vector{ "a"sv, "bbb"sv, "cc"sv }));
	assert((SplitIntoWordsView("a    bbb   cc   ") == vector{ "a"sv, "bbb"sv, "cc"sv }));
	assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{ "a"sv, "bbb"sv, "cc"sv }));
	cout << "All OK" << endl;
}
