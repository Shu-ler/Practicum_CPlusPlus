#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) {
        return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
            });
    return before_sentence_end == tokens_end ? tokens_end : next(before_sentence_end);
}

template<typename Token>
void SkipEndSentence(auto& iter, std::vector<Token>& tokens) {
    while (iter != tokens.end() && iter->IsEndSentencePunctuation()) {
        iter = std::next(iter);
    }
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<Sentence<Token>> sentences;
    auto sentenceBegin = tokens.begin();

    // Пропуск токенов, являющихся концом предложения, в начале вектора
    SkipEndSentence(sentenceBegin, tokens);

    while (sentenceBegin != tokens.end()) {
        auto sentenceEnd = std::find_if(sentenceBegin, tokens.end(), [](const Token& token) {
            return token.IsEndSentencePunctuation();
            });

        // Пропускаем все знаки препинания в конце предложения
        SkipEndSentence(sentenceEnd, tokens);

        // Создаём новое предложение и добавляем его в вектор sentences
        Sentence<Token> currentSentence(sentenceBegin, sentenceEnd);
        sentences.push_back(std::move(currentSentence));

        // Переходим к следующему предложению
        sentenceBegin = sentenceEnd;
        SkipEndSentence(sentenceEnd, tokens);
    }

    return sentences;
}


struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s} }))
        == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}} }));

    assert(SplitIntoSentences(vector<TestToken>({ {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true} }))
        == vector<Sentence<TestToken>>({ {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}} }));

    assert(SplitIntoSentences(vector<TestToken>(
        { {"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true} }))
        == vector<Sentence<TestToken>>({
            {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
            {{"Without"s}, {"copies"s}, {"."s, true}},
            }));
}

int main() {
    TestSplitting();
}
