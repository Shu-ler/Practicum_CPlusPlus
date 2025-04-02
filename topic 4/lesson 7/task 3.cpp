#include <iostream>
#include <vector>

using namespace std;

int main()
{
    std::vector<char> abc{'a', 'b', 'c', 'd'};

    for (char one : abc)
    {
        for (char two : abc)
        {
            for (char three : abc)
            {
                if (one == two || one == three || two == three)
                {
                    cout << one << two << three << endl;
                }
            }
        }
    }
}