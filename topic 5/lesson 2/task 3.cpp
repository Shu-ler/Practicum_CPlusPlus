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

void SplitAndAnalyze(const string& str, char delimiter, char character)
{
    size_t start = 0;
    size_t end = 0;

    end = str.find(delimiter);
    while (end != string::npos)
    {
        CountAndPrint(str.substr(start, end - start), character);
        start = end + 1;
        end = str.find(delimiter, start);
    }
    CountAndPrint(str.substr(start), character);
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
