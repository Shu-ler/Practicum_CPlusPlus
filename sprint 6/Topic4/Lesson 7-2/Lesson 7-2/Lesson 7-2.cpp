#include <iostream>
#include <string>

using namespace std;

template <typename F>
string BruteForceRec(F check, const string& begin, int n) {
    if (n == 5) {
        return check(begin) ? begin : ""s;
    }
    for (char c = 'A'; c != 'Z' + 1; ++c) {
        string res = BruteForceInternal(check, begin + string({ c }), n + 1);
        if (!res.empty()) {
            return res;
        }
    }
    return ""s;
}

template <typename F>
string BruteForce(F check) {
    return BruteForceRec(check, ""s, 0);
}

int main() {
    string pass = "ARTUR"s;
    auto check = [pass](const string& s) {
        return s == pass;
        };
    cout << BruteForce(check) << endl;
}
