#include <cassert>
#include <iostream>
#include <string_view>
#include <map>
#include <deque>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target);
    string_view TranslateForward(string_view source) const;
    string_view TranslateBackward(string_view target) const;

private:
    map<string_view, string_view> forward_{};
    map<string_view, string_view> backward_{};
    deque<string> words_{};
};

void Translator::Add(string_view source, string_view target) {
    
    // Добавляем строки в deque, если их ещё нет там
    auto& source_str = words_.emplace_back(source);
    auto& target_str = words_.emplace_back(target);

    // Создаём string_view для ссылок на строки в deque
    string_view sv_source = source_str;
    string_view sv_target = target_str;

    // Сохраняем пары переводов в maps
    forward_[sv_source] = sv_target;
    backward_[sv_target] = sv_source;
}

string_view Translator::TranslateForward(string_view source) const {
    auto it = forward_.find(source);
    if (it != forward_.end()) {
        return it->second;
    }
    return string_view();
}

string_view Translator::TranslateBackward(string_view target) const {
    auto it = backward_.find(target);
    if (it != backward_.end()) {
        return it->second;
    }
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
