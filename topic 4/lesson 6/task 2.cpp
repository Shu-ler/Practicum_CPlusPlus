#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    size_t N = 0;           // Количество вводимых строк
    size_t K = 0;           // Количество выводимых строк с конца текста
    vector<string> strings; // Текст - вектор строк
    string single_str;      // Буфер ввода текста

    // Ввод количества строк
    cin >> N;
    cin.get();

    // Ввод текста
    for (size_t i = 0; i < N; ++i)
    {
        getline(cin, single_str);
        strings.push_back(single_str);
    }

    // Ввод количества выводимых строк
    cin >> K;

    // Вывод последних строк
    for (size_t i = strings.size(); i > (N - K);)
    {
        --i;
        cout << i << " "s << strings[i] << endl;
    }
}
