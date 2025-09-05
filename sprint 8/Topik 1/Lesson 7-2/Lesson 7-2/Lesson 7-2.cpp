#include <cassert>
#include <iostream>
#include <string_view>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target);
    string_view TranslateForward(string_view source) const;
    string_view TranslateBackward(string_view target) const;

private:

};

void Translator::Add(string_view source, string_view target) {
}

string_view Translator::TranslateForward(string_view source) const {
    return string_view();
}

string_view Translator::TranslateBackward(string_view target) const {
    return string_view();
}

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
}
