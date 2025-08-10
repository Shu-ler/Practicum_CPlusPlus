#include <iostream>
#include <string>

using namespace std;

void CountAndPrint(string str, char ch)
{
    int count = 0;
    size_t last_pos = 0;

    last_pos = str.find(ch);
    while (last_pos != string::npos){
        ++count;
        last_pos = str.find(ch, last_pos + 1);
    }

    cout << "В строке \"" << str <<  "\" символ '" << ch << "' встречается " << count << " раз(а).";
}

int main() {
    string str;
    getline(std::cin, str);

    char ch;
    cin >> ch;

    CountAndPrint(str, ch);
}
