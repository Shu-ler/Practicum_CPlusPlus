#include <string>
#include <iostream>
#include <list>

using SrcText = std::list<char>;
using SrcIterator = SrcText::iterator;

class Editor {
public:
    Editor() {};
    // сдвинуть курсор влево
    void Left();
    // сдвинуть курсор вправо 
    void Right();
    // вставить символ token
    void Insert(char token);
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1);
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1);
    // вставить содержимое буфера в текущую позицию курсора
    void Paste();
    // получить текущее содержимое текстового редактора
    std::string GetText() const;

private:
    // Возвращает итератор, сдвинутый на нужное число позиций. 
    // При выходе за границу строки возваращает end()
    SrcIterator GetEndIterator(size_t tokens);

private:
    SrcText src_{};
    SrcText clip_{};
    SrcIterator cur_pos_ = src_.begin();
};

void Editor::Left() {
    if (cur_pos_ != src_.begin()) {
        --cur_pos_;
    }
}

void Editor::Right() {
    if (cur_pos_ != src_.end()) {
        ++cur_pos_;
    }
}

void Editor::Insert(char token) {
    cur_pos_ = src_.insert(cur_pos_, token);
    Right();
}

void Editor::Cut(size_t tokens) {
    Copy(tokens);
    cur_pos_ = src_.erase(cur_pos_, GetEndIterator(tokens));
}

void Editor::Copy(size_t tokens) {
    clip_.assign(cur_pos_, GetEndIterator(tokens));
}

void Editor::Paste() {
    if (!clip_.empty()) {
        cur_pos_ = src_.insert(cur_pos_, clip_.begin(), clip_.end());
        cur_pos_ = std::next(cur_pos_, clip_.size());
    }
}

std::string Editor::GetText() const {
    return { src_.begin(), src_.end() };
}

SrcIterator Editor::GetEndIterator(size_t tokens) {
    size_t to_end = std::min(tokens, 
                            static_cast<size_t>(std::distance(cur_pos_, src_.end())));
    return std::next(cur_pos_, to_end);
}

using namespace std;

int main() {
    Editor editor;
    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}
