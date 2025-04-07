#include <iostream>
#include <string>

using namespace std;

void CountAndPrint(string str, char character)
{
    int count = 0;
    size_t pos = str.find(character);
    while (pos != string::npos)
    {
        count++;
        pos = str.find(character, pos + 1);
    }
    cout << "В строке \"" << str
         << "\" символ '" << character
         << "' встречается " << count << " раз(а)." << endl;
}

// Добавьте сюда функцию SplitAndAnalyze.
void SplitAndAnalyze(string str, char del, char ch)
{
}

int main()
{
    string str;
    getline(cin, str);

    char del;
    cin >> del;

    char ch;
    cin >> ch;

    SplitAndAnalyze(str, del, ch);
}
