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

template <typename T>
T FindSentenceBegin(T tokens_begin, T tokens_end) {
	auto sentence_begin = std::find_if(tokens_begin, tokens_end, [](const auto& token) {
		return !token.IsEndSentencePunctuation();
		});
	return sentence_begin;
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
	vector<Sentence<Token>> sentences;
	auto sentenceBegin = tokens.begin();

	while (sentenceBegin != tokens.end()) {
		auto sentenceEnd = FindSentenceEnd(sentenceBegin, tokens.end());

		// Создаём новое предложение и добавляем его в вектор sentences
		sentences.push_back(Sentence<Token>{make_move_iterator(sentenceBegin), make_move_iterator(sentenceEnd)});

		// Переходим к следующему предложению
		sentenceBegin = FindSentenceBegin(sentenceEnd, tokens.end());
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

void TestStartExclamation() {
	using namespace std;
	vector<TestToken> tokens;
	tokens.push_back({ "!"s, true });
	tokens.push_back({ "Move"s });
	tokens.push_back({ "Hello"s });
	tokens.push_back({ "."s, true });
	tokens.push_back({ "Goodbye"s });
	const auto sentences = SplitIntoSentences(move(tokens));
	assert(sentences[0][0] == TestToken({ "!"s, true }));
	assert(sentences[1][0] == TestToken({ "Move"s }));
	assert(sentences[1][1] == TestToken({ "Hello"s }));
	assert(sentences[1][2] == TestToken({ "."s, true }));
	assert(sentences[2][0] == TestToken({ "Goodbye"s }));
}

int main() {
	TestSplitting();
	TestStartExclamation();
}
