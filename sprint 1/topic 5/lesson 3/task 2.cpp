#include <iostream>
#include <cstdint>

using namespace std;

uint64_t GetFactorial(int n)
{
    uint64_t result = 1;

    if (n > 1)
    {
        for (int i = 2; i <= n; ++i)
        {
            result *= i;
        }
        
    }

    return result;
}

int main()
{
    int n;
    cin >> n;
    cout << GetFactorial(n) << endl;
}