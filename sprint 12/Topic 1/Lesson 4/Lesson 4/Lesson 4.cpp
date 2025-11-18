#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
        : first(begin)
        , last(end) {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

private:
    Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
    return IteratorRange{ v.begin(), next(v.begin(), min(top, v.size())) };
}

struct Person {
    string name;
    int age, income;
    bool is_male;
};

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;

    vector<Person> result(count);
    for (Person& p : result) {
        char gender;
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = gender == 'M';
    }

    return result;
}

class PeopleAnalyzer {
public:
    explicit PeopleAnalyzer(istream& input) {
        int count;
        input >> count;
        people_.resize(count);
        for (Person& p : people_) {
            char gender;
            input >> p.name >> p.age >> p.income >> gender;
            p.is_male = (gender == 'M');
        }
    }

    void ProcessCommands(istream& input, ostream& output) const {
        string command;
        while (input >> command) {
            if (command == "AGE") {
                int adult_age;
                input >> adult_age;
                ProcessAgeCommand(adult_age, output);
            }
            else if (command == "WEALTHY") {
                int count;
                input >> count;
                ProcessWealthyCommand(count, output);
            }
            else if (command == "POPULAR_NAME") {
                char gender;
                input >> gender;
                ProcessPopularNameCommand(gender, output);
            }
        }
    }

private:
    vector<Person> people_;  // изменяется только в конструкторе

    void ProcessAgeCommand(int adult_age, ostream& out) const {
        // Считаем количество людей с age >= adult_age
        int count = 0;
        for (const auto& p : people_) {
            if (p.age >= adult_age) {
                ++count;
            }
        }
        out << "There are " << count << " adult people for maturity age " << adult_age << '\n';
    }

    void ProcessWealthyCommand(int count, ostream& out) const {
        vector<const Person*> people_ptrs;
        people_ptrs.reserve(people_.size());
        for (const auto& p : people_) {
            people_ptrs.push_back(&p);
        }

        // Сортируем указатели по убыванию дохода
        partial_sort(people_ptrs.begin(),
            people_ptrs.begin() + min(count, (int)people_ptrs.size()),
            people_ptrs.end(),
            [](const Person* a, const Person* b) {
                return a->income > b->income;
            });

        long long total_income = 0;
        for (int i = 0; i < min(count, (int)people_ptrs.size()); ++i) {
            total_income += people_ptrs[i]->income;
        }

        out << "Top-" << count << " people have total income " << total_income << '\n';
    }

    void ProcessPopularNameCommand(char gender, ostream& out) const {
        vector<string> names;
        for (const auto& p : people_) {
            if ((gender == 'M') == p.is_male) {
                names.push_back(p.name);
            }
        }

        if (names.empty()) {
            out << "No people of gender " << gender << '\n';
            return;
        }

        sort(names.begin(), names.end());

        string most_popular;
        int max_count = 0;
        for (auto i = names.begin(); i != names.end();) {
            auto j = find_if_not(i, names.end(), [i](const string& n) { return n == *i; });
            int count = distance(i, j);
            if (count > max_count) {
                max_count = count;
                most_popular = *i;
            }
            i = j;
        }

        out << "Most popular name among people of gender " << gender
            << " is " << most_popular << '\n';
    }
};

int main() {
    PeopleAnalyzer analyzer(cin);
    analyzer.ProcessCommands(cin, cout);
}