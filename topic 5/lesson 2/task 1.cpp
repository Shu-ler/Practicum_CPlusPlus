#include <iostream>
#include <string>
#include <cctype>

using namespace std;

void PrintUpper(string buffer)
{
    size_t buffer_size = buffer.size();
    for (size_t i = 0; i < buffer_size; ++i)
    {
        buffer[i] = toupper(buffer[i]);
    }
    cout << buffer << endl;
}

void PrintLower(string buffer)
{
    size_t buffer_size = buffer.size();
    for (size_t i = 0; i < buffer_size; ++i)
    {
        buffer[i] = tolower(buffer[i]);
    }
    cout << buffer << endl;
}

int main()
{
    string str;
    getline(cin, str);
    PrintUpper(str);
    PrintLower(str);
}