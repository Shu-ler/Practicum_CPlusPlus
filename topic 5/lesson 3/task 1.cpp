#include <iostream>
#include <string>
#include <cctype>

using namespace std;

string ToUpper(string buffer)
{
    string result;

    for (unsigned char i : buffer)
    {
        result += toupper(i);
    }
    return result;
}

string ToLower(string buffer)
{
    string result;

    for (unsigned char i : buffer)
    {
        result += tolower(i);
    }
    return result;
}

int main()
{
    string str;
    getline(cin, str);

    cout << "Upper: " << ToUpper(str) << endl;
    cout << "Lower: " << ToLower(str) << endl;
}
